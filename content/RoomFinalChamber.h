#pragma once
#include "../world/Room.h"

// Fourth Area: Ending chamber where all routes meet.
class RoomFinalChamber : public Room {
public:
    RoomFinalChamber();

    std::string getStartAreaId() const override;
    bool attemptFinalPuzzle(Inventory& inv,
                            CraftingSystem& crafting) override;

private:
    void buildAreas();
    void checkAllPlaced();

    bool socksPlaced = false;
    bool memoryPlaced = false;
    bool pathsPlaced = false;
    bool selfPlaced = false;
    bool finalDoorAppeared = false;
};
