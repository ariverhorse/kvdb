#include "string_view.h"
#include "gtest/gtest.h"

#include <iostream>

TEST(string_view_test, normal) {
  std::string str("This is a test");
  std::string str1(" is a test");
  kvdb::util::string_view sv(str); 
  sv.remove_prefix(4);
	std::string a = sv.tostring();
  ASSERT_EQ(a,str1);
}

