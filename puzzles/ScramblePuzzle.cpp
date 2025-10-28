#include "ScramblePuzzle.h"
#include <iostream>
#include <string>

ScramblePuzzle::ScramblePuzzle(std::vector<std::pair<std::string,std::string>> wordPairs)
    : pairs(std::move(wordPairs)) {}

bool ScramblePuzzle::play() 
{
    std::cout << "[BASKET] \"Prove you can match what is scrambled!\"\n";

    for (auto& p : pairs) {
        std::cout << "Unscramble this word: " << p.first << "\n> ";
        std::string guess;
        std::getline(std::cin, guess);

        if (guess != p.second) {
            std::cout << "[BASKET] \"No match! Tragic. Try again once you're less useless.\"\n";
            return false;
        }
    }

    std::cout << "[BASKET] \"Impressive. Balance returns to laundry.\"\n";
    return true;
}
