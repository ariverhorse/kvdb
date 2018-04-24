#include "include/block.h"
#include "include/varint.h"
#include <utility>

namespace kvdb {
namespace table {

void BlockBuilder::Add(const util::Stringview& key,
								const util::Stringview& value) {
	PutLengthPrefixedString(content_, key);
	PutLengthPrefixedString(content_, value);
}

util::Stringview BlockBuilder::Finish() {
	return util::Stringview(content_);
}

Block::Iterator Block::Begin() {
	return Block::Iterator(*this);
}

Block::Iterator Block::End() {
	return Block::Iterator();	
}


} //table
} //kvdb
