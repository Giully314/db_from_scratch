// PURPOSE: Define aliases for width types.

export module scratchdb.types;

import std;

export namespace scratchdb {

using u8 = std::uint8_t;
using u6 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8 = std::int8_t;
using i6 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;


// Used with std::expected to point a void error.
struct VoidError {};

} // namespace scratchdb
