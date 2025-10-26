#pragma once
#include "world/Room.h"
#include "puzzles/ScramblePuzzle.h"

// The Lost Socks room defines:
//  - its Areas ("sock_mountain", "fuzzy_desk", "dryer_portal", etc.)
//  - connections (Doors between Areas)
//  - which items can be found by interacting
//  - which crafting recipes we "teach"
//  - final puzzle (ScramblePuzzle) to unlock the exit door
class RoomLostSocks : public Room {
public:
    RoomLostSocks();

    std::string getStartAreaId() const override;

    // attemptFinalPuzzle():
    //  - require crafted "Matched Pair"
    //  - run ScramblePuzzle
    //  - if success:
    //        solved = true
    //        unlock exit door to Pantry room
    bool attemptFinalPuzzle(Inventory& inv,
                            CraftingSystem& crafting) override;

private:
    ScramblePuzzle gatePuzzle;

    // helper to build all Areas and Doors for this room
    void buildAreas();
};
