#pragma once
#include <string>

// Door connects one Area to another Area (or exits to a new Room).
// A door can be locked behind puzzle conditions.
class Door {
public:
    Door(std::string name = "",
         std::string leadsToRoom = "",
         std::string leadsToArea = "",
         bool locked = false,
         std::string lockReason = "")
        : name(name),
          targetRoom(leadsToRoom),
          targetArea(leadsToArea),
          isLocked(locked),
          lockedText(lockReason) {}

    std::string getName() const { return name; }

    // Return which room the door leads to (could be same room or different room)
    std::string getTargetRoom() const { return targetRoom; }

    // Return which area in that room it leads to
    std::string getTargetArea() const { return targetArea; }

    bool locked() const { return isLocked; }

    // called when we try to use the door but it's locked
    std::string getLockedText() const { return lockedText; }

    // unlock when puzzle solved
    void unlock() { isLocked = false; lockedText = ""; }

private:
    std::string name;
    std::string targetRoom;
    std::string targetArea;
    bool isLocked;
    std::string lockedText;
};
