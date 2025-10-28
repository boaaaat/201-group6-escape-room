#include "CraftingSystem.h"
#include <iostream>
#include <Helpers.h>

void CraftingSystem::addRecipe(const CraftRecipe& r) {
    // Don't add duplicates with the same resultName
    for (const auto& existing : recipes) {
        if (existing.resultName == r.resultName) {
            return;
        }
    }
    recipes.push_back(r);
}

bool CraftingSystem::craft(Inventory& inv, const std::string& resultName) {
    const CraftRecipe* chosen = nullptr;
    for (const auto& r : recipes) {
        if (toLower(r.resultName) == resultName) {
            chosen = &r;
            break;
        }
    }
    if (!chosen) {
        std::cout << "You don't know how to craft that yet.\n";
        return false;
    }

    // check ingredients
    for (const auto& need : chosen->ingredients) {
        if (!inv.hasItem(need)) {
            std::cout << "Missing ingredient: " << need << "\n";
            return false;
        }
    }

    // consume ingredients
    for (const auto& need : chosen->ingredients) {
        inv.removeItem(need);
    }

    // give the crafted item
    inv.addItem(Item(
        chosen->resultName,
        chosen->resultDesc,
        true,
        chosen->resultTip
    ));

    // store undo info
    lastCraftedRecipe = *chosen;
    hasLastCraft = true;

    std::cout << "[Crafted] " << chosen->resultName << "!\n";
    return true;
}

bool CraftingSystem::uncraft(Inventory& inv) {
    if (!hasLastCraft) {
        std::cout << "There's nothing recent to uncraft.\n";
        return false;
    }

    // remove crafted result
    if (!inv.removeItem(lastCraftedRecipe.resultName)) {
        std::cout << "You don't even have " << lastCraftedRecipe.resultName
                  << " anymore, so you can't uncraft it.\n";
        hasLastCraft = false;
        return false;
    }

    // restore ingredients
    for (const auto& need : lastCraftedRecipe.ingredients) {
        inv.addItem(Item(
            need,
            "(restored component)",
            true,
            "This was part of something you crafted."
        ));
    }

    std::cout << "[Uncrafted] You carefully separate "
              << lastCraftedRecipe.resultName << " back into pieces.\n";

    hasLastCraft = false;
    return true;
}

void CraftingSystem::listRecipes() const {
    if (recipes.empty()) {
        std::cout << "You don't know any crafting recipes yet.\n";
        return;
    }

    std::cout << "=== Known Crafting Recipes ===\n";
    for (const auto& r : recipes) {
        std::cout << r.resultName << " = ";
        for (size_t i = 0; i < r.ingredients.size(); ++i) {
            std::cout << r.ingredients[i];
            if (i + 1 < r.ingredients.size()) std::cout << " + ";
        }
        std::cout << "\n";
        std::cout << "   " << r.resultDesc << "\n";
        std::cout << "   Tip: " << r.resultTip << "\n";
    }
}

bool CraftingSystem::knowsRecipe(const std::string& resultName) const {
    for (const auto& r : recipes) {
        if (r.resultName == resultName) return true;
    }
    return false;
}