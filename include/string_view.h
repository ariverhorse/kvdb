#ifndef _KVDB_UTIL_STRING_VIEW_
#define _KVDB_UTIL_STRING_VIEW_

#include <string>
#include <algorithm>

namespace kvdb {
namespace util {

// This is a Stringview implementation that provide similar functionality as 
// std::string_view in c++17 (I don't have g++ support for c++17 yet, so just
// build one for now, will replace it with std::string_view when it is available)

class Stringview {
 public:

	Stringview();

  Stringview(const char* s, size_t len);

	Stringview(const char* s);

  Stringview(const std::string& str);

  Stringview(const Stringview& other);

	Stringview& operator=(const Stringview& other);
  
  void RemovePrefix(size_t n); 

	bool Empty() const;

	size_t Length() const;

	size_t Size() const;

	const char operator[](size_t idx) const;
 
	std::string ToString() const;

	bool StartsWith(const char* str) const;

	bool StartsWith(const Stringview& other) const;

	const char* Data() const;
 
 private:
	const char* start_;
  size_t length_;	
};

} //namespace util
} //namespace kvdb

#endif
