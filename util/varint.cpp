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

bool GetFix32(Stringview& buf, uint32_t& data) {
	if(buf.Size()<4) {
		return false;
	}
	data = 0;
	for(int i=3; i>=0; --i) {
		data <<= 8;
		data |= static_cast<unsigned char>(buf[i]);
	}	
	buf.RemovePrefix(4);
	return true;
}

bool GetFix64(Stringview& buf, uint64_t& data){
	if(buf.Size()<4) {
		return false;
	}
	data = 0;
	for(int i=7; i>=0; --i) {
		data <<= 8;
		data |= static_cast<unsigned char>(buf[i]);
	}	
	buf.RemovePrefix(8);
	return true;
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

	
bool GetVar32(Stringview& buf, uint32_t& data) {
	uint32_t mask = 0x7f;
	uint32_t morebit = 0x80;
	int count = 0;
	data = 0;
	for(int i=0; i<buf.Size() && count<5; ++i) {
		data |= (static_cast<unsigned char>(buf[i]) & mask)<<(i*7);		
		++count;
		if((static_cast<unsigned char>(buf[i]) & morebit)==0) {
			buf.RemovePrefix(count);
			return true;
		}
	}
	return false;
}

bool GetVar64(Stringview& buf, uint64_t& data) {
	uint64_t mask = 0x7f;
	uint64_t morebit = 0x80;
	int count = 0;
	data = 0;
	for(int i=0; i<buf.Size() && count < 10; ++i) {
		data |= (static_cast<unsigned char>(buf[i]) & mask)<<(i*7);		
		++count;
		if((static_cast<unsigned char>(buf[i]) & morebit)==0) {
			buf.RemovePrefix(count);
			return true;
		}
	}
	return false;
}


void PutLengthPrefixedString(std::string& buf, const Stringview& sv) {
 	PutVar32(buf, sv.Size());
	buf.append(sv.Data(), sv.Size()); 
} 

bool GetLengthPrefixedString(Stringview& input, Stringview& output) {
	uint32_t len;
	if(GetVar32(input, len)) {
		Stringview sv;
		output = Stringview(input.Data(), len);
		input.RemovePrefix(len);
		return true;
	} else {
		return false;
	}
}

} //namespace util
} //namespace kvdb
