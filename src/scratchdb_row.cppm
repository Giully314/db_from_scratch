// PURPOSE:
//
// CLASSES:
// 
// DESCRIPTION:
// 

export module scratchdb.row;

import std;

import scratchdb.types;

namespace scratchdb {

export constexpr auto column_username_size = 32;
export constexpr auto column_email_size = 255;

export struct Row final {
    Row(const u32 id_, const std::array<char, column_username_size> username_, const std::array<char, column_email_size> email_) : 
        id(id_), username{username_}, email{email_} {}

    Row(const u32 id_, const std::string_view username_, const std::string_view email_) : id(id_), username{}, email{} {
        // NOTE: std::string is not null terminated, but in this case, because the arrays are initialized with \0,
        // we don't have any problem later when using them as strings.
        std::ranges::copy(username_, username.begin());
        std::ranges::copy(email_, email.begin());
    }

    u32 id;
    std::array<char, column_username_size> username;
    std::array<char, column_email_size> email;
};


} // namespace scratchdb

export 
template<>
struct std::formatter<::scratchdb::Row> final : std::formatter<std::string_view> {
    auto format(const ::scratchdb::Row& r, std::format_context& ctx) const {
        // Only works with null terminated strings username and email.
        return std::formatter<std::string_view>::format(std::format("Row: {} {} {}", r.id, std::string{r.username.data()}, std::string{r.email.data()}), ctx);
    }
};