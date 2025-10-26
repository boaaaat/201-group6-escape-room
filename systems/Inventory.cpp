#include "Inventory.h"
#include <iostream>

// addItem:
//  - push item into vector
//  - print confirmation text
bool Inventory::addItem(const Item& item) {
    // TODO: implement
    return true;
}

// removeItem:
//  - find item by name
//  - erase from vector
//  - return true if removed, false if not found
bool Inventory::removeItem(const std::string& itemName) {
    // TODO: implement
    return false;
}

// hasItem:
//  - scan vector for name match
bool Inventory::hasItem(const std::string& itemName) const {
    // TODO: implement
    return false;
}

// listItems:
//  - print each item with short description
void Inventory::listItems() const {
    // TODO: implement
}

// describeItem:
//  - find item
//  - print longer help / tip text
bool Inventory::describeItem(const std::string& itemName) const {
    // TODO: implement
    return false;
}
