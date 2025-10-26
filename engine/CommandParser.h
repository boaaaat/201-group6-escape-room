#pragma once
#include <string>
#include <vector>

// Very simple command tokenizer / interpreter helper.
// Game will feed player input string here to split into verb + args.
class CommandParser {
public:
    // parse("move north") -> ["move","north"]
    static std::vector<std::string> tokenize(const std::string& line);
};
