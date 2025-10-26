#pragma once
#include <string>

class Puzzle {
public:
    virtual ~Puzzle() {}
    virtual bool play() = 0;          // run the puzzle interaction
    virtual std::string getName() const = 0;
};
