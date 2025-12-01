#pragma once
#include <vector>
#include <string>
#include "CraftRecipe.h"
#include "Inventory.h"

class CraftingSystem {
public:
    void addRecipe(const CraftRecipe& r);
    bool craft(Inventory& inv, const std::string& resultName);
    bool craft(Inventory& inv, size_t recipeIndex);
    bool uncraft(Inventory& inv);
    void listRecipes() const;
    bool knowsRecipe(const std::string& resultName) const;
    size_t recipeCount() const { return recipes.size(); }

private:
    std::vector<CraftRecipe> recipes;
    CraftRecipe lastCraftedRecipe;
    bool hasLastCraft = false;
};
