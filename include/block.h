#ifndef _KVDB_UTIL_BLOCK_
#define _KVDB_UTIL_BLOCK_

// The basic building block of sstable
// The format of my sstable is a simplified version compared with
// leveldb's sstable (will bring more advanced feature such as 
// prefix compression, bloomfilter, etc back in the future)
//
// SSTable Format:
// Block 0
// Block 1
// Block 2
// ....
// Block N
// Block Index
// Footer(block handle of Index block, and magic number)
//
// Block Format
// [shared_key_len][non_shared_key_len][val_len][non_shared_key][val]
// [shared_key_len][non_shared_key_len][val_len][non_shared_key][val]
// [shared_key_len][non_shared_key_len][val_len][non_shared_key][val]
// [shared_key_len][non_shared_key_len][val_len][non_shared_key][val]
// .....
// [Restart offset1] fixint32
// [Restart offset2] fixint32
// ....
// [Restart offsetN] fixint32
// [Restart block offset] fixint32
// (Notice that shared key len, to val_len are all varint32)


#include <string>
#include <cassert>
#include <memory>
#include <vector>
#include "include/string_view.h"
#include "include/varint.h"
#include "include/iterator.h"
#include "include/option.h"

namespace kvdb {
namespace table {

// A data block contains key value pair
class Block {
	public:
		explicit Block(const std::string& content) : content_(content) { }
    virtual ~Block() = default;
		//TODO(pengyuantao) add comparator interface
		std::unique_ptr<Iterator> NewIterator();

	private:
		const std::string& content_;
		class BlockIterator;
};

// A data block contains key value pair
class BlockBuilder {
	public:
		BlockBuilder(Option& option) :
			restart_interval_(option.block_restart_interval),
      counter_(0){
      key_.clear();
      restarts_.push_back(0);
		}

    virtual ~BlockBuilder() = default;

		virtual void Add(const util::Stringview& key,
										 const util::Stringview& value);
		virtual util::Stringview Finish();

	private:
		std::string content_;	
    std::string key_;
    int counter_;
		int restart_interval_;
    std::vector<int32_t> restarts_;
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
