#include "Inventory.h"
#include <iostream>

// addItem:
//  - push item into vector
//  - print confirmation text
bool Inventory::addItem(const Item& item) 
{
    items.push_back(item);
    return true;
}

// removeItem:
// - find item by name
// - erase from vector
// - return true if removed, false if not found
bool Inventory::removeItem(const std::string& itemName) 
{
    for(auto item = items.begin(); item != items.end(); item++)
    {
        if(item->getName() == itemName)
        {
            items.erase(item);
            return true;
        }
    }
    return false;
}

bool Inventory::hasItem(const std::string& itemName) const 
{
    for (const auto& i : items) 
    {
        if (i.getName() == itemName) return true;
    }
    return false;
}

// print each item with short description
void Inventory::listItems() const 
{
    if (items.empty()) 
    {
        std::cout << "[Inventory is empty]\n";
        return;
    }
    std::cout << "=== Your Inventory ===\n";
    for (const auto& i : items) 
    {
        std::cout << "- " << i.getName() << ": " << i.getDescription() << "\n";
    }
}


bool Inventory::describeItem(const std::string& itemName) const 
{
    for (const auto& i : items) 
    {
        if (i.getName() == itemName) 
        {
            std::cout << "=== " << i.getName() << " ===\n";
            std::cout << i.getDescription() << "\n";
            std::cout << "Tip: " << i.getTip() << "\n";
            return true;
        }
    }
    std::cout << "You don't have that item.\n";
    return false;
}
