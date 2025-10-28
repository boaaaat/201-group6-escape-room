#include "Game.h"
#include "CommandParser.h"
#include "../content/RoomLostSocks.h"
#include "../world/Room.h"
#include <iostream>
#include <algorithm>

class RoomPantryStub : public Room 
{
public:
    RoomPantryStub()
        : Room(
            "pantry",
            "The Pantry of Misplaced Snacks",
            "You step into a kitchen-like space stacked with half-eaten chips and cursed Tupperware.\n"
            "The fridge hums nervously."
        )
    {
        Area entry(
            "pantry_entry",
            "Crumb Floor",
            "You are standing in sticky crumbs. Something smells... historical."
        );
        // Add at least one door back to socks room for now:
        entry.addDoor(
            "back",
            Door(
                "Portal shimmering back to the sock dimension",
                "lost_socks",
                "dryer_portal",
                false,
                ""
            )
        );
        areas["pantry_entry"] = entry;
    }

    std::string getStartAreaId() const override {
        return "pantry_entry";
    }

    bool attemptFinalPuzzle(Inventory& inv,
                            CraftingSystem& crafting) override {
        (void)inv;
        (void)crafting;
        std::cout << "The fridge mumbles: \"Pantry puzzle not implemented yet.\"\n";
        return false;
    }
};



// ===== Game Implementation =====

Game::Game() {
    // Register rooms.
    // Lost Socks room needs CraftingSystem reference so it can inject recipes.
    world.registerRoom(std::make_unique<RoomLostSocks>(crafting));
    world.registerRoom(std::make_unique<RoomPantryStub>());

    // Starting location
    player.currentRoomId = "lost_socks";
    Room* startRoom = world.getRoom(player.currentRoomId);
    if (startRoom) {
        player.currentAreaId = startRoom->getStartAreaId();
        startRoom->enterRoomIntro();
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
}

void Game::run() {
    std::cout << "=== Commands: ===" << std::endl;
    std::cout << " move <direction>" << std::endl;
    std::cout << " observe" << std::endl;
    std::cout << " interact <object>" << std::endl;
    std::cout << " craft <object name in quotes>" << std::endl;
    std::cout << " uncraft" << std::endl;
    std::cout << " recipes" << std::endl;
    std::cout << " hint" << std::endl;
    std::cout << " info <object>" << std::endl;
    std::cout << " inv (see inventory)" << std::endl;
    std::cout << " solve (solve the puzzle)" << std::endl << std::endl;
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

        std::vector<std::string> args;
        if (tokens.size() > 1) {
            args.assign(tokens.begin() + 1, tokens.end());
        }

        if (cmd == "quit" || cmd == "exit") {
            std::cout << "You fade out of this dimension...\n";
            break;

        } else if (cmd == "move") {
            cmdMove(args);

        } else if (cmd == "observe") {
            cmdObserve();

        } else if (cmd == "interact") {
            cmdInteract(args);

        } else if (cmd == "craft") {
            cmdCraft(args);

        } else if (cmd == "uncraft") {
            cmdUncraft();

        } else if (cmd == "recipes") {
            cmdRecipes();

        } else if (cmd == "hint") {
            cmdHint();

        } else if (cmd == "info") {
            cmdInfo(args);

        } else if (cmd == "inv" || cmd == "inventory") {
            cmdInventory();

        } else if (cmd == "solve") {
            tryRoomFinalPuzzle();

        } 
        else if (cmd == "help") {
            std::cout << "=== Commands: ===" << std::endl;
            std::cout << " move <direction>" << std::endl;
            std::cout << " observe" << std::endl;
            std::cout << " interact <object>" << std::endl;
            std::cout << " craft <object name in quotes>" << std::endl;
            std::cout << " uncraft" << std::endl;
            std::cout << " recipes" << std::endl;
            std::cout << " hint" << std::endl;
            std::cout << " info <object>" << std::endl;
            std::cout << " inv (see inventory)" << std::endl;
            std::cout << " solve (solve the puzzle)" << std::endl;
        }
        else {
            std::cout << "Unknown command. Try:\n";
            std::cout << "  move / observe / interact / craft / uncraft / recipes / hint / info / inv / solve / help / quit\n";
        }
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
        std::cout << d.getLockedText() << "\n";
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
        newRoom->enterRoomIntro();
    }

    // auto observe new area
    cmdObserve();
}

void Game::cmdObserve() {
    Area* area = getCurrentArea();
    if (!area) {
        std::cout << "You stare into the void. Nothing stares back. (Area not found)\n";
        return;
    }
    area->observe();
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

    area->interact(objectName, player.getInventory());
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

    crafting.craft(player.getInventory(), itemName);
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
