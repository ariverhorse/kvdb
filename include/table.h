#ifndef _KVDB_UTIL_TABLE_
#define _KVDB_UTIL_TABLE_

#include "include/status.h"
#include "include/file.h"
#include "include/string_view.h"
#include <string>
#include <memory>

namespace kvdb {
namespace table {

class TableImpl;

class Table {
  public:
		static std::unqiue_ptr<Table> Open(std::unique_ptr<util::File> file, 
				uint32_t filesize);

		class Iterator {
		  public:
				Iterator(Block::Iterator index_iter_begin, Block::Iterator index_iter_end);
				Iterator(const Iterator&) = default;
				Iterator& operator=(const Iterator&) = delete;
				std::pair<util::Stringview, util::Stringview> operator*();
				Iterator& operator++();
				Iterator operator++(int);
				bool operator==(const Iterator& other);
				bool operator!=(const Iterator& other);
			private:
				const Table& table_;
				Block::Iterator index_iter_begin_;
				Block::Iterator index_iter_end_;
				Block::Iterator cur_index_iter_;
				Block::Iterator cur_block_iter_start_;
				Block::Iterator cur_block_iter_end_;
				Block::Iterator cur_block_iter_;	
				File* file;
		};

		Iterator Begin();
		Iterator End();
	private:
		Table(std::unique_ptr<TableImpl> impl);
		std::unique_ptr<TableImpl> impl_;
};

}
}





#endif
