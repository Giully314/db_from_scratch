
export module scratchdb.serialize;

import std;

import scratchdb.types;
import scratchdb.row;
import scratchdb.constants;

namespace scratchdb {

// We can also accept std::span<std::byte> to enforce correct size.
// Precondition: dest must point to a memory region large enough.
export auto serialize(const Row& src, std::byte *const dest) -> void {
    std::memcpy(dest + id_offset, &src.id, id_size);
    std::memcpy(dest + username_offset, src.username.data(), username_size);
    std::memcpy(dest + email_offset, src.email.data() , email_size);
}

export auto deserialize(const std::byte *const source) -> Row {
    u32 id;
    std::array<char, column_username_size> username;
    std::array<char, column_email_size> email;

    std::memcpy(&id, source + id_offset, id_size);
    std::memcpy(username.data(), source + username_offset, username_size);
    std::memcpy(email.data(), source + email_offset, email_size);    

    return Row{id, username, email};
}
    
} // namespace scratchdb
