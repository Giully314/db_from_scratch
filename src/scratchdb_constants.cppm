
export module scratchdb.constants;

import scratchdb.types;
import scratchdb.row;

export namespace scratchdb {

// **** Row ***
constexpr auto id_size = sizeof(Row::id);
constexpr auto username_size = sizeof(Row::username);
constexpr auto email_size = sizeof(Row::email);
constexpr u32 id_offset = 0;
constexpr u32 username_offset = id_offset + username_size;
constexpr u32 email_offset = username_offset + email_size;
[[maybe_unused]] constexpr u32 row_size = id_size + username_size + email_size;


// **** Table ***

// 4KB page, this should be aligned with the page size of the system.
[[maybe_unused]] constexpr u32 page_size = 4096;
constexpr u32 table_max_pages = 100;
[[maybe_unused]] constexpr u32 rows_per_page = page_size / row_size;
[[maybe_unused]] constexpr u32 table_max_rows = rows_per_page * table_max_pages;


} // namespace scratchdb

