

export module scratchdb.errors;

import std;

import scratchdb.types;

export namespace scratchdb {

struct TerminateProgram : std::runtime_error {
    template <typename StringLike>
    requires std::convertible_to<StringLike, std::string_view>
    TerminateProgram(StringLike&& msg) : std::runtime_error{std::forward<StringLike>(msg)} {}
};



enum class InputError : u8 {
    ParseStatement,
    UnrecognizedStatement,
};
    
} // namespace scratchdb
