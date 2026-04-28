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

export constexpr u32 column_username_size = 32;
export constexpr u32 column_email_size = 256;


export struct Row final {
    Row(const u32 id_, const std::array<char, column_username_size> username_, const std::array<char, column_email_size> email_) : 
        id(id_), username{username_}, email{email_} {}

    Row(const u32 id_, const std::string_view username_, const std::string_view email_) : id(id_), username{}, email{} {
        // NOTE: std::string is not null terminated, but in this case, because the arrays are initialized with \0,
        // we don't have any problem later when using them as strings.
        // -1 is to keep the \0.
        std::ranges::copy_n(username_, std::min(username_.size(), column_username_size - 1), username.begin());
        std::ranges::copy_n(email_, std::min(email_.size(), column_email_size - 1), email.begin());
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