#include "Room.h"
#include <iostream>

// enterRoomIntro():
//   - print intro text / flavor
//   - could run NPC dialogue for that room
void Room::enterRoomIntro() {
    // TODO
}

Area* Room::getArea(const std::string& areaId) {
    auto it = areas.find(areaId);
    if (it == areas.end()) return nullptr;
    return &(it->second);
}

bool Room::hasArea(const std::string& areaId) const {
    return areas.find(areaId) != areas.end();
}
