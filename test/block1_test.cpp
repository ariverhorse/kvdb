#include "include/block.h"
#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

namespace kvdb {
namespace table {


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

