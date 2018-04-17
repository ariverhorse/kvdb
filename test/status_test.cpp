#include "include/status.h"
#include "gtest/gtest.h"

#include <iostream>

namespace kvdb {
namespace util{


TEST(StatusTest, Normal) {

	Stringview msg1("Failed to find");

	Stringview msg2("Please contact SRE");

	Status status = Status::NotFound(msg1, msg2);

	std::string expected = "NotFound: Failed to find: Please contact SRE";

	ASSERT_EQ(expected, status.ToString());	
} 

TEST(StatusTest, NotMsg2) {

	Stringview msg1("Failed to find");

	Status status = Status::NotFound(msg1);

	std::string expected = "NotFound: Failed to find";

	ASSERT_EQ(expected, status.ToString());	
} 




} //namespace util
} //namespace kvdb

