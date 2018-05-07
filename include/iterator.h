#ifndef _KVDB_UTIL_ITERATOR_
#define _KVDB_UTIL_ITERATOR_

#include <string>
#include "include/string_view.h"

namespace kvdb {
namespace table {

//Iterator interface
class Iterator {
 public:
	Iterator() = default;
	virtual ~Iterator() = default;

	Iterator(const Iterator&) = delete;
	Iterator& operator=(const Iterator&) = delete;	
	
	//Return true iff the iterator is positioned at
	//valid key value pair, False otherwise
	virtual bool Valid() const = 0;

	//Return Key if Valid()==True
	virtual util::Stringview Key() const = 0;

	//Return Value if Valid() == True
	virtual util::Stringview Value() const = 0;

	//Position the iterator to the first valid Key
	//Value pair,  Valid() is true after this call
  //If the input is not empty.
	virtual void SeekToFirst() = 0;

	//Position the iterator to the last valid Key
	//Value pair,  Valid() is true after this call
  //If the input is not empty.
	virtual void SeekToLast() = 0;

	//Position the iterator at a position >= target
	virtual void Seek(const util::Stringview& target) = 0;

	//Move the iterator to the next Key Value Pair
	virtual void Next() = 0;

	//Move the iterator to the Previous Key Value Pair
	virtual void Prev() = 0;
};

} //namespace table 
} //namespace kvdb


#endif
