#pragma once
#include <vector>
#include <string>
#include "Item.h"

class Inventory {
public:
    // add an item to inventory
    bool addItem(const Item& item);

    // remove an item by name
    bool removeItem(const std::string& itemName);

    // check if we have a particular item
    bool hasItem(const std::string& itemName) const;

    // show all items to the player
    void listItems() const;

    // get read-only list for internal logic
    const std::vector<Item>& getItems() const { return items; }

    // describe one item + its tip (for "info <item>")
    bool describeItem(const std::string& itemName) const;

private:
    std::vector<Item> items;
};
