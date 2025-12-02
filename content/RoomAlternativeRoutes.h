#pragma once
#include "../world/Room.h"

// Third Area: Alternative Routes (Hall of Could-Have-Beens)
class RoomAlternativeRoutes : public Room {
public:
    RoomAlternativeRoutes();

    std::string getStartAreaId() const override;
    bool attemptFinalPuzzle(Inventory& inv,
                            CraftingSystem& crafting) override;

    Area* getAreaMutable(const std::string& areaId) { return getArea(areaId); }

private:
    void buildAreas();
    void markHallVisited();
    void tryUnlockAdminExit();

    bool visitedAnyHall = false;
    bool ballObtained = false;
    bool ballPlaced = false;
    bool mapPieceAFound = false;
    bool mapPieceBFound = false;
    bool mapPieceCFound = false;
    bool mapPlaced = false;
    bool cocoaObtained = false;
    bool cocoaPlaced = false;
    bool adminExitUnlocked = false;
    bool adminLogRead = false;
    bool symbolPathsObtained = false;
};
