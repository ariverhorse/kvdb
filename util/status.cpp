#include "include/status.h"

namespace kvdb {
namespace util {


Status::Status() : code_(Code::kOk) { }

Status::Status(Code code, const Stringview& msg1, const Stringview& msg2) {
	code_ = code;
	msg1_ = msg1.ToString();
	msg2_ = msg2.ToString();
} 

Status::Status(const Status& s) {
	code_ = s.code_;
	msg1_ = s.msg1_;
	msg2_ = s.msg2_;
}

Status& Status::operator=(const Status& s) {
	if(this != &s) {
		code_ = s.code_;
		msg1_ = s.msg1_;
		msg2_ = s.msg2_;
	}
	return *this;
}

Status Status::OK() {
	return Status();
}

Status Status::NotFound(const Stringview& msg1, const Stringview& msg2) {
	return Status(Code::kNotFound, msg1, msg2);
}

Status Status::Corruption(const Stringview& msg1, const Stringview& msg2) {
	return Status(Code::kCorruption, msg1, msg2);
}

Status Status::NotSupported(const Stringview& msg1, const Stringview& msg2) {
	return Status(Code::kNotSupported, msg1, msg2);
}
  
Status Status::InvalidArgument(const Stringview& msg1, const Stringview& msg2) {
	return Status(Code::kInvalidArgument, msg1, msg2);
}

Status Status::IOError(const Stringview& msg1, const Stringview& msg2) {
	return Status(Code::kIOError, msg1, msg2);
}

bool Status::Ok() const {
	return code_ == Code::kOk;
}

bool Status::IsNotFound() const {
	return code_ == Code::kNotFound;
}

bool Status::IsCorruption() const {
	return code_ == Code::kCorruption;
}

bool Status::IsIOError() const {
	return code_ == Code::kIOError;
}

bool Status::IsNotSupported() const {
	return code_ == Code::kNotSupported;
}

bool Status::IsInvalidArgument() const {
	return code_ == Code::kInvalidArgument;
}

std::string Status::ToString() const {
	std::string code;
	switch(code_) {
		case Code::kOk:
			code = "OK";
			break;
		case Code::kNotFound:
			code = "NotFound";
			break;
		case Code::kCorruption:
			code = "Corruption";
			break;
		case Code::kIOError:
			code = "IOError";
			break;
		case Code::kNotSupported:
			code = "NotSupported";
			break;
		case Code::kInvalidArgument:
			code = "InvalidArgument";
			break;
		default:
			code = "Unknown code";
	}	
	std::string result = code;
	if(msg1_.length()>0) {
		result+=": "+msg1_;
		if(msg2_.length() > 0) {
			result +=": "+msg2_;
		}
	}
	return result;
}

} // namespace util
} // namespace kvdb
