// PURPOSE: Manage disk pages.
//
// CLASSES: 
//  Pager: manage pages of memory from disk to ram.
// 
// DESCRIPTION:
//  This module provides the "driver" which handles the memory pages between
//  disk and memory, serving as a cache system.
//  

export module scratchdb.pager;

import std;

import scratchdb.types;
import scratchdb.constants;

namespace scratchdb {

export
class Pager {
public:
    Pager(const std::filesystem::path& filename_) : filename{filename_},
        filestream{filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::app}, 
        pages(table_max_rows) {
        if (!filestream) {
            throw std::runtime_error{"Error opening the file"};
        }
    }

    /// @brief Get a page of memory.
    /// @param page_num 
    /// @exception std
    /// @return 
    auto page(const u32 page_num) -> non_owned_ptr<std::byte> {

    }

private:
    std::filesystem::path filename;
    std::fstream filestream;
    std::vector<std::unique_ptr<std::byte[]>> pages;
};
    
} // namespace scratchdb
