#pragma once
#include "Puzzle.h"
#include <vector>
#include <string>
#include <utility>

class ScramblePuzzle : public Puzzle {
public:
    ScramblePuzzle(std::vector<std::pair<std::string,std::string>> wordPairs);
    bool play() override;
    std::string getName() const override { return "Sock Scramble"; }

private:
    std::vector<std::pair<std::string,std::string>> pairs;
};
