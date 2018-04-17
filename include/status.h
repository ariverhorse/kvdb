#ifndef _KVDB_UTIL_STATUS_
#define _KVDB_UTIL_STATUS_

#include "include/string_view.h"
#include <string>

namespace kvdb {
namespace util {

class Status {
 public:
	Status();
	~Status() = default;

	Status(const Status& s);
  Status& operator=(const Status& s);

  static Status NotFound(const Stringview& msg1, const Stringview& msg2 = Stringview());

  static Status Corruption(const Stringview& msg1, const Stringview& msg2 = Stringview());

  static Status NotSupported(const Stringview& msg1, const Stringview& msg2 = Stringview());
  
	static Status InvalidArgument(const Stringview& msg1, const Stringview& msg2 = Stringview());

	static Status IOError(const Stringview& msg1, const Stringview& msg2 = Stringview());

	bool Ok() const;

	bool IsNotFound() const;

	bool IsCorruption() const;

	bool IsIOError() const;

	bool IsNotSupported() const;

	bool IsInvalidArgument() const;

	std::string ToString() const;

 private:
  enum class Code {
		kOk = 0,
		kNotFound = 1,
		kCorruption = 2,
		kNotSupported = 3,
	  kInvalidArgument = 4,
		kIOError = 5
	};
	Code code_;
	std::string msg1_;
	std::string msg2_;
	Status(Code code, const Stringview& msg1, const Stringview& msg2);
};



}
}


#endif
