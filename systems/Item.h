#pragma once
#include <string>

class Item {
public:
    Item(std::string name = "",
         std::string desc = "",
         bool key = false,
         std::string tip = "")
        : name(name), description(desc), keyItem(key), tipText(tip) {}

    // returns the display name of the item
    std::string getName() const { return name; }

    // short description of the item
    std::string getDescription() const { return description; }

    // is this a story-critical / puzzle-relevant / craftable object?
    bool isKeyItem() const { return keyItem; }

    // hint text / usage tips for the "info" command
    std::string getTip() const { return tipText; }

private:
    std::string name;
    std::string description;
    bool keyItem;
    std::string tipText;
};
