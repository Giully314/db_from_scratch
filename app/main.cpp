import std;

import scratchdb;

using namespace scratchdb;

// TODO: left and right strip of white spaces.



// ********************* PAGE AND TABLE ********************


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