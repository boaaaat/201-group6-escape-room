#include "Game.h"
#include "CommandParser.h"
#include "../content/RoomLostSocks.h"
#include "../content/RoomSpectator.h"
#include "../content/RoomMemories.h"
#include "../content/RoomAlternativeRoutes.h"
#include "../content/RoomFinalChamber.h"
#include "../systems/Item.h"
#include "../world/Room.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <Helpers.h>

// ===== Game Implementation =====

Game::Game() {
    // Register rooms.
    // Lost Socks room needs CraftingSystem reference so it can inject recipes.
    world.registerRoom(std::make_unique<RoomFinalChamber>());
    world.registerRoom(std::make_unique<RoomAlternativeRoutes>());
    world.registerRoom(std::make_unique<RoomMemories>());
    world.registerRoom(std::make_unique<RoomSpectator>());
    // world.registerRoom(std::make_unique<RoomLostSocks>(crafting));

    // Starting location
    player.currentRoomId = "spectator";
    Room* startRoom = world.getRoom(player.currentRoomId);
    if (startRoom) {
        player.currentAreaId = startRoom->getStartAreaId();
        startRoom->enterRoomIntro(&audio);
        player.markVisited(player.currentRoomId, player.currentAreaId);
    }

    // Register some basic hints for areas
    hints.registerHint(
        "area:awakening_cell",
        "Try interacting with the sockpile more than once; something useful might be buried there."
    );
    hints.registerHint(
        "area:socky_field",
        "The Sockpost is missing a specific sock. Check the sock pile again after examining it."
    );
    hints.registerHint(
        "area:cocoaville",
        "Grab the Cup of Cocoa and see what happens if you approach the fireplace with it."
    );
    hints.registerHint(
        "area:projector_room",
        "Interact with the stand to learn what's missing, then re-check old areas for parts."
    );
    hints.registerHint(
        "area:forgotten_maze",
        "Think: Right path, then Left, then 4th door, then Truth."
    );
    hints.registerHint(
        "area:observatory",
        "Point the telescope upward, then give it a 180 spin."
    );
}

void Game::run() {
    std::cout << "=== Commands: ===" << std::endl;
    std::cout << " move <direction> (m)" << std::endl;
    std::cout << " observe (obs)" << std::endl;
    std::cout << " interact <object|number> (int)" << std::endl;
    // std::cout << " craft <object name> (c)" << std::endl;
    // std::cout << " uncraft (u)" << std::endl;
    // std::cout << " recipes (r)" << std::endl;
    // std::cout << " hint (h)" << std::endl;
    std::cout << " info <object> (i)" << std::endl;
    std::cout << " inv (see inventory)" << std::endl;
    std::cout << " teleport (tp)" << std::endl;
    std::cout << " help (see the command list)\n";

    while (true) {
        std::cout << "\n> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;

        auto tokens = CommandParser::tokenize(line);
        if (tokens.empty()) continue;

        // lowercase the command verb
        std::string cmd = tokens[0];
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        // capture area context at the moment the command was entered
        std::string commandAreaContext = player.currentAreaId;

        std::vector<std::string> args;
        if (tokens.size() > 1) {
            args.assign(tokens.begin() + 1, tokens.end());
        }

        if (cmd == "quit" || cmd == "exit") {
            std::cout << "You fade out of this dimension...\n";
            break;

        } else if (cmd == "move" || cmd == "m") {
            cmdMove(args);

        } else if (cmd == "observe" || cmd == "obs") {
            cmdObserve();

        } else if (cmd == "interact" || cmd == "int") {
            cmdInteract(args);

        } else if (cmd == "craft" || cmd == "c") {
            cmdCraft(args);

        } else if (cmd == "uncraft" || cmd == "u") {
            cmdUncraft();

        } else if (cmd == "recipes" || cmd == "r") {
            cmdRecipes();

        } else if (cmd == "hint" || cmd == "h") {
            cmdHint();

        } else if (cmd == "info" || cmd == "i") {
            cmdInfo(args);

        } else if (cmd == "inv" || cmd == "inventory") {
            cmdInventory();

        } else if (cmd == "audio" || cmd == "sound") {
            cmdPlayAudio(args);

        } else if (cmd == "teleport" || cmd == "tp") {
            cmdTeleport();

        } else if (cmd == "solve" || cmd == "s") {
            tryRoomFinalPuzzle();

        } 
        else if (cmd == "help") {
            std::cout << "=== Commands: ===" << std::endl;
            std::cout << " move <direction|number> (m)" << std::endl;
            std::cout << " observe (obs)" << std::endl;
            std::cout << " interact <object|number> (int)" << std::endl;
            // std::cout << " craft <object name> (c)" << std::endl;
            // std::cout << " uncraft (u)" << std::endl;
            // std::cout << " recipes (r)" << std::endl;
            // std::cout << " hint (h)" << std::endl;
            std::cout << " info <object> (i)" << std::endl;
            std::cout << " inv (see inventory)" << std::endl;
            std::cout << " teleport (tp)" << std::endl;
            std::cout << " solve (solve the final puzzle) (s)" << std::endl << std::endl;
        }
        else 
        {
            std::cout << "Unknown command. Try:\n";
            std::cout << "  move / observe / interact / hint / info / inv / audio / solve / help / quit\n";
        }

        events.handleCommand(commandAreaContext, cmd, &audio);
    }
}

Room* Game::getCurrentRoom() {
    return world.getRoom(player.currentRoomId);
}

Area* Game::getCurrentArea() {
    Room* r = getCurrentRoom();
    if (!r) return nullptr;
    return r->getArea(player.currentAreaId);
}

void Game::cmdMove(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Move where? (example: move east)\n";
        return;
    }
    std::string direction = args[0];

    Area* area = getCurrentArea();
    Room* room = getCurrentRoom();
    if (!area || !room) {
        std::cout << "You feel spatially confused. (No current area/room.)\n";
        return;
    }

    // If user passed a number, map it to a door in the current listing.
    const auto& doors = area->getDoors();
    auto doorDirections = area->getDoorDirections();
    bool usedNumber = std::all_of(direction.begin(), direction.end(), ::isdigit);
    if (usedNumber && !doorDirections.empty()) {
        size_t idx = std::stoul(direction);
        if (idx == 0 || idx > doorDirections.size()) {
            std::cout << "No exit with that number.\n";
            return;
        }
        direction = doorDirections[idx - 1];
    }

    auto it = doors.find(direction);
    if (it == doors.end()) {
        std::cout << "There's no exit that way.\n";
        return;
    }

    const Door& d = it->second;
    if (d.locked()) {
        if (d.getLockedDialogue().has_value()) {
            d.getLockedDialogue()->deliver(&audio);
        } else {
            std::cout << d.getLockedText() << "\n";
        }
        return;
    }

    std::string targetRoomId = d.getTargetRoom();
    std::string targetAreaId = d.getTargetArea();

    // move
    player.currentRoomId = targetRoomId;
    player.currentAreaId = targetAreaId;
    player.markVisited(player.currentRoomId, player.currentAreaId);

    Room* newRoom = getCurrentRoom();
    if (newRoom && newRoom != room) {
        // entering a new major room => intro
        newRoom->enterRoomIntro(&audio);
    }

    Area* newArea = getCurrentArea();
    if (newArea && newArea != area) {
        if (player.currentAreaId == "socky_field" &&
            !player.getInventory().hasItem("Token:ProjectorQuestStarted")) {
            player.getInventory().addItem(Item(
                "Token:ProjectorQuestStarted",
                "Quest flag: Projector parts are now findable.",
                false,
                ""
            ));
        }
    }

    // auto observe new area
    std::cout << "\033[2J\033[1;1H";
    cmdObserve();
}

void Game::cmdObserve() {
    Area* area = getCurrentArea();
    if (!area) {
        std::cout << "You stare into the void. Nothing stares back. (Area not found)\n";
        return;
    }
    area->observe(&audio);
}

void Game::cmdInteract(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Interact with what?\n";
        return;
    }

    Area* area = getCurrentArea();
    if (!area) {
        std::cout << "You reach out, but reality is missing.\n";
        return;
    }

    // join all args back into one objectName (to support multiword)
    std::string objectName = args[0];
    for (size_t i = 1; i < args.size(); ++i) {
        objectName += " ";
        objectName += args[i];
    }

    // If numeric, map to nth visible object.
    bool usedNumber = std::all_of(objectName.begin(), objectName.end(), ::isdigit);
    if (usedNumber) {
        auto visibleObjects = area->getVisibleObjectNames();
        if (visibleObjects.empty()) {
            std::cout << "There is nothing to interact with here.\n";
            return;
        }
        size_t idx = std::stoul(objectName);
        if (idx == 0 || idx > visibleObjects.size()) {
            std::cout << "No object with that number.\n";
            return;
        }
        objectName = visibleObjects[idx - 1];
    }

    area->interact(objectName, player.getInventory(), &audio);
}

void Game::cmdCraft(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Craft what? (Use recipes to see options)\n";
        return;
    }

    // join args to support multi-word items
    std::string itemName = args[0];
    for (size_t i = 1; i < args.size(); ++i) {
        itemName += " ";
        itemName += args[i];
    }

    bool usedNumber = std::all_of(itemName.begin(), itemName.end(), ::isdigit);
    if (usedNumber) {
        size_t idx = std::stoul(itemName);
        if (idx == 0) {
            std::cout << "No recipe with that number.\n";
            return;
        }
        crafting.craft(player.getInventory(), idx - 1);
        return;
    }

    crafting.craft(player.getInventory(), toLower(itemName));
}

void Game::cmdUncraft() {
    crafting.uncraft(player.getInventory());
}

void Game::cmdRecipes() {
    crafting.listRecipes();
}

void Game::cmdHint() {
    Area* area = getCurrentArea();
    if (!area) {
        std::cout << "You attempt to request cosmic guidance, but you are nowhere.\n";
        return;
    }
    std::string ctx = area->getHintContextId();
    hints.getHint(ctx, player.getInventory());
}

void Game::cmdInfo(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Info on what item?\n";
        return;
    }

    // join args into item name
    std::string itemName = args[0];
    for (size_t i = 1; i < args.size(); ++i) {
        itemName += " ";
        itemName += args[i];
    }

    // allow numeric selection from inventory list
    bool usedNumber = std::all_of(itemName.begin(), itemName.end(), ::isdigit);
    if (usedNumber) {
        size_t idx = std::stoul(itemName);
        if (idx == 0) {
            std::cout << "No item with that number.\n";
            return;
        }
        const auto& items = player.getInventory().getItems();
        if (idx > items.size()) {
            std::cout << "No item with that number.\n";
            return;
        }
        itemName = items[idx - 1].getName();
    }

    player.getInventory().describeItem(itemName);
}

void Game::cmdPlayAudio(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Play which MP3? Example: audio assets/voice/intro.mp3\n";
        return;
    }

    std::string path = args[0];
    for (size_t i = 1; i < args.size(); ++i) {
        path += " ";
        path += args[i];
    }

    bool ok = audio.playMp3(path);
    if (!ok) {
        std::cout << "[Audio] Unable to start playback. Ensure the path is correct and a command-line player is available.\n";
    }
}

void Game::cmdInventory() 
{
    player.getInventory().listItems();
}

void Game::cmdTeleport() {
    const auto& visitedRooms = player.getVisitedRooms();
    if (visitedRooms.empty()) {
        std::cout << "You haven't been anywhere to teleport to yet.\n";
        return;
    }

    std::vector<std::string> roomIds(visitedRooms.begin(), visitedRooms.end());
    std::cout << "=== Teleport: Visited Rooms ===\n";
    for (size_t i = 0; i < roomIds.size(); ++i) {
        Room* r = world.getRoom(roomIds[i]);
        std::string name = (r ? r->getDisplayName() : roomIds[i]);
        std::cout << (i + 1) << ". " << name << "\n";
    }

    std::cout << "Select room number (or press Enter to cancel): ";
    std::string roomChoiceStr;
    std::getline(std::cin, roomChoiceStr);
    if (roomChoiceStr.empty()) {
        std::cout << "Teleport cancelled.\n";
        return;
    }
    if (!std::all_of(roomChoiceStr.begin(), roomChoiceStr.end(), ::isdigit)) {
        std::cout << "Please enter a valid number.\n";
        return;
    }

    size_t roomChoice = std::stoul(roomChoiceStr);
    if (roomChoice == 0 || roomChoice > roomIds.size()) {
        std::cout << "No room with that number.\n";
        return;
    }
    std::string chosenRoomId = roomIds[roomChoice - 1];
    Room* chosenRoom = world.getRoom(chosenRoomId);
    if (!chosenRoom) {
        std::cout << "That room no longer exists.\n";
        return;
    }

    const auto& visitedAreas = player.getVisitedAreas(chosenRoomId);
    if (visitedAreas.empty()) {
        std::cout << "You haven't visited any areas in that room yet.\n";
        return;
    }

    std::vector<std::string> areaIds;
    for (const auto& id : visitedAreas) {
        if (chosenRoom->hasArea(id)) {
            areaIds.push_back(id);
        }
    }
    if (areaIds.empty()) {
        std::cout << "No valid areas to teleport to in that room.\n";
        return;
    }

    std::cout << "=== Teleport: Areas in " << chosenRoom->getDisplayName() << " ===\n";
    for (size_t i = 0; i < areaIds.size(); ++i) {
        Area* a = chosenRoom->getArea(areaIds[i]);
        std::string name = (a ? a->getDisplayName() : areaIds[i]);
        std::cout << (i + 1) << ". " << name << "\n";
    }

    std::cout << "Select area number (or press Enter to cancel): ";
    std::string areaChoiceStr;
    std::getline(std::cin, areaChoiceStr);
    if (areaChoiceStr.empty()) {
        std::cout << "Teleport cancelled.\n";
        return;
    }
    if (!std::all_of(areaChoiceStr.begin(), areaChoiceStr.end(), ::isdigit)) {
        std::cout << "Please enter a valid number.\n";
        return;
    }

    size_t areaChoice = std::stoul(areaChoiceStr);
    if (areaChoice == 0 || areaChoice > areaIds.size()) {
        std::cout << "No area with that number.\n";
        return;
    }

    // execute teleport
    std::string previousRoomId = player.currentRoomId;
    Room* previousRoom = world.getRoom(previousRoomId);
    player.currentRoomId = chosenRoomId;
    player.currentAreaId = areaIds[areaChoice - 1];
    player.markVisited(player.currentRoomId, player.currentAreaId);

    Room* destinationRoom = world.getRoom(player.currentRoomId);
    if (destinationRoom && destinationRoom != previousRoom) {
        destinationRoom->enterRoomIntro(&audio);
    }

    std::cout << "\033[2J\033[1;1H";
    cmdObserve();
}

void Game::tryRoomFinalPuzzle() {
    Room* r = getCurrentRoom();
    if (!r) {
        std::cout << "You can't solve a room that doesn't exist.\n";
        return;
    }

    bool before = r->isSolved();
    bool after = r->attemptFinalPuzzle(player.getInventory(), crafting);

    if (!before && after) {
        std::cout << "[A new exit shimmers open. Try 'move exit'.]\n";
    }
}
