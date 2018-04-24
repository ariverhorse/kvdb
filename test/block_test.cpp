#include "include/block.h"
#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

namespace kvdb {
namespace table {

TEST(BlockTest, BlockWriteAndRead) {
	std::vector<std::pair<std::string, std::string>> data={
		{"K1", "1234"},
		{"KxxxEY2", "56789"},
		{"KrrrrrrrrrrY3", "00000000000"},
		{"4", "pyt"},
		{"Kk5", "hello world"},
		{"KEY6", "This is sstable"},
		{"KEY7", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"},
		{"KEY8", "oooooooooooo"},
		{"KkkkkkkkkkkEY9", "hello world"}
	};

	BlockBuilder block_builder;
	for(auto& dt : data) {
		block_builder.Add(dt.first, dt.second);
	}

	auto buf = block_builder.Finish();

	Block block(buf);
	auto begin = block.Begin();
	auto end = block.End();
	int i=0;
	while(begin!=end) {
		std::pair<util::Stringview, util::Stringview> keyval = *begin;
		ASSERT_EQ(keyval.first, data[i].first);
		ASSERT_EQ(keyval.second, data[i].second);
		++i;
		++begin;
	}

}

TEST(BlockTest, BlockHandle) {
	uint32_t offset = 17891;
	uint32_t size = 1239;
	BlockHandle handle(offset, size);
	std::string buf;
  handle.EncodeTo(buf);
	BlockHandle test;
	util::Stringview sv(buf);
	test.DecodeFrom(sv);
	ASSERT_EQ(test.Offset(), offset);
	ASSERT_EQ(test.Size(), size);
}


TEST(BlockTest, FooterTest) {
	uint32_t offset = 17891;
	uint32_t size = 1239;
	BlockHandle index(offset, size);
  Footer footer(index);
	std::string buf;
	footer.EncodeTo(buf);
	Footer test;
	util::Stringview sv(buf);
	test.DecodeFrom(sv);
	auto testhandler = test.IndexHandle();
	ASSERT_EQ(testhandler.Offset(), offset);
	ASSERT_EQ(testhandler.Size(), size);
}

} //namespace table
} //namespace kvdb

