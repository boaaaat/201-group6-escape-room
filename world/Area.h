#pragma once
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "Door.h"
#include "Item.h"
#include "Inventory.h"
#include "InteractableObject.h"
#include "HintSystem.h"
#include "../engine/Dialogue.h"

class AudioEngine;

// An Area is an explorable sub-location inside a Room.
// Example: "Mount Sockmore" is an Area inside the Lost Socks Room.
class Area {
public:
    Area(std::string id = "",
         std::string niceName = "",
         Dialogue desc = Dialogue(""))
        : areaId(id), displayName(niceName), description(desc) {}

    std::string getId() const { return areaId; }
    std::string getDisplayName() const { return displayName; }

    // observe():
    //  - print description of area
    //  - list visible interactable objects
    //  - list doors
    void observe(AudioEngine* audioEngine = nullptr) const;

    // interact(objectName, inv):
    //  - handle things like "dig pile", "search desk", etc.
    //  - may add items to inventory
    void interact(const std::string& objectName,
                  Inventory& inv,
                  AudioEngine* audioEngine = nullptr);

    // list doors so the player knows possible move directions
    const std::map<std::string, Door>& getDoors() const { return doors; }

    // addDoor("north", Door(...))
    void addDoor(const std::string& direction, const Door& d);

    // get a mutable ref to a door (to unlock after puzzle)
    Door* getDoor(const std::string& direction);

    // Add an interactable object in this area.
    void addObject(const std::string& objectName, const InteractableObject& object);
    // Convenience helpers for a simple single-interaction object
    void addObject(const std::string& objectName,
                   const std::string& objectDesc,
                   const Item& rewardItem,
                   bool singleUse);
    void addObject(const std::string& objectName,
                   const Dialogue& dialogue,
                   const Item& rewardItem,
                   bool singleUse);

    void setDescriptionDialogue(const Dialogue& dlg) { description = dlg; }
    void setObjectDialogue(const std::string& objectName, const Dialogue& dlg);

    // called by hint system to decide what hint to show
    std::string getHintContextId() const { return "area:" + areaId; }

    // ordered listings for QoL numbering
    std::vector<std::string> getVisibleObjectNames() const;
    std::vector<std::string> getDoorDirections() const;

private:
    std::string areaId;        // internal key e.g. "sock_mountain"
    std::string displayName;   // "Mount Sockmore"
    Dialogue description;      // long description for observe()
    std::map<std::string, InteractableObject> objects;
    std::map<std::string, Door> doors;
};
