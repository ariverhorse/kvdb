#include "include/block.h"
#include "include/option.h"
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

	Option option;
	option.block_restart_interval = 12;
	BlockBuilder block_builder(option);
	for(auto& dt : data) {
		block_builder.Add(dt.first, dt.second);
	}

	auto buf = block_builder.Finish();

	Block block(buf.ToString());
	std::unique_ptr<Iterator> iter = block.NewIterator();
	iter->SeekToFirst();
	int i = 0;
	while(iter->Valid()) {
		ASSERT_EQ(iter->Key(), data[i].first);
		ASSERT_EQ(iter->Value(), data[i].second);
		++i;
		iter->Next();
	}

}


} //namespace table
} //namespace kvdb

