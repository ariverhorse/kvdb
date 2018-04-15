#include "include/string_view.h"

namespace kvdb {
namespace util {

void string_view::remove_prefix(size_t n) {
  size_t m = std::min(n, length_);
	start_ += m;
  length_ -= m;
}

std::string string_view::tostring() {
	return std::string(start_, length_);
}


}
}
