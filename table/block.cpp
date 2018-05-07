#include "include/block.h"
#include "include/varint.h"
#include <utility>

namespace kvdb {
namespace table {

namespace {
	class BlockIterator : public Iterator {
		public:
			BlockIterator(const Block& block)
				: buf_(block.Content().c_str()),
					size_(block.Content().size()),
					end_(buf_+size_),
					next_(buf_) {

			}

			bool Valid() const {
				return key_.Size()!=0;
			}

			util::Stringview Key() const {
				return key_;
			}

			util::Stringview Value() const {
				return value_;
			}

			void SeekToFirst() {
				next_ = buf_;
				_ParseNextEntry();		
			}

			void Next() {
				next_ = value_.Data()+value_.Size();
				_ParseNextEntry();
			}

			void Prev() {

			}

			void SeekToLast() {

			}

			void Seek(const util::Stringview& target) {


			}

		private:
			const char* buf_;
			int size_;
			const char* end_;
			const char* next_;
			util::Stringview key_;
			util::Stringview value_;
    	void _ParseNextEntry() {
				key_ = util::Stringview();
				value_ = util::Stringview();
				util::Stringview sv(next_, end_-next_);
    		GetLengthPrefixedString(sv, key_);
    		GetLengthPrefixedString(sv, value_);
    	}	

	};

}

void BlockBuilder::Add(const util::Stringview& key,
								const util::Stringview& value) {
	PutLengthPrefixedString(content_, key);
	PutLengthPrefixedString(content_, value);
}

util::Stringview BlockBuilder::Finish() {
	return util::Stringview(content_);
}


std::unique_ptr<Iterator> Block::NewIterator() {
	return std::make_unique<BlockIterator>(*this);
}


} //table
} //kvdb
