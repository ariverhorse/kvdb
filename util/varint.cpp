#include "include/varint.h"
#include <iostream>

namespace kvdb {
namespace util {

void PutFix32(std::string& buf, uint32_t data){
	uint32_t mask = 0xff;
	for(int i=0; i<4; ++i) {
	  char c = data & mask;
		buf.push_back(c);
		data = data >> 8;
	}
}

void PutFix64(std::string& buf, uint64_t data){
	uint64_t mask = 0xff;
	for(int i=0; i<8; ++i) {
		char c = data & mask;
		buf.push_back(c);
		data = data >> 8;
	}
}

void GetFix32(Stringview& buf, uint32_t& data) {
	for(int i=3; i>=0; --i) {
		data <<= 8;
		data |= static_cast<unsigned char>(buf[i]);
	}	
}

void GetFix64(Stringview& buf, uint64_t& data){
	for(int i=7; i>=0; --i) {
		data <<= 8;
		data |= static_cast<unsigned char>(buf[i]);
	}	
}

void PutVar32(std::string& buf, uint32_t data){
	uint32_t mask = 0x7f;
	uint32_t morebit = 0x80;
	int count = 0;
	for(int i=0; i<5; ++i) {
		++count;
		if(data <= mask) {
			buf.push_back(data&mask);
			break;
		} else {
			char c = data & mask;
			buf.push_back(c | morebit);
			data >>= 7;
		}
	}	
}


void PutVar64(std::string& buf, uint64_t data){
	uint64_t mask = 0x7f;
	uint64_t morebit = 0x80;
	for(int i=0; i<10; ++i) {
		if(data <= mask) {
			buf.push_back(data&mask);
			return;
		} else {
			char c = data & mask;
			buf.push_back(c | morebit);
			data >>= 7;
		}
	}	

}

	
void GetVar32(Stringview& buf, uint32_t& data) {
	uint32_t mask = 0x7f;
	uint32_t morebit = 0x80;
	int count = 0;
	for(int i=0; i<buf.Size(); ++i) {
		data |= (static_cast<unsigned char>(buf[i]) & mask)<<(i*7);		
		++count;
		if((static_cast<unsigned char>(buf[i]) & morebit)==0) {
			break;
		}
	}
	buf.RemovePrefix(count);
}

void GetVar64(Stringview& buf, uint64_t& data) {
	uint64_t mask = 0x7f;
	uint64_t morebit = 0x80;
	int count = 0;
	for(int i=0; i<buf.Size(); ++i) {
		data |= (static_cast<unsigned char>(buf[i]) & mask)<<(i*7);		
		++count;
		if((static_cast<unsigned char>(buf[i]) & morebit)==0) {
			break;
		}
	}
	buf.RemovePrefix(count);
}




} //namespace util
} //namespace kvdb
