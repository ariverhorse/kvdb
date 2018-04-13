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
	string_view() : start_(nullptr), length_(0) { }

  string_view(const string_view& other) {
	  start_ = other.start_;
	  length_ = other.length_;
  }

  string_view(const char* s, size_t len)
   : start_(s), length_(len) {}

  string_view(const std::string& str)
   : start_(str.c_str()), length_(str.length()) {}

  void remove_prefix(size_t n); 
	bool empty() const {
		return (length_ == 0);	
	}

	const char& operator[](size_t idx) const {
		return start_[idx];
	}

	std::string tostring();
 
 private:
	const char* start_;
  size_t length_;	
};

} //namespace util
} //namespace kvdb

#endif
