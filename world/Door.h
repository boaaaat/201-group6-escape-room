#pragma once
#include <string>
#include <optional>
#include "../engine/Dialogue.h"

// Door connects one Area to another Area (or exits to a new Room).
// A door can be locked behind puzzle conditions.
class Door {
public:
    Door(std::string name = "",
         std::string leadsToRoom = "",
         std::string leadsToArea = "",
         bool locked = false,
         std::string lockReason = "",
         std::optional<Dialogue> lockDialogue = std::nullopt)
        : name(name),
          targetRoom(leadsToRoom),
          targetArea(leadsToArea),
          isLocked(locked),
          lockedText(lockReason),
          lockedDialogue(lockDialogue) {}

    std::string getName() const { return name; }

    // Return which room the door leads to (could be same room or different room)
    std::string getTargetRoom() const { return targetRoom; }

    // Return which area in that room it leads to
    std::string getTargetArea() const { return targetArea; }

    bool locked() const { return isLocked; }

    // called when we try to use the door but it's locked
    std::string getLockedText() const { return lockedText; }
    const std::optional<Dialogue>& getLockedDialogue() const { return lockedDialogue; }

    // unlock when puzzle solved
    void unlock() { isLocked = false; lockedText = ""; lockedDialogue.reset(); }

    void setLockedDialogue(const Dialogue& dlg) { lockedDialogue = dlg; }

private:
    std::string name;
    std::string targetRoom;
    std::string targetArea;
    bool isLocked;
    std::string lockedText;
    std::optional<Dialogue> lockedDialogue;
};
