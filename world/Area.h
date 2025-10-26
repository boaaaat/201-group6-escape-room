#pragma once
#include <string>
#include <map>
#include <vector>
#include "Door.h"
#include "Item.h"
#include "Inventory.h"
#include "HintSystem.h"

// An Area is an explorable sub-location inside a Room.
// Example: "Mount Sockmore" is an Area inside the Lost Socks Room.
class Area {
public:
    Area(std::string id = "",
         std::string niceName = "",
         std::string desc = "")
        : areaId(id), displayName(niceName), description(desc) {}

    std::string getId() const { return areaId; }
    std::string getDisplayName() const { return displayName; }

    // observe():
    //  - print description of area
    //  - list visible interactable objects
    //  - list doors
    void observe() const;

    // interact(objectName, inv):
    //  - handle things like "dig pile", "search desk", etc.
    //  - may add items to inventory
    void interact(const std::string& objectName,
                  Inventory& inv);

    // list doors so the player knows possible move directions
    const std::map<std::string, Door>& getDoors() const { return doors; }

    // addDoor("north", Door(...))
    void addDoor(const std::string& direction, const Door& d);

    // get a mutable ref to a door (to unlock after puzzle)
    Door* getDoor(const std::string& direction);

    // Add an interactable "object" in this area, like "sockpile", "desk"
    // The value is a bool to indicate if it's been looted/used already.
    void addObject(const std::string& objectName,
                   const std::string& objectDesc,
                   const Item& rewardItem,
                   bool singleUse);

    // called by hint system to decide what hint to show
    std::string getHintContextId() const { return "area:" + areaId; }

private:
    struct ObjectData {
        std::string desc;
        Item reward;
        bool singleUse = false;
        bool alreadyUsed = false;
    };

    std::string areaId;        // internal key e.g. "sock_mountain"
    std::string displayName;   // "Mount Sockmore"
    std::string description;   // long description for observe()
    std::map<std::string, ObjectData> objects;
    std::map<std::string, Door> doors;
};
