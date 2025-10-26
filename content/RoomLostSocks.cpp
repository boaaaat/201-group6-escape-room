#include "RoomLostSocks.h"
#include "systems/CraftRecipe.h"
#include <iostream>

// ctor:
//  - call Room ctor with ids/names/text
//  - initialize gatePuzzle with word pairs
//  - call buildAreas() to define the internal layout
RoomLostSocks::RoomLostSocks()
    : Room(
        "lost_socks",
        "Lobby of Lost Socks",
        "You step into a warm, sock-scented dimension. A glowing laundry basket blocks the exit.\n"
        "CLERK (distant): \"Only the truly matched may pass...\""
      ),
      gatePuzzle({
          {"KCSO",  "SOCK"},
          {"HCTAM", "MATCH"},
          {"RIAP",  "PAIR"}
      })
{
    // TODO: buildAreas();
    // NOTE: you should also register starting crafting recipe here, e.g.:
    // Matched Pair = Sock of Truth (L) + Sock of Truth (R)
    //
    // HOWEVER: crafting recipes live in CraftingSystem, which is owned by Game,
    // so you'll likely expose a method later like RoomLostSocks::registerRecipes(CraftingSystem&).
}

// getStartAreaId():
//  - return the area where the player first spawns in this room,
//    e.g. "sock_mountain"
std::string RoomLostSocks::getStartAreaId() const {
    // TODO
    return "sock_mountain";
}

// attemptFinalPuzzle():
//  - check Inventory for "Matched Pair"
//  - if missing, print "basket: you are unworthy"
//  - else run gatePuzzle.play()
//  - if solved:
//       - set solved = true
//       - find the Area/door that leads to Pantry (e.g. "exit_portal")
//       - unlock that door so player can `move east` (which changes room)
bool RoomLostSocks::attemptFinalPuzzle(Inventory& inv, CraftingSystem& crafting) {
    // TODO
    (void)inv;
    (void)crafting;
    return false;
}

// buildAreas():
//  - Create Area("sock_mountain", "Mount Sockmore", "...description...")
//      - addObject("sockpile", "...dig description...",
//                  Item("Sock of Truth (L)","A left sock buzzing with cosmic static.",true,
//                        "Probably needs its partner."),
//                  /*singleUse=*/true)
//      - addDoor("east", Door("Path to Fuzzy Desk", "lost_socks", "fuzzy_desk", false,""))
//  - Create Area("fuzzy_desk", ...)
//      - addObject("desk", "...you find Right Sock... Sock of Truth (R)...", Item(...))
//      - addDoor("west", back to sock_mountain)
//      - addDoor("south", Dryer Portal area)
//  - Create Area("dryer_portal", ...)
//      - addDoor("north", "fuzzy_desk")
//      - addDoor("exit", Door("Portal to Pantry", "pantry", "pantry_entry", /*locked=*/true,
//                             "The laundry basket blocks your way. It demands PROOF OF MATCHING."))
//  - Insert all these Area objects into `areas` map by areaId
void RoomLostSocks::buildAreas() {
    // TODO
}
