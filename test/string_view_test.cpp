#include "include/string_view.h"
#include "gtest/gtest.h"

#include <iostream>

namespace kvdb {

TEST(string_view_test, normal) {
  std::string str("This is a test");
  std::string str1(" is a test");
  util::string_view sv(str); 
  sv.remove_prefix(4);
	std::string a = sv.tostring();
  ASSERT_EQ(a,str1);
}

TEST(string_view_test, empty) {
	char a[13]="peng yuantao";
  util::string_view sv(a, 12);
  sv.remove_prefix(12);
  ASSERT_EQ(sv.empty(), true);
}

TEST(string_view_test, startswithreturntrue) {
	std::string str("This is my first kvdb impl");
	util::string_view sv(str);
	ASSERT_EQ(true, sv.starts_with("This")); 
}

TEST(string_view_test, startswithreturnfalse) {
	std::string str("This is my first kvdb impl");
	util::string_view sv(str);
	ASSERT_EQ(false, sv.starts_with("this")); 
}

TEST(string_view_test, operatoroverload) {
	std::string str("abcdefghijk");
	util::string_view sv(str);
	ASSERT_EQ('d', sv[3]); 
}

TEST(string_view_test, startswithreturntrueforsv) {
	std::string str("This is my first kvdb impl");
	util::string_view sv(str);
	std::string str1("This");
	util::string_view sv1(str1);
	ASSERT_EQ(true, sv.starts_with(sv1)); 
}

}
