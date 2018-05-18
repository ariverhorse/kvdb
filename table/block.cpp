#include "include/block.h"
#include "include/varint.h"
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

namespace kvdb {
namespace table {

namespace {
		
	const char* DecodeEntry(const char* p, const char* limit, uint32_t& shared, uint32_t& nonshared, uint32_t& vallen) {
		if((p = util::GetVar32Ptr(p, limit, shared)) == nullptr) {
			return nullptr;
		}
		if((p = util::GetVar32Ptr(p, limit, nonshared)) == nullptr) {
			return nullptr;
		}
		if((p = util::GetVar32Ptr(p, limit, vallen)) == nullptr) {
			return nullptr;
		}
		if(static_cast<uint32_t>(limit-p) < (nonshared+vallen)) {
			return nullptr;
		}
		return p;
	}

	bool compare(util::Stringview key1, util::Stringview key2) {
		return key1.Compare(key2)<=0;
	}		
}

class Block::BlockIterator : public Iterator {
	public:
		BlockIterator(const Block& block) :
		  data_(block.content_.c_str()),
			size_(block.content_.size()),
			num_restart_(_GetNumRestarts()),
			restart_offset_(size_-(1+num_restart_)*sizeof(uint32_t)),
			cur_offset_(restart_offset_),
			cur_restart_index_(num_restart_){
      cur_key_.clear();
      cur_val_.Clear();
		}

		bool Valid() const {
			return cur_offset_ < restart_offset_; 
		}

		util::Stringview Key() const {
			return util::Stringview(cur_key_);
		}

		util::Stringview Value() const {
			return cur_val_;
		}

		void SeekToFirst() {
			_SeekToRestart(0);
			_ParseCurrentKeyValue();
		}

		void Next() {
			cur_offset_ = cur_val_.Data() + cur_val_.Size() - data_;
			_ParseCurrentKeyValue();
		}

		void Prev() {

		}

		void SeekToLast() {

		}

		void Seek(const util::Stringview& target) {
			//Binary search for restart
			int start = 0;
			int end = num_restart_-1;
			while(start < end) {
				int mid = (start+end+1)/2;
				uint32_t offset = _GetRestartOffset(mid);
				uint32_t shared;
				uint32_t nonshared;
				uint32_t vallen;
				const char* p = DecodeEntry(data_+offset, data_+restart_offset_, shared, nonshared, vallen);
				assert(shared==0);	
				util::Stringview midkey=util::Stringview(p, nonshared);
				bool result = compare(midkey, target);
				if(result) {
					start = mid;
				} else {
					end = mid-1;
				}	
			}
			//start is the offset 	
			_SeekToRestart(start);			
			_ParseCurrentKeyValue();
			if(!compare(util::Stringview(cur_key_), target)) {
				cur_offset_ = restart_offset_;
				cur_key_.clear();
				cur_val_.Clear();
				return;	
			}
			while(compare(util::Stringview(cur_key_), target)) {
				Next();	
				if(cur_offset_ >= restart_offset_) {
					break;
				} 
			}	
		}

	private:
		const char* data_;
		uint32_t size_;
		uint32_t num_restart_;
		uint32_t restart_offset_;	

		uint32_t cur_offset_; //current offset	
		uint32_t cur_restart_index_; //current restart index
		std::string cur_key_;  //current key
		util::Stringview cur_val_; //current value
	
		uint32_t _GetNumRestarts() {
			uint32_t num_restart;
			util::Stringview sv(data_+size_- sizeof(uint32_t), sizeof(uint32_t));
			util::GetFix32(sv, num_restart);	
			return num_restart;
		}

		uint32_t _GetRestartOffset(int index) {
			util::Stringview sv(data_+restart_offset_+index*sizeof(uint32_t), sizeof(uint32_t));
			uint32_t offset;
			util::GetFix32(sv, offset);
			return offset;
		}

		void _SeekToRestart(int index) {
			cur_restart_index_ = index;
			cur_offset_ = _GetRestartOffset(index);	
			cur_key_.clear();
			cur_val_.Clear();
		}

		void _CorruptionError() {

		}

		void _ParseCurrentKeyValue() {
			uint32_t shared;
			uint32_t nonshared;
			uint32_t vallen;
			const char* p = DecodeEntry(data_+cur_offset_, data_+restart_offset_, shared, nonshared, vallen);
			if(p==nullptr) {
				_CorruptionError();
				return;
			}	
			cur_key_.resize(shared);
			cur_key_.append(p, nonshared);
			cur_val_ = util::Stringview(p+nonshared, vallen);
			if(cur_restart_index_+1 < num_restart_ && cur_offset_ >= _GetRestartOffset(cur_restart_index_+1)) {
				++cur_restart_index_;
			}
		}
};

void BlockBuilder::Add(const util::Stringview& key,
								const util::Stringview& value) {

  uint32_t shared = 0;
  uint32_t nonshared = 0;
  uint32_t vallen = value.Size();
  int minlen = std::min(key.Size(), key_.size());
  if(counter_ == restart_interval_) {
    counter_ = 0;
    key_.clear();
    restarts_.push_back(content_.size());
  } 
  for(int i=0; i<minlen; ++i) {
    if(key[i] == key_[i]) {
      ++shared;
    } else {
      break;
    }
  }
  nonshared = key.Size()-shared;
  util::PutVar32(content_, shared);
  util::PutVar32(content_, nonshared);
  util::PutVar32(content_, vallen);
	content_.append(key.Data()+shared, nonshared);
	content_.append(value.Data(), value.Size());
  key_.resize(shared);
  key_.append(key.Data()+shared, nonshared);
  ++counter_;
}

util::Stringview BlockBuilder::Finish() {
  for(int i=0; i<restarts_.size(); ++i) {
    util::PutFix32(content_, restarts_[i]);
  }
  util::PutFix32(content_, restarts_.size());
	return util::Stringview(content_);
}


std::unique_ptr<Iterator> Block::NewIterator() {
	return std::make_unique<BlockIterator>(*this);
}


} //table
} //kvdb
