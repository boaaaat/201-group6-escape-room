#pragma once
#include <string>
#include "Inventory.h"

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

private:
    std::string name;
    Inventory inventory;
};
