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

TEST(string_view_test, empty) {
	char a[13]="peng yuantao";
  kvdb::util::string_view sv(a, 12);
  sv.remove_prefix(12);
  ASSERT_EQ(sv.empty(), true);
}
