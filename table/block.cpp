#include "include/block.h"
#include "include/varint.h"
#include <utility>

namespace kvdb {
namespace table {

void Block::Add(const util::Stringview& key,
								const util::Stringview& value) {
	PutLengthPrefixedString(content_, key);
	PutLengthPrefixedString(content_, value);
}

void Block::Finish() {
	//TODO(pengyuantao): add prefix compress, the finish function
  //should be used to build the restart point index
}

size_t Block::Size() {
	return content_.size();
}

Block::Iterator Block::Begin() {
	return Block::Iterator(*this);
}

Block::Iterator Block::End() {
	return Block::Iterator();	
}


} //table
} //kvdb
