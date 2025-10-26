#include "Game.h"
#include "CommandParser.h"
#include "RoomLostSocks.h"
#include <iostream>

// Game():
//  - register rooms in world (Lost Socks, Pantry, etc.)
//  - set player's starting room/area
//  - set up crafting recipes known at start (if any)
//  - register hints
Game::Game() {
    // TODO:
    // 1. world.registerRoom(std::make_unique<RoomLostSocks>());
    // 2. player.currentRoomId = "lost_socks";
    //    player.currentAreaId = <LostSocks start area id>;
    // 3. maybe pre-load a basic recipe or none yet
    // 4. hints.registerHint("area:sock_mountain", "Try digging in the sock pile...");
}

// run():
//  - print global intro (clerk scene)
//  - loop until quit command
//  - each loop:
//      - fetch room+area
//      - ask for input
//      - parse
//      - dispatch based on first token:
//          move / observe / interact / craft / uncraft / recipes
//          / hint / info / inv / quit
void Game::run() {
    // TODO:
    // print story setup
    // while true:
    //   1. prompt ">"
    //   2. getline
    //   3. tokenize
    //   4. if empty continue
    //   5. if cmd == "quit", break
    //   6. else if "move": cmdMove(args...)
    //   7. else if "observe": cmdObserve()
    //   8. else if "interact": cmdInteract(args...)
    //   9. else if "craft": cmdCraft(args...)
    //   10. else if "uncraft": cmdUncraft()
    //   11. else if "recipes": cmdRecipes()
    //   12. else if "hint": cmdHint()
    //   13. else if "info": cmdInfo(args...)
    //   14. else if "inv": player.getInventory().listItems();
    //   15. else if "solve" (optional): tryRoomFinalPuzzle();
    //   16. else print "unknown command"
}

// cmdMove(args):
//  - args[0] should be direction ("north","east", etc.)
//  - look at current Area's doors map
//  - if door exists in that direction:
//        - if locked -> print lockedText
//        - else:
//            - update player's currentRoomId / currentAreaId from door target
//            - if new room is different: call room->enterRoomIntro()
//  - if no door -> tell player no path
void Game::cmdMove(const std::vector<std::string>& args) {
    // TODO
    (void)args;
}

// cmdObserve():
//  - get current area and call area->observe()
void Game::cmdObserve() {
    // TODO
}

// cmdInteract(args):
//  - args[0] should be objectName
//  - call area->interact(objectName, player.getInventory())
//  - this might give items, unlock hints, etc.
void Game::cmdInteract(const std::vector<std::string>& args) {
    // TODO
    (void)args;
}

// cmdCraft(args):
//  - args[0] should be itemName we want to craft ("Matched Pair")
//  - crafting.craft(player.getInventory(), itemName)
void Game::cmdCraft(const std::vector<std::string>& args) {
    // TODO
    (void)args;
}

// cmdUncraft():
//  - crafting.uncraft(player.getInventory())
void Game::cmdUncraft() {
    // TODO
}

// cmdRecipes():
//  - crafting.listRecipes()
void Game::cmdRecipes() {
    // TODO
}

// cmdHint():
//  - ask HintSystem using current area's hint context
//  - hints.getHint(area->getHintContextId(), player.getInventory())
void Game::cmdHint() {
    // TODO
}

// cmdInfo(args):
//  - args[0] = itemName
//  - player.getInventory().describeItem(itemName)
void Game::cmdInfo(const std::vector<std::string>& args) {
    // TODO
    (void)args;
}

// getCurrentRoom():
//  - world.getRoom(player.currentRoomId)
Room* Game::getCurrentRoom() {
    // TODO
    return nullptr;
}

// getCurrentArea():
//  - getCurrentRoom()
//  - room->getArea(player.currentAreaId)
Area* Game::getCurrentArea() {
    // TODO
    return nullptr;
}

// tryRoomFinalPuzzle():
//  - get current room
//  - call room->attemptFinalPuzzle(player.getInventory(), crafting)
//  - if solved:
//      - unlock the exit door in some special area
//      - maybe print "The exit to the next dimension hums open..."
void Game::tryRoomFinalPuzzle() {
    // TODO
}
