#include "Room.h"
#include <iostream>
#include "../engine/AudioEngine.h"

// enterRoomIntro():
//   - print intro text / flavor
//   - could run NPC dialogue for that room
void Room::enterRoomIntro(AudioEngine* audioEngine) {
    intro.deliver(audioEngine);
}

Area* Room::getArea(const std::string& areaId) {
    auto it = areas.find(areaId);
    if (it == areas.end()) return nullptr;
    return &(it->second);
}

bool Room::hasArea(const std::string& areaId) const {
    return areas.find(areaId) != areas.end();
}
