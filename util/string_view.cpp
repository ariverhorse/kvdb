#include "include/string_view.h"
#include <cstring>
#include <cassert>

namespace kvdb {
namespace util {

Stringview::Stringview()
  : start_(nullptr), length_(0) {}

Stringview::Stringview(const char* s, size_t len)
  : start_(s), length_(len) {}

Stringview::Stringview(const char* s)
	: start_(s), length_(strlen(s)) {} 

Stringview::Stringview(const std::string& str)
  : start_(str.c_str()), length_(str.length()) {}

Stringview::Stringview(const Stringview& other) {
  start_ = other.start_;
	length_ = other.length_;
}

Stringview& Stringview::operator=(const Stringview& other) {
	if(this != &other) { 
		start_ = other.start_;
		length_ = other.length_;
 	} 
	return *this;	
}

void Stringview::RemovePrefix(size_t n) {
	assert(n<=length_);
	start_ += n;
  length_ -= n;
}

bool Stringview::Empty() const {
	return (length_ == 0);	
}

const char Stringview::operator[](size_t idx) const {
	return start_[idx];
}

size_t Stringview::Size() const {
	return length_;
}

size_t Stringview::Length() const {
	return length_;
}

std::string Stringview::ToString() const {
	return std::string(start_, length_);
}


bool Stringview::StartsWith(const char* str) const {
	size_t len = strlen(str);
	return (len <= length_) && 
				 (strncmp(start_, str, len)==0);
}

bool Stringview::StartsWith(const Stringview& other) const {
	return (other.length_ <= length_) &&
         (strncmp(start_, other.start_, other.length_)==0);
}

const char* Stringview::Data() const {
	return start_;
}

bool operator==(const Stringview& sv1, const Stringview& sv2) {
	return (sv1.Size() == sv2.Size()) && 
				 (strncmp(sv1.Data(), sv2.Data(), sv1.Size())==0);
}

bool operator==(const Stringview& sv1, const std::string& buf) {
	return (sv1.Size() == buf.length()) && 
				 (strncmp(sv1.Data(), buf.c_str(), sv1.Size())==0);
}


} //namespace util
} //namespace kvdb
