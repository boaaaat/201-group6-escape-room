#pragma once
#include "../world/Room.h"

// RoomSpectator contains the first area and feeds items forward to later rooms.
class RoomSpectator : public Room {
public:
    RoomSpectator();

    std::string getStartAreaId() const override;
    bool attemptFinalPuzzle(Inventory& inv,
                            CraftingSystem& crafting) override;

    Area* getAreaMutable(const std::string& areaId) { return getArea(areaId); }

private:
    void buildAreas();
    void unlockAwakeningDoor();
    void openSockyExits();
    void unlockCocoavilleExit();

    bool hasToken(const Inventory& inv, const std::string& name) const;
    void giveToken(Inventory& inv, const std::string& name,
                   const std::string& desc);

    bool doorUnlocked = false;
    bool keyPromptShown = false;
    bool keyObtained = false;
    int sockpileProgress = 0;
    bool sockpostSeen = false;
    bool sockCollected = false;
    bool sockPlaced = false;
    bool soccerPortalOpened = false;
    bool fireplaceCleared = false;
    bool cocoaTaken = false;
    bool lensObtained = false;
    bool cableObtained = false;
    bool giftBoxObtained = false;
    bool symbolSocksObtained = false;
};
