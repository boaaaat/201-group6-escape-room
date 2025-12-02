#pragma once
#include "../world/Room.h"

// Second Area: Memories of the past
class RoomMemories : public Room {
public:
    RoomMemories();

    std::string getStartAreaId() const override;
    bool attemptFinalPuzzle(Inventory& inv,
                            CraftingSystem& crafting) override;

    Area* getAreaMutable(const std::string& areaId) { return getArea(areaId); }

private:
    void buildAreas();
    void unlockProjectorDoor();
    void unlockMazeExit();
    void unlockThirdAreaDoor();

    bool projectorQuestStarted = false;
    bool lensInstalled = false;
    bool cableInstalled = false;
    bool filmInstalled = false;
    int projectorPartsInstalled = 0;
    bool projectorComplete = false;
    bool filmReelObtained = false;
    bool mazeSolved = false;
    int mazeStage = 0;
    bool telescopeOrientationUp = false;
    bool telescopeSolved = false;
    bool symbolMemoryObtained = false;
    bool symbolSelfObtained = false;
};
