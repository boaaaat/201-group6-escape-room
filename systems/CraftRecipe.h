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
