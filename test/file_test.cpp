#include "include/file.h"
#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <string>

namespace kvdb {
namespace util{


TEST(FileTest, Read) {
	const char* filename = "/tmp/test";
	std::ofstream file(filename);
	std::string str="abcdefgh";
	file << str;
	file.close();
	std::unique_ptr<File> myfile = File::NewFile(std::string(filename), File::Options::ReadOnly);
	std::string buf;
	myfile->Read(0, 8, buf);
	ASSERT_EQ(buf, str);	 
} 





} //namespace util
} //namespace kvdb

