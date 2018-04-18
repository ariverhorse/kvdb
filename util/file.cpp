#include "include/file.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

namespace kvdb {
namespace util {

namespace {

Status PosixError(std::string& msg, int error) {
	if(error == ENOENT) {
		return Status::NotFound(msg, strerror(error));
	} else {
		return Status::IOError(msg, strerror(error));
	}
}

class PosixFile : public File{
 public:
	PosixFile(const std::string& name, const File::Options& option) : name_(name) {
			int flag;
			switch(option) {
				case File::Options::ReadOnly: 
					flag = O_RDONLY;
					break;
				case File::Options::WriteOnly:
					flag = O_WRONLY | O_TRUNC | O_CREAT;
					break;
				case File::Options::ReadWrite:
					flag = O_RDWR | O_TRUNC | O_CREAT;
					break;
				case File::Options::Append:
					flag = O_APPEND;
					break;
				default:
					flag = O_RDONLY;
			}
			fd_ = ::open(name.c_str(), flag);
	}

	~PosixFile() {
		::close(fd_);
	}

	Status Read(int offset, int n, std::string& buf) override{
		lseek(fd_, offset, SEEK_SET);
		char* local_buf = new char[n];
		ssize_t count = ::read(fd_, local_buf, n);		
		buf.append(local_buf, count);
		delete [] local_buf;
		return Status::OK();
	}

	Status Append(std::string& buf) override {
		lseek(fd_, 0, SEEK_END);
		const char* p = buf.c_str();
		int n = buf.length();
		while(n > 0) {
			ssize_t c = ::write(fd_, p, n);
			if(c < 0) {
				if(errno == EINTR) {
					continue;
				} else {
					return PosixError(name_, errno);
				}
			} else {
				p+=c;
				n-=c;
			}
		}
		return Status::OK();
	}

	Status Flush() override {
		::fsync(fd_);
		return Status::OK();
	}
	
	private:
		std::string name_;
		int fd_;
};


} //anonymous namespace


std::unique_ptr<File> File::NewFile(const std::string& filename, const File::Options& option) {
	std::unique_ptr<File> file = std::make_unique<PosixFile>(filename, option);
	return file; 
}



} //namespace util
} //namespace kvdb

