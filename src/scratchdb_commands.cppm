// PURPOSE:
//
// CLASSES:
// 
// DESCRIPTION:
// 

export module scratchdb.commands;

import std;

import scratchdb.types;
import scratchdb.errors;

namespace scratchdb {

export enum class MetaCommandResult : u8 {
    Success,
    UnrecognizedCommand
};

export auto execute_meta_command(std::string_view cmd) -> MetaCommandResult {
    if (cmd == ".exit") {
        throw TerminateProgram{"Quitting."};
    } else {
        std::println("Unrecognized command '{}'.", cmd);
        return MetaCommandResult::UnrecognizedCommand;
    }
}

} // namespace scratchdb
