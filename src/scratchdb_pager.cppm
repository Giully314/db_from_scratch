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

namespace scratchdb {

export
class Pager {
public:
    Pager(const std::filesystem::path& filename_) : filename{filename_},
        filestream{filename} {
        if (!filestream) {
            throw std::runtime_error{"Error opening the file"};
        }
    }

private:
    std::filesystem::path filename;
    std::fstream filestream;
};
    
} // namespace scratchdb
