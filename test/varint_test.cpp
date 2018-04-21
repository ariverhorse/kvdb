#include "include/varint.h"
#include "gtest/gtest.h"

namespace kvdb {
namespace util {

TEST(VarintTest, Fix32Int) {
	uint32_t data = 36783;
	std::string buf;
	PutFix32(buf, data);
	uint32_t expected;
	Stringview sv(buf);
	GetFix32(sv, expected);
	ASSERT_EQ(expected, data);
}

TEST(VarintTest, Fix64Int) {
	uint64_t data = 9192714390;
	std::string buf;
	PutFix64(buf, data);
	uint64_t expected;
	Stringview sv(buf);
	GetFix64(sv, expected);
	ASSERT_EQ(expected, data);
}

TEST(VarintTest, Var32Int) {
	uint32_t data = 36783;
	std::string buf;
	PutVar32(buf, data);
	uint32_t expected;
	Stringview sv(buf);
	GetVar32(sv, expected);
	ASSERT_EQ(expected, data);
}

TEST(VarintTest, Var64Int) {
	uint64_t data = 9192714390;
	std::string buf;
	PutVar64(buf, data);
	uint64_t expected;
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

TEST(VarintTest, LengthPrefix) {
	const std::string str1 = "pengyuantao";
	const std::string str2 = "is working at Google";	
	const Stringview sv1(str1);
	const Stringview sv2(str2);
	std::string buf;
	PutLengthPrefixedString(buf, sv1);
	PutLengthPrefixedString(buf, sv2);
	Stringview svbuf(buf);
	Stringview sv3;
	Stringview sv4;
	GetLengthPrefixedString(svbuf, sv3);
  GetLengthPrefixedString(svbuf, sv4);
	//ASSERT_EQ(sv3, str1);
	ASSERT_EQ(str1, sv3);
	ASSERT_EQ(sv4.ToString(), str2);
}

TEST(VarintTest, Varint32Overflow) {
	uint32_t result;
	Stringview sv("\x81\x82\x83\x84\x85\0x11");
	ASSERT_FALSE(GetVar32(sv, result));
}

TEST(VarintTest, Varint64Overflow) {
	uint64_t result;
	Stringview sv("\x81\x82\x83\x84\x85\x81\x82\x83\x84\x85\0x11");
	ASSERT_FALSE(GetVar64(sv, result));
}

} //namespace util
} //namespace kvdb
