#ifndef _KVDB_UTIL_BLOCK_
#define _KVDB_UTIL_BLOCK_

// The basic building block of sstable
// The format of my sstable is a simplified version compared with
// leveldb's sstable (will bring more advanced feature such as 
// prefix compression, bloomfilter, etc back in the future)
//
// Block 0
// Block 1
// Block 2
// ....
// Block N
// Block Index
// Footer(block handle of Index block, and magic number)
//
// Each block is Varint32 prefixed Key-Value pair
// We don't do prefix comrpession here, and no block compression

#include <string>
#include "include/string_view.h"
#include "include/varint.h"

namespace kvdb {
namespace table {

// A data block contains key value pair
class Block {
	public:
		Block() = default;
    virtual ~Block() = default;

		virtual void Add(const util::Stringview& key,
										 const util::Stringview& value);
		virtual void Finish();

		//class Iterator;
    class Iterator {
    	public:
    		Iterator(const Block& block) : sv_(block.content_), eof_(false) {
    			_ParseNextEntry();	
    		}
    	
    		Iterator() : sv_(""), eof_(true) {
    		}
    
    		Iterator(const Iterator&) = default;
    		Iterator& operator=(const Iterator&) = default;
    	
    		std::pair<util::Stringview, util::Stringview> operator*() {
    			return std::make_pair(key_, value_);
    		}
    
    		Iterator& operator++() {
    			if(sv_.Size()>0) {
    				_ParseNextEntry();
    			} else if (sv_.Size()==0) {
						eof_ = true;
					} 
    			return *this;
    		}
    
    		Iterator operator++(int) {
    			Iterator tmp = *this;
    			if(sv_.Size()>0) {
    				_ParseNextEntry();
    			}
    			return tmp; 
    		}
    		
    		bool operator==(const Iterator& other) {
    			return this->sv_.Data() == other.sv_.Data()&&
    						 this->sv_.Size() == other.sv_.Size() ||
								 this->sv_.Size()==0 && other.sv_.Size()==0 && 
								 this->eof_ == other.eof_;
    		}
    
    		bool operator!=(const Iterator& other) {
    			return !operator==(other);
    		}
    
    	private:
    		util::Stringview sv_;
    		util::Stringview key_;
    		util::Stringview value_;
				bool eof_;
    		void _ParseNextEntry() {
    			GetLengthPrefixedString(sv_, key_);
    			GetLengthPrefixedString(sv_, value_);
    		}	
    };

		Iterator Begin(); 
		Iterator End();

	private:
		std::string content_;	
};


}//namespace table
}//namespace kvdb

#endif
