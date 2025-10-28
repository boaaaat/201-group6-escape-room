#pragma once
#include "../world/Room.h"
#include "../puzzles/ScramblePuzzle.h"
#include "../systems/CraftingSystem.h"

class RoomLostSocks : public Room {
public:
    RoomLostSocks(CraftingSystem& craftRef);

    std::string getStartAreaId() const override;

    bool attemptFinalPuzzle(Inventory& inv,
                            CraftingSystem& crafting) override;

    // expose a helper to unlock the exit door once solved
    // so Game can allow moving to Pantry
    void unlockExitDoor();

    // for Game to check/unlock doors
    Area* getAreaMutable(const std::string& areaId) { return getArea(areaId); }

private:
    ScramblePuzzle gatePuzzle;
    CraftingSystem& craftingRef;

    void buildAreas();
};
