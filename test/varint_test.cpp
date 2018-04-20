#include "include/varint.h"
#include "gtest/gtest.h"

namespace kvdb {
namespace util {

TEST(VarintTest, Fix32Int) {
	uint32_t data = 36783;
	std::string buf;
	PutFix32(buf, data);
	uint32_t expected = 0;
	Stringview sv(buf);
	GetFix32(sv, expected);
	ASSERT_EQ(expected, data);
}

TEST(VarintTest, Fix64Int) {
	uint64_t data = 9192714390;
	std::string buf;
	PutFix64(buf, data);
	uint64_t expected = 0;
	Stringview sv(buf);
	GetFix64(sv, expected);
	ASSERT_EQ(expected, data);
}

TEST(VarintTest, Var32Int) {
	uint32_t data = 36783;
	std::string buf;
	PutVar32(buf, data);
	uint32_t expected = 0;
	Stringview sv(buf);
	GetVar32(sv, expected);
	ASSERT_EQ(expected, data);
}

TEST(VarintTest, Var64Int) {
	uint64_t data = 9192714390;
	std::string buf;
	PutVar64(buf, data);
	uint64_t expected = 0;
	Stringview sv(buf);
	GetVar64(sv, expected);
	ASSERT_EQ(expected, data);
}

TEST(VarintTest, Fixed32) {
	std::string s;
	for(uint32_t v=0; v<10000; ++v) {
		PutFix32(s, v);
	}

	Stringview sv(s);
	for(uint32_t v=0; v<10000; ++v) {
		uint32_t val;
		GetFix32(sv, val);
		ASSERT_EQ(val, v);
		sv.RemovePrefix(4);
	}	
}

TEST(VarintTest, Fix32) {
	std::string s;
	for(uint32_t v=0; v<10000; ++v) {
		PutFix32(s, v);
	}

	Stringview sv(s);
	for(uint32_t v=0; v<10000; ++v) {
		uint32_t val;
		GetFix32(sv, val);
		ASSERT_EQ(val, v);
		sv.RemovePrefix(4);
	}	
}

TEST(VarintTest, Var32) {
	std::string s;
	for(uint32_t v=0; v<10000; ++v) {
		PutVar32(s, v);
	}

	Stringview sv(s);

	for(uint32_t v=0; v<10000; ++v) {
		uint32_t val=0;
		GetVar32(sv, val);
		ASSERT_EQ(val, v);
	}	
}


} //namespace util
} //namespace kvdb
