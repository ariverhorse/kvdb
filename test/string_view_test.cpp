#include "include/string_view.h"
#include "gtest/gtest.h"

#include <iostream>

namespace kvdb {
namespace util {

TEST(Stringview_test, normal) {
  std::string str("This is a test");
  std::string str1(" is a test");
  Stringview sv(str); 
  sv.RemovePrefix(4);
	std::string a = sv.ToString();
  ASSERT_EQ(a,str1);
}

TEST(Stringview_test, Empty) {
	char a[13]="peng yuantao";
  Stringview sv(a, 12);
  sv.RemovePrefix(12);
  ASSERT_EQ(sv.Empty(), true);
}

TEST(Stringview_test, startswithreturntrue) {
	std::string str("This is my first kvdb impl");
	Stringview sv(str);
	ASSERT_EQ(true, sv.StartsWith("This")); 
}

TEST(Stringview_test, startswithreturnfalse) {
	std::string str("This is my first kvdb impl");
	Stringview sv(str);
	ASSERT_EQ(false, sv.StartsWith("this")); 
}

TEST(Stringview_test, operatoroverload) {
	std::string str("abcdefghijk");
	Stringview sv(str);
	ASSERT_EQ('d', sv[3]); 
}

TEST(Stringview_test, startswithreturntrueforsv) {
	std::string str("This is my first kvdb impl");
	Stringview sv(str);
	std::string str1("This");
	Stringview sv1(str1);
	ASSERT_EQ(true, sv.StartsWith(sv1)); 
}

}
}
