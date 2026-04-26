

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
    
} // namespace scratchdb
