#pragma once
#include <vector>
#include <string>
#include "CraftRecipe.h"
#include "Inventory.h"

class CraftingSystem {
public:
    std::string toLower(const std::string &input);
    void addRecipe(const CraftRecipe& r);
    bool craft(Inventory& inv, const std::string& resultName);
    bool uncraft(Inventory& inv);
    void listRecipes() const;
    bool knowsRecipe(const std::string& resultName) const;

private:
    std::vector<CraftRecipe> recipes;
    CraftRecipe lastCraftedRecipe;
    bool hasLastCraft = false;
};
