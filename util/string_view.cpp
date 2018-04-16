#include "include/string_view.h"
#include <cstring>
#include <cassert>

namespace kvdb {
namespace util {

string_view::string_view()
  : start_(nullptr), length_(0) {}

string_view::string_view(const char* s, size_t len)
  : start_(s), length_(len) {}

string_view::string_view(const char* s)
	: start_(s), length_(strlen(s)) {} 

string_view::string_view(const std::string& str)
  : start_(str.c_str()), length_(str.length()) {}

string_view::string_view(const string_view& other) {
  start_ = other.start_;
	length_ = other.length_;
}

string_view& string_view::operator=(const string_view& other) {
	if(this != &other) { 
		start_ = other.start_;
		length_ = other.length_;
 	} 
	return *this;	
}

void string_view::remove_prefix(size_t n) {
	assert(n<=length_);
	start_ += n;
  length_ -= n;
}

bool string_view::empty() const {
	return (length_ == 0);	
}

const char string_view::operator[](size_t idx) const {
	return start_[idx];
}

size_t string_view::size() const {
	return length_;
}

size_t string_view::length() const {
	return length_;
}

std::string string_view::tostring() {
	return std::string(start_, length_);
}


bool string_view::starts_with(const char* str) const {
	size_t len = strlen(str);
	return (len <= length_) && 
				 (strncmp(start_, str, len)==0);
}

bool string_view::starts_with(const string_view& other) const {
	return (other.length_ <= length_) &&
         (strncmp(start_, other.start_, other.length_)==0);
}

const char* string_view::data() const {
	return start_;
}

}
}
