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

void GetFix32(Stringview& buf, uint32_t& data);
void GetFix64(Stringview& buf, uint64_t& data);
void GetVar32(Stringview& buf, uint32_t& data);
void GetVar64(Stringview& buf, uint64_t& data);


} //namespace util
} //namespace kvdb


#endif
