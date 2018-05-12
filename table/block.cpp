#include "include/block.h"
#include "include/varint.h"
#include <utility>
#include <vector>
#include <algorithm>

namespace kvdb {
namespace table {

namespace {
		
	const char* DecodeEntry(const char* p, const char* limit, int32_t& shared, int32_t& nonshared, int32_t& vallen) {
		if((p = util::GetVar32Ptr(p, limit, shared)) == nullptr) {
			return nullptr;
		}
		if((p = util::GetVar32Ptr(p, limit, nonshared)) == nullptr) {
			return nullptr;
		}
		if((p = util::GetVar32Ptr(p, limit, vallen)) == nullptr) {
			return nullptr;
		}
		if(static_cast<int32_t>(limit-p) < (*nonshared+*vallen)) {
			return nullptr;
		}
		return p;
	}			
}

class Block::BlockIterator : public Iterator {
	public:
		BlockIterator(const Block& block) :
		  data_(block.content_.c_str()),
			size_(block.content_.size()),
			num_restart_(_GetNumRestarts()),
			restart_offset_(size_-(1+num_restart_)*sizeof(int32_t)),
			cur_offset_(restart_offset_),
			current_restart_index_(num_restart_){
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


		}

	private:
		const char* data_;
		int32_t size_;
		int32_t num_restart_;
		int32_t restart_offset_;	

		int32_t cur_offset_; //current offset	
		int32_t cur_restart_index_; //current restart index
		std::string cur_key_;  //current key
		util::Stringview cur_val_; //current value
	
		int32_t _GetNumRestarts() {
			util::Stringview sv(data_+size_- sizeof(int32_t), sizeof(int32_t));
			util::GetFix32(sv, num_restart_);	
		}
	
		int32_t _GetRestartOffset(int index) {
			util::Stringview sv(data_+restart_offset_+index*sizeof(int32_t), sizeof(int32_t));
			int32_t offset;
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
			int32_t shared;
			int32_t nonshared;
			int32_t vallen;
			const char* p = DecodeEntry(data_+cur_offset_, data_+restart_offset_, shared, nonshared, vallen);
			if(p==nullptr) {
				_CorruptionError();
				return;
			}	
			cur_key_.resize(shared);
			cur_key_.append(p, nonshared);
			cur_val_ = util::Stringview(p+nonshared, vallen);
			if(cur_restart_index_+1 < num_restarts_ && cur_offset_ >= _GetRestartOffset(cur_restart_index_+1)) {
				++cur_restart_index_;
			}
		}
};

void BlockBuilder::Add(const util::Stringview& key,
								const util::Stringview& value) {

  int32_t shared = 0;
  int32_t nonshared = 0;
  int32_t vallen = value.Size();
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
  key.RemovePrefix(shared);
  content_ += key.ToString();
  content_ += value.ToString();
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
