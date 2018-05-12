#ifndef _KVDB_UTIL_OPTION_
#define _KVDB_UTIL_OPTION_


namespace kvdb {

struct Option {
	//Num of entries between two restart point in block
	int block_restart_interval;
};


}//namespace kvdb

#endif
