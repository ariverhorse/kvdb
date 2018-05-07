#ifndef _KVDB_UTIL_VARINT_
#define _KVDB_UTIL_VARINT_

#include <string>
#include "include/string_view.h"

namespace kvdb {
namespace util {

void PutFix32(std::string& buf, uint32_t data);
void PutFix64(std::string& buf, uint64_t data);
void PutVar32(std::string& buf, uint32_t data);
void PutVar64(std::string& buf, uint64_t data);
void PutLengthPrefixedString(std::string& buf, const Stringview& sv);

// These Get* functions will get the value and store it in data, 
// then advance the Stringview passed the parsed value
bool GetFix32(Stringview& buf, uint32_t& data);
bool GetFix64(Stringview& buf, uint64_t& data);
bool GetVar32(Stringview& buf, uint32_t& data);
bool GetVar64(Stringview& buf, uint64_t& data);
bool GetLengthPrefixedString(Stringview& input, Stringview& output);


// These are pointer based GetVar*, which parsed the char stored at
// [start, end), it either return a value in data and a pointer passed
// the parsed value; or return NULL
const char* GetVar32Ptr(const char* start, const char* end, uint32_t& data);
const char* GetVar64Ptr(const char* start, const char* end, uint64_t& data);
} //namespace util
} //namespace kvdb


#endif
