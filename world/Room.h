#pragma once
#include <string>
#include <map>
#include <memory>
#include "Area.h"
#include "Inventory.h"
#include "CraftingSystem.h"
#include "Puzzle.h"
#include "../engine/Dialogue.h"

class AudioEngine;

// A Room is a major chapter (Lost Socks, Pantry, etc.).
// It owns multiple Areas and one final gate puzzle.
// The room is considered "cleared" when final puzzle solved.
class Room {
public:
    Room(std::string id = "",
         std::string niceName = "",
         Dialogue introText = Dialogue(""))
        : roomId(id), displayName(niceName),
          intro(introText), solved(false) {}

    virtual ~Room() {}

    std::string getId() const { return roomId; }
    std::string getDisplayName() const { return displayName; }
    void setIntroDialogue(const Dialogue& dlg) { intro = dlg; }
    const Dialogue& getIntroDialogue() const { return intro; }

    // called when player first (or re-)enters the room
    virtual void enterRoomIntro(AudioEngine* audioEngine = nullptr);

    // get current Area object
    Area* getArea(const std::string& areaId);

    // used by Game to move player between areas
    bool hasArea(const std::string& areaId) const;

    // return starting area for this room (like spawn point)
    virtual std::string getStartAreaId() const = 0;

    // attempt final puzzle:
    //   - uses items from inventory and crafting
    //   - if success: set solved = true
    virtual bool attemptFinalPuzzle(Inventory& inv,
                                    CraftingSystem& crafting) = 0;

    bool isSolved() const { return solved; }

    // Expose internal areas map for Game to look up
    const std::map<std::string, Area>& getAllAreas() const { return areas; }

protected:
    std::string roomId;
    std::string displayName;
    Dialogue intro;
    bool solved;

    // map of areaId -> Area
    std::map<std::string, Area> areas;
};
