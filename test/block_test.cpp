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
		{"A", "1234"},
		{"AA", "56789"},
		{"AAA", "0000xxx0000"},
		{"B", "pyt"},
		{"BB", "hello world"},
		{"BBB", "This is sstable"},
		{"C", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"},
		{"CC", "oooooooooooo"},
		{"CCC", "hello world"},
		{"DDD", "This is end"},
	};

	Option option;
	option.block_restart_interval = 3; 
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


TEST(BlockTest, SeekTarget) {
	std::vector<std::pair<std::string, std::string>> data={
		{"A", "1234"},
		{"AA", "56789"},
		{"AAA", "0000xxx0000"},
		{"B", "pyt"},
		{"BB", "hello world"},
		{"BBB", "This is sstable"},
		{"C", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"},
		{"CC", "oooooooooooo"},
		{"CCC", "hello world"},
		{"DDD", "This is end"},
	};

	Option option;
	option.block_restart_interval = 3; 
	BlockBuilder block_builder(option);
	for(auto& dt : data) {
		block_builder.Add(dt.first, dt.second);
	}

	auto buf = block_builder.Finish();

	Block block(buf.ToString());
	std::unique_ptr<Iterator> iter = block.NewIterator();
	std::string str("BB");
	util::Stringview target(str);
	iter->Seek(target);
	ASSERT_EQ(iter->Valid(), true);
	ASSERT_EQ(iter->Key(), std::string("BB"));
	ASSERT_EQ(iter->Value(), std::string("hello world"));
}

} //namespace table
} //namespace kvdb

