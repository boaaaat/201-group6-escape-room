#pragma once
#include <string>

class Puzzle {
public:
    virtual ~Puzzle() {}
    virtual bool play() = 0;
    virtual std::string getName() const = 0;
};
