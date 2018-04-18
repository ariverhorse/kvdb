#ifndef _KVDB_UTIL_FILE_
#define _KVDB_UTIL_FILE_

#include "include/string_view.h"
#include "include/status.h"

#include <string>
#include <memory>

namespace kvdb {
namespace util {

class File {
 public:
	enum class Options {
		ReadOnly,
		WriteOnly,
		Append,
		ReadWrite
	};

	static std::unique_ptr<File> NewFile(const std::string& name, const File::Options& option); 

	virtual Status Read(int offset, int n, std::string& buf) = 0;

	virtual Status Append(std::string& buf) = 0;
	
	virtual Status Flush() = 0;
};


} //namespace util
} //namespace kvdb

#endif
