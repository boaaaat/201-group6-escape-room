#include "WorldMap.h"

void WorldMap::registerRoom(std::unique_ptr<Room> room) 
{
    std::string id = room->getId();
    rooms[id] = std::move(room);
}

Room* WorldMap::getRoom(const std::string& roomId) 
{
    auto it = rooms.find(roomId);
    if (it == rooms.end()) return nullptr;
    return it->second.get();
}
