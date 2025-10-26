#include "CraftingSystem.h"
#include <iostream>

// addRecipe:
//  - push_back into recipes
void CraftingSystem::addRecipe(const CraftRecipe& r) {
    // TODO
}

// craft:
//  - find recipe with resultName
//  - verify all ingredients exist in inv
//  - remove ingredients from inv
//  - create Item(resultName, resultDesc, true, resultTip) and add to inv
//  - store lastCraftedRecipe = that recipe, hasLastCraft = true
//  - print success text
//  - return true on success
bool CraftingSystem::craft(Inventory& inv, const std::string& resultName) {
    // TODO
    return false;
}

// uncraft:
//  - if no hasLastCraft, explain you can't undo
//  - remove the crafted item from inventory
//  - give ingredients back
//  - print text about "you carefully separate the objects back out"
//  - set hasLastCraft = false
bool CraftingSystem::uncraft(Inventory& inv) {
    // TODO
    return false;
}

// listRecipes:
//  - loop recipes and print:
//    "<ResultName>: <ingredients joined with +> -> does <resultTip>"
void CraftingSystem::listRecipes() const {
    // TODO
}

bool CraftingSystem::knowsRecipe(const std::string& resultName) const {
    // TODO: check in recipes vector
    return false;
}
