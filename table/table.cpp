#include "include/table.h"
#include "include/block.h"
#include "include/varint.h"
#include "include/string_view.h"
#include <utility>

namespace kvdb {
namespace table {

class TableImpl {
	public:
		Block index_block_;	
};


std::string ReadBlock(const BlockHandle& handle, std::unique_ptr<util::File> file) {


} 

std::unique_ptr<Table> Table::Open(std::unique_ptr<util::File> file, uint32_t filesize) {
  auto impl = std::make_unique<TableImpl>();
	Footer footer;
	std::string footer_buf;
	file->Read(filesize-Footer::kFooterSize, Footer::kFooterSize, footer_buf);
	util::Stringview sv(footer_buf);
	footer.DecodeFrom(sv);
	BlockHandle index_handle = footer.IndexHandle();
	//TODO(pengyuantao) Fill up the impl with index block content
	auto table = std::make_unique<Table>(std::move(impl));			
	return table;
}


Table::Iterator::Iterator(Block::Iterator index_iter_begin, Block::Iterator index_iter_end) : index_iter_begin_(index_iter_begin), index_iter_end_(index_iter_end) {
}

std::pair<util::Stringview, util::Stringview> Table::Iterator::operator*() {
	return *cur_block_iter_;	
}

Table::Iterator& Table::Iterator::operator++() {
	++cur_block_iter_;
	if(cur_block_iter_ == cur_block_iter_end_) {
		//Move index iter and reinitialze all the block iter
		++cur_index_iter_;
		if(cur_index_iter_ != index_iter_end_) {
			handle = *cur_index_iter;
			std::string buf = ReadBlock(handle, file);
			Block blk(buf);
			cur_block_iter_start_ = blk.Begin();
			cur_block_iter_end_ = blk.End();
			cur_block_iter_ = blk.Begin();
		}
	}
}

bool Table::Iterator::operator==(const Iterator& other) {


}

bool Table::Iterator::operator!=(const Iterator& other) {


}

Table::Iterator Table::Begin() {
	return Table::Iterator(idx_begin);
}

Table::Iterator Table::End() {
	return Table::Iterator(idx_end);
}


} //table
} //kvdb
