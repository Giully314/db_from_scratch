// PURPOSE: Orchestrate the db.
//
// CLASSES:
// 
// DESCRIPTION:
// 

export module scratchdb.vm;

import std;

import scratchdb.types;
import scratchdb.table;
import scratchdb.row;
import scratchdb.statements;
import scratchdb.commands;
import scratchdb.serialize;
import scratchdb.errors;

namespace scratchdb {

export class VM {
public:

    // Main loop of the execution of the db.
    auto run() -> void {
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
    }

private:
    auto execute_statement(Statement& stm) -> void {
        std::visit(StatementExecutor{table}, stm);
    }

private:
    Table table;
};
    
} // namespace scratchdb
