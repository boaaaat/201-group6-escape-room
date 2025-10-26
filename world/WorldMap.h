#pragma once
#include <map>
#include <memory>
#include <string>
#include "Room.h"

// WorldMap holds every major Room (Lost Socks, Pantry...)
// Game queries this to resolve navigation across rooms.
class WorldMap {
public:
    // registerRoom:
    //  - store unique_ptr<Room> in map
    void registerRoom(std::unique_ptr<Room> room);

    // getRoom by id
    Room* getRoom(const std::string& roomId);

private:
    std::map<std::string, std::unique_ptr<Room>> rooms;
};
