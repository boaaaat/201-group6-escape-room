#include "Game.h"
#include "CommandParser.h"
#include "../content/RoomLostSocks.h"
#include "../world/Room.h"
#include <iostream>
#include <algorithm>
#include <Helpers.h>

// ===== Game Implementation =====

Game::Game() {
    // Register rooms.
    // Lost Socks room needs CraftingSystem reference so it can inject recipes.
    world.registerRoom(std::make_unique<RoomLostSocks>(crafting));

    // Starting location
    player.currentRoomId = "lost_socks";
    Room* startRoom = world.getRoom(player.currentRoomId);
    if (startRoom) {
        player.currentAreaId = startRoom->getStartAreaId();
        startRoom->enterRoomIntro(&audio);
    }

    // Register some basic hints for areas
    hints.registerHint(
        "area:sock_mountain",
        "Try 'interact sockpile'. Something might be buried in the fluff."
    );
    hints.registerHint(
        "area:fuzzy_desk",
        "Maybe 'interact desk'. The sticky note looks suspicious."
    );
    hints.registerHint(
        "area:dryer_portal",
        "If you're stuck, craft \"Matched Pair\" and then try 'solve'."
    );

    // Example inner-monologue event when the player observes the sock mountain.
    events.registerCommandEvent(
        "sock_mountain",
        "observe",
        Dialogue(
            "You feel like the socks are watching you... maybe there's something buried here.",
            "speech\\RoomLostSocks\\room_intro.mp3"  // attach an MP3 path here to auto-play TTS
        ),
        /*fireOnce=*/true
    );
}

void Game::run() {
    std::cout << "=== Commands: ===" << std::endl;
    std::cout << " move <direction> (m)" << std::endl;
    std::cout << " observe (obs)" << std::endl;
    std::cout << " interact <object> (int)" << std::endl;
    std::cout << " craft <object name> (c)" << std::endl;
    std::cout << " uncraft (u)" << std::endl;
    std::cout << " recipes (r)" << std::endl;
    std::cout << " hint (h)" << std::endl;
    std::cout << " info <object> (i)" << std::endl;
    std::cout << " inv (see inventory)" << std::endl;
    std::cout << " solve (solve the puzzle) (s)" << std::endl << std::endl;
    std::cout << "[You awaken in the Interdimensional Lost & Found...]\n";
    std::cout << "CLERK: \"Four rooms. Four proofs. Reclaim yourself.\"\n";

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

        } else if (cmd == "solve" || cmd == "s") {
            tryRoomFinalPuzzle();

        } 
        else if (cmd == "help") {
            std::cout << "=== Commands: ===" << std::endl;
            std::cout << " move <direction> (m)" << std::endl;
            std::cout << " observe (obs)" << std::endl;
            std::cout << " interact <object> (int)" << std::endl;
            std::cout << " craft <object name> (c)" << std::endl;
            std::cout << " uncraft (u)" << std::endl;
            std::cout << " recipes (r)" << std::endl;
            std::cout << " hint (h)" << std::endl;
            std::cout << " info <object> (i)" << std::endl;
            std::cout << " inv (see inventory)" << std::endl;
            std::cout << " audio <mp3 path> (play sound effect or dialogue)" << std::endl;
            std::cout << " solve (solve the puzzle) (s)" << std::endl << std::endl;
        }
        else {
            std::cout << "Unknown command. Try:\n";
            std::cout << "  move / observe / interact / craft / uncraft / recipes / hint / info / inv / audio / solve / help / quit\n";
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

    const auto& doors = area->getDoors();
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

    Room* newRoom = getCurrentRoom();
    if (newRoom && newRoom != room) {
        // entering a new major room => intro
        newRoom->enterRoomIntro(&audio);
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
