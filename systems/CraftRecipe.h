#pragma once
#include <string>
#include <vector>

// A crafting recipe defines:
//  - which ingredients are consumed
//  - what item is produced
//  - explanation of what the crafted item does
struct CraftRecipe {
    std::vector<std::string> ingredients;
    std::string resultName;
    std::string resultDesc;
    std::string resultTip;
};
// Recipe for Matched Pair (combining L + R)
inline CraftRecipe matchedPair() {
    return CraftRecipe{
        {"Sock of Truth (L)", "Sock of Truth (R)"},
        "Matched Pair",
        "A perfectly matched pair of Socks of Truth. Can activate the Scramble Puzzle.",
        "Prove the socks match by completing the puzzle activation."
    };
}
