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

