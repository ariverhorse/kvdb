#include "include/block.h"
#include "include/varint.h"
#include <utility>

namespace kvdb {
namespace table {

namespace {
	class BlockIterator : public Iterator {
		public:
			BlockIterator(const Block& block) : buf_(block.content_.c_str()) {

			}

			bool Valid() const {
				return key_.Size()!=0;
			}

			Stringview Key() const {
				return key_;
			}

			Stringview Value() const {
				return value_;
			}

			void SeekToFirst() {
				
			}

			void Next() {

			}

			void Prev() {

			}

			void SeekToLast() {

			}

			void Seek(const Stringview& target) {


			}

		private:
			const char* buf_;
    	void _ParseNextEntry() {
    		GetLengthPrefixedString(sv_, key_);
    		GetLengthPrefixedString(sv_, value_);
    	}	

	};

	/*
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
    }; */


}

void BlockBuilder::Add(const util::Stringview& key,
								const util::Stringview& value) {
	PutLengthPrefixedString(content_, key);
	PutLengthPrefixedString(content_, value);
}

util::Stringview BlockBuilder::Finish() {
	return util::Stringview(content_);
}




} //table
} //kvdb
