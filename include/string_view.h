#ifndef _KVDB_UTIL_STRING_VIEW_
#define _KVDB_UTIL_STRING_VIEW_

#include <string>
#include <algorithm>

namespace kvdb {
namespace util {

// This is a string_view implementation that provide similar functionality as 
// std::string_view in c++17 (I don't have g++ support for c++17 yet, so just
// build one for now, will replace it with std::string_view when it is available)

class string_view {
 public:

	string_view();

  string_view(const char* s, size_t len);

	string_view(const char* s);

  string_view(const std::string& str);

  string_view(const string_view& other);

	string_view& operator=(const string_view& other);
  
  void remove_prefix(size_t n); 

	bool empty() const;

	size_t length() const;

	size_t size() const;

	const char operator[](size_t idx) const;
 
	std::string tostring();

	bool starts_with(const char* str) const;

	bool starts_with(const string_view& other) const;

	const char* data() const;
 
 private:
	const char* start_;
  size_t length_;	
};

} //namespace util
} //namespace kvdb

#endif
