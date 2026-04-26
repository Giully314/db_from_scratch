import std;

import scratchdb.types;

using namespace scratchdb;

// TODO: left and right strip of white spaces.


// ********************* ROW ********************



constexpr auto column_username_size = 32;
constexpr auto column_email_size = 255;


struct Row final {
    Row(const u32 id_, const std::array<char, column_username_size> username_, const std::array<char, column_email_size> email_) : 
        id(id_), username{username_}, email{email_} {}

    Row(const u32 id_, const std::string_view username_, const std::string_view email_) : id(id_) {
        // NOTE: std::string is not null terminated, but in this case, because the arrays are initialized with \0,
        // we don't have any problem later when using them as strings.
        std::ranges::copy(username_, username.begin());
        std::ranges::copy(email_, email.begin());
    }

    u32 id;
    std::array<char, column_username_size> username;
    std::array<char, column_email_size> email;
};


template<>
struct std::formatter<::Row> final : std::formatter<std::string_view> {
    auto format(const ::Row& r, std::format_context& ctx) const {
        // Only works with null terminated strings username and email.
        return std::formatter<std::string_view>::format(std::format("Row: {} {} {}", r.id, std::string{r.username.data()}, std::string{r.email.data()}), ctx);
    }
};


constexpr auto id_size = sizeof(Row::id);
constexpr auto username_size = sizeof(Row::username);
constexpr auto email_size = sizeof(Row::email);
constexpr u32 id_offset = 0;
constexpr u32 username_offset = id_offset + username_size;
constexpr u32 email_offset = username_offset + email_size;
[[maybe_unused]] constexpr u32 row_size = id_size + username_size + email_size;


// We can also accept std::span<std::byte> to enforce correct size.
// Precondition: 
auto serialize(const Row& src, std::byte *const dest) -> void {
    std::memcpy(dest + id_offset, &src.id, id_size);
    std::memcpy(dest + username_offset, src.username.data(), username_size);
    std::memcpy(dest + email_offset, src.email.data() , email_size);
}

auto deserialize(const std::byte *const source) -> Row {
    u32 id;
    std::array<char, column_username_size> username;
    std::array<char, column_email_size> email;

    std::memcpy(&id, source + id_offset, id_size);
    std::memcpy(username.data(), source + username_offset, username_size);
    std::memcpy(email.data(), source + email_offset, email_size);    

    return Row{id, username, email};
}



struct TerminateProgram : std::runtime_error {
    template <typename StringLike>
    requires std::convertible_to<StringLike, std::string_view>
    TerminateProgram(StringLike&& msg) : std::runtime_error{std::forward<StringLike>(msg)} {}
};


// ********************* PAGE AND TABLE ********************

// 4KB page, this should be aligned with the page size of the system.
[[maybe_unused]] constexpr u32 page_size = 4096;
constexpr u32 table_max_pages = 100;
[[maybe_unused]] constexpr u32 rows_per_page = page_size / row_size;
[[maybe_unused]] constexpr u32 table_max_rows = rows_per_page * table_max_pages;

// NOTE: at this moment, the Row is fixed, which means that every instance has the same size and 
// the same fields.
struct Table {
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
        const u32 byte_offset = row_offset % row_size;
        return page.get() + byte_offset;
    }
    // Preallocate for now a fixed number of pages. We could use std::array but in future
    // we could support dynamic number of pages.
    std::vector<std::unique_ptr<std::byte[]>> pages;
    u32 num_rows;
};


// ********************* META COMMAND ********************

enum class MetaCommandResult : u8 {
    Success,
    UnrecognizedCommand
};

auto execute_meta_command(std::string_view cmd) -> MetaCommandResult {
    if (cmd == ".exit") {
        throw TerminateProgram{"Quitting."};
    } else {
        std::println("Unrecognized command '{}'.", cmd);
        return MetaCommandResult::UnrecognizedCommand;
    }
}

// ********************* STATEMENT ********************

enum class InputError : u8 {
    ParseStatement,
    UnrecognizedStatement,
};


struct SelectStatement {

};

struct InsertStatement {
    Row row;
};

using Statement = std::variant<SelectStatement, InsertStatement>;


auto prepare_statement(std::string_view cmd) -> std::expected<Statement, InputError> {
    std::stringstream iss{cmd};
    if (cmd.starts_with("select")) {
        return std::expected<Statement, InputError>{std::in_place, SelectStatement{}};
    } else if (cmd.starts_with("insert")) {
        u32 id{};
        std::string _, username, email;
        iss >> _ >> id >> username >> email;
        if (iss.fail() || !(iss >> std::ws).eof()) {
            return std::unexpected<InputError>{std::in_place, InputError::ParseStatement};
        }
        return std::expected<Statement, InputError>{std::in_place, InsertStatement{Row{id, username, email}}};
    }

    return std::unexpected<InputError>{std::in_place, InputError::UnrecognizedStatement}; 
}


struct StatementExecutor {  
    auto operator()([[maybe_unused]] const SelectStatement& stm) const -> void {
        std::println("Select statement");

        for (u32 i = 0; i < table.num_rows; ++i) {
            auto r = deserialize(table[i]);
            std::println("{}", r);
        }
    }
    
    auto operator()([[maybe_unused]] const InsertStatement& stm) const -> void {
        std::println("Insert statement with row {}", stm.row);
        
        if (!table.can_insert()) {
            // Signal no memory.
        }

        const auto p = table[table.num_rows];
        serialize(stm.row, p);  
        ++table.num_rows;
    }


    Table& table;
};

Table table;

auto execute_statement(Statement& stm) -> void {
    std::visit(StatementExecutor{table}, stm);
}


auto dispatch_input_error(const InputError e) -> void {
    switch (e) {
    using enum InputError;
    
    case ParseStatement:
        std::println("Error while parsing the statement");
        break;
    case UnrecognizedStatement:
        std::println("Unrecognized statement");
        break;
    }
}


// ********************* MAIN ********************


auto main() -> int {
    std::println("Hello world {}", sizeof(Row::username));

    Row r{23, "pippo", "pippo@example.com"};

    std::println("{}", r);

    std::string input;
    try {
        while (true) {
            std::print("db > ");
            std::getline(std::cin, input);
    
            // Dispatch between statements and meta commands.
    
            if (input.starts_with(".")) {
                execute_meta_command(input);
            } else {
                
                auto statement = prepare_statement(input);
                if (statement) {
                    execute_statement(*statement);
                } else {
                    dispatch_input_error(statement.error());
                }
            }
        }
    } catch (const TerminateProgram& e) {
        std::println("{}", e.what());
    }

    return 0;
}