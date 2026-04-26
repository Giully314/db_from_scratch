// PURPOSE:
//
// CLASSES:
// 
// DESCRIPTION:
// 

export module scratchdb.statements;

import std;

import scratchdb.row;
import scratchdb.errors;
import scratchdb.types;
import scratchdb.table;
import scratchdb.serialize;

namespace scratchdb {


export struct SelectStatement {

};

export struct InsertStatement {
    Row row;
};

export using Statement = std::variant<SelectStatement, InsertStatement>;



export auto prepare_statement(std::string_view cmd) -> std::expected<Statement, InputError> {
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




export struct StatementExecutor {  
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
    

} // namespace scratchdb
