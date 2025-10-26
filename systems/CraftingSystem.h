#pragma once
#include <vector>
#include <string>
#include "CraftRecipe.h"
#include "Inventory.h"

class CraftingSystem {
public:
    // Add a new recipe to known list
    void addRecipe(const CraftRecipe& r);

    // Try to craft an item by name (e.g. "Matched Pair")
    // - Check if inventory has all ingredients
    // - Remove them if yes
    // - Add the resulting item
    // - Record craft history for "uncraft"
    bool craft(Inventory& inv, const std::string& resultName);

    // Undo last craft:
    // - Remove crafted result from inventory
    // - Add back original ingredients
    // - Only 1-level undo (stack of last craft)
    bool uncraft(Inventory& inv);

    // Show all known recipes:
    // - For each recipe, print ingredients -> result
    // - Also print what the result does
    void listRecipes() const;

    // used by hint system or UI
    bool knowsRecipe(const std::string& resultName) const;

private:
    std::vector<CraftRecipe> recipes;

    // store last craft attempt for undo
    CraftRecipe lastCraftedRecipe;
    bool hasLastCraft = false;
};
