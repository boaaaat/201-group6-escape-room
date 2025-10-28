#include "Area.h"
#include <iostream>

void Area::observe() const {
    std::cout << "=== " << displayName << " ===\n";
    std::cout << description << "\n\n";

    // list objects
    bool anyObjects = false;
    for (const auto& kv : objects) {
        const auto& objName = kv.first;
        const auto& data = kv.second;
        if (!data.alreadyUsed) {
            if (!anyObjects) {
                std::cout << "You notice:\n";
                anyObjects = true;
            }
            std::cout << " - " << objName << "\n";
        }
    }
    if (!anyObjects) {
        std::cout << "Nothing else here looks interactive.\n";
    }
    std::cout << std::endl;

    // list doors
    if (!doors.empty()) {
        std::cout << "Exits:\n";
        for (const auto& kv : doors) {
            const auto& dir = kv.first;
            const auto& d = kv.second;
            std::cout << " - " << dir << " -> " << d.getName();
            if (d.locked()) {
                std::cout << " [LOCKED: " << d.getLockedText() << "]";
            }
            std::cout << "\n";
        }
    }
}

void Area::interact(const std::string& objectName,
                    Inventory& inv) {
    auto it = objects.find(objectName);
    if (it == objects.end()) {
        std::cout << "There's nothing called '" << objectName << "' here.\n";
        return;
    }

    ObjectData& data = it->second;
    std::cout << data.desc << "\n";

    if (!data.alreadyUsed) {
        // give reward item
        if (data.reward.getName() != "") {
            std::cout << "You obtain: " << data.reward.getName() << "\n";
            inv.addItem(data.reward);
        }
        if (data.singleUse) {
            data.alreadyUsed = true;
        }
    } else {
        std::cout << "You've already searched that.\n";
    }
}

void Area::addDoor(const std::string& direction, const Door& d) {
    doors[direction] = d;
}

Door* Area::getDoor(const std::string& direction) {
    auto it = doors.find(direction);
    if (it == doors.end()) return nullptr;
    return &(it->second);
}

void Area::addObject(const std::string& objectName,
                     const std::string& objectDesc,
                     const Item& rewardItem,
                     bool singleUse) {
    ObjectData data;
    data.desc = objectDesc;
    data.reward = rewardItem;
    data.singleUse = singleUse;
    data.alreadyUsed = false;
    objects[objectName] = data;
}
