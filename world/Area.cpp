#include "Area.h"
#include <iostream>

// observe():
//  - print area name and description
//  - print list of interactable objects that are not alreadyUsed
//  - print list of doors with direction and locked/unlocked
void Area::observe() const {
    // TODO
}

// interact(objectName, inv):
//  - check if object exists
//  - if doesn't exist: print "there's nothing like that here"
//  - if exists:
//      - print desc / flavor text
//      - if not alreadyUsed:
//          - add reward item to inventory
//          - set alreadyUsed = true if singleUse
void Area::interact(const std::string& objectName,
                    Inventory& inv) {
    // TODO
    (void)inv;
}

void Area::addDoor(const std::string& direction, const Door& d) {
    doors[direction] = d;
}

Door* Area::getDoor(const std::string& direction) {
    auto it = doors.find(direction);
    if (it == doors.end()) return nullptr;
    return &(it->second);
}

// addObject():
//  - create ObjectData from args
//  - insert into objects map
void Area::addObject(const std::string& objectName,
                     const std::string& objectDesc,
                     const Item& rewardItem,
                     bool singleUse) {
    // TODO
}
