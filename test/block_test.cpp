#include "include/block.h"
#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

namespace kvdb {
namespace table {

TEST(BlockTest, normal) {
	std::vector<std::pair<std::string, std::string>> data={
		{"KEY1", "1234"},
		{"KEY2", "56789"},
		{"KEY3", "00000000000"},
		{"KEY4", "pyt"},
		{"KEY5", "hello world"}
	};

	Block block;
	for(auto& dt : data) {
		block.Add(dt.first, dt.second);
	}

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

} //namespace table
} //namespace kvdb

