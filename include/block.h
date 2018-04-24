#ifndef _KVDB_UTIL_BLOCK_
#define _KVDB_UTIL_BLOCK_

// The basic building block of sstable
// The format of my sstable is a simplified version compared with
// leveldb's sstable (will bring more advanced feature such as 
// prefix compression, bloomfilter, etc back in the future)
//
// Block 0
// Block 1
// Block 2
// ....
// Block N
// Block Index
// Footer(block handle of Index block, and magic number)
//
// Each block is Varint32 prefixed Key-Value pair
// We don't do prefix comrpession here, and no block compression

#include <string>
#include <cassert>
#include "include/string_view.h"
#include "include/varint.h"

namespace kvdb {
namespace table {

// A data block contains key value pair
class Block {
	public:
		explicit Block(const util::Stringview content) : content_(content) { }
    virtual ~Block() = default;

		//class Iterator;
    class Iterator {
    	public:
    		Iterator(const Block& block) : sv_(block.content_), eof_(false) {
    			_ParseNextEntry();	
    		}
    	
    		Iterator() : sv_(""), eof_(true) {
    		}
    
    		Iterator(const Iterator&) = default;
    		Iterator& operator=(const Iterator&) = default;
    	
    		std::pair<util::Stringview, util::Stringview> operator*() {
    			return std::make_pair(key_, value_);
    		}
    
    		Iterator& operator++() {
    			if(sv_.Size()>0) {
    				_ParseNextEntry();
    			} else if (sv_.Size()==0) {
						eof_ = true;
					} 
    			return *this;
    		}
    
    		Iterator operator++(int) {
    			Iterator tmp = *this;
    			if(sv_.Size()>0) {
    				_ParseNextEntry();
    			}
    			return tmp; 
    		}
    		
    		bool operator==(const Iterator& other) {
    			return this->sv_.Data() == other.sv_.Data()&&
    						 this->sv_.Size() == other.sv_.Size() ||
								 this->sv_.Size()==0 && other.sv_.Size()==0 && 
								 this->eof_ == other.eof_;
    		}
    
    		bool operator!=(const Iterator& other) {
    			return !operator==(other);
    		}
    
    	private:
    		util::Stringview sv_;
    		util::Stringview key_;
    		util::Stringview value_;
				bool eof_;
    		void _ParseNextEntry() {
    			GetLengthPrefixedString(sv_, key_);
    			GetLengthPrefixedString(sv_, value_);
    		}	
    };

		Iterator Begin(); 
		Iterator End();

	private:
		util::Stringview content_;	
};

// A data block contains key value pair
class BlockBuilder {
	public:
		BlockBuilder() = default;
    virtual ~BlockBuilder() = default;

		virtual void Add(const util::Stringview& key,
										 const util::Stringview& value);
		virtual util::Stringview Finish();

	private:
		std::string content_;	
};

//Block handle contens the location and size of the block 
class BlockHandle {
	public:
		const static int kBlockHandleSize = 10;
		BlockHandle() : offset_(0), size_(0) {}
		BlockHandle(uint32_t offset, uint32_t size)
			: offset_(offset), size_(size) { }
		BlockHandle(const BlockHandle&) = default;
		uint32_t Offset() { return offset_; }
		uint32_t Size() { return size_; }
		void SetOffset(uint32_t offset) {  offset_ = offset; }
		void SetSize(uint32_t size) { size_ = size; }
		void EncodeTo(std::string& buf) {
			util::PutVar32(buf, offset_);
			util::PutVar32(buf, size_);
		}
		void DecodeFrom(util::Stringview sv) {
			util::GetVar32(sv, offset_);
			util::GetVar32(sv, size_);
		}	
	private:
		uint32_t offset_;
		uint32_t size_;
};


class Footer {
	public:
		const static int kFooterSize = BlockHandle::kBlockHandleSize + 8; 
		const static uint64_t kMagicNumber = 0x1122334455667788;
		Footer() = default;
		Footer(BlockHandle& handle) : index_handle_(handle) {
			
		}
		~Footer() = default;
    
		void EncodeTo(std::string& buf) {
			util::PutFix64(buf, kMagicNumber);
			std::string index_handle_buf;
			index_handle_.EncodeTo(index_handle_buf);
			index_handle_buf.resize(BlockHandle::kBlockHandleSize);
			buf.append(index_handle_buf);
		}

		void DecodeFrom(util::Stringview sv) {
			uint64_t magicword;
			util::GetFix64(sv, magicword);
			assert(magicword == kMagicNumber);
			index_handle_.DecodeFrom(sv);	
		}

		BlockHandle IndexHandle() { return index_handle_; }
	
	private:
		BlockHandle index_handle_;
};



}//namespace table
}//namespace kvdb

#endif
