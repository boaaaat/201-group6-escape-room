#pragma once
#include <string>
#include <map>
#include <set>
#include "systems/Inventory.h"

class Player {
public:
    Player(std::string name = "You")
        : name(name) {}

    Inventory& getInventory() { return inventory; }
    const std::string& getName() const { return name; }

    // track location
    // current major room (e.g. "lost_socks")
    std::string currentRoomId;

    // current area inside that room (e.g. "sock_mountain")
    std::string currentAreaId;

    // mark a room/area as visited and unlocked
    void markVisited(const std::string& roomId, const std::string& areaId) {
        visitedRooms.insert(roomId);
        visitedAreas[roomId].insert(areaId);
    }

    const std::set<std::string>& getVisitedRooms() const { return visitedRooms; }
    const std::set<std::string>& getVisitedAreas(const std::string& roomId) const {
        static const std::set<std::string> empty;
        auto it = visitedAreas.find(roomId);
        if (it == visitedAreas.end()) return empty;
        return it->second;
    }
    bool hasVisitedArea(const std::string& roomId, const std::string& areaId) const {
        auto it = visitedAreas.find(roomId);
        if (it == visitedAreas.end()) return false;
        return it->second.count(areaId) > 0;
    }

private:
    std::string name;
    Inventory inventory;
    std::set<std::string> visitedRooms;
    std::map<std::string, std::set<std::string>> visitedAreas;
};
