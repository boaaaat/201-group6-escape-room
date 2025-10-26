#include "ScramblePuzzle.h"
#include <iostream>

// ctor:
//  - store wordPairs like {{"KCSO","SOCK"}, ...}
ScramblePuzzle::ScramblePuzzle(std::vector<std::pair<std::string,std::string>> wordPairs)
    : pairs(wordPairs) {}

// play():
//  - For each pair:
//      - print scrambled
//      - read player guess
//      - if any guess wrong, print fail text and return false
//  - if all correct, print success text and return true
bool ScramblePuzzle::play() {
    // TODO
    return false;
}
