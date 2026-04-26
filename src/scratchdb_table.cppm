// PURPOSE:
//
// CLASSES:
// 
// DESCRIPTION:
// 
export module scratchdb.table;

import std;

import scratchdb.types;
import scratchdb.constants;

namespace scratchdb {

// NOTE: at this moment, the Row is fixed, which means that every instance has the same size and 
// the same fields.
export struct Table {
    explicit Table() : pages(table_max_rows), num_rows{0} {}

    auto can_insert() const noexcept -> bool {
        return num_rows >= table_max_rows;
    }

    auto operator[](const u32 row_num) -> non_owned_ptr<std::byte> {
        const u32 page_num = row_num / rows_per_page;
        auto& page = pages[page_num];
        if (!page) {
            pages[page_num] = std::make_unique<std::byte[]>(page_size);
        }
        const u32 row_offset = row_num % rows_per_page;
        const u32 byte_offset = row_offset * row_size;
        return page.get() + byte_offset;
    }
    // Preallocate for now a fixed number of pages. We could use std::array but in future
    // we could support dynamic number of pages.
    std::vector<std::unique_ptr<std::byte[]>> pages;
    u32 num_rows;
};

} // namespace scratchdb
