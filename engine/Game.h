#pragma once
#include <string>
#include "Player.h"
#include "WorldMap.h"
#include "CraftingSystem.h"
#include "HintSystem.h"
#include "AudioEngine.h"
#include "EventSystem.h"

// Game drives the loop:
// - prints intro
// - listens for player commands
// - updates player's location and state
// - checks room final puzzle when player tries to leave the room
class Game {
public:
    Game();

    // run():
    //  - intro text
    //  - main loop:
    //      - get current Room and Area
    //      - prompt player
    //      - read input, parse command
    //      - dispatch
    void run();

private:
    Player player;
    WorldMap world;
    CraftingSystem crafting;
    HintSystem hints;
    AudioEngine audio;
    EventSystem events;

    // helper functions for commands:
    void cmdMove(const std::vector<std::string>& args);
    void cmdObserve();
    void cmdInteract(const std::vector<std::string>& args);
    void cmdCraft(const std::vector<std::string>& args);
    void cmdUncraft();
    void cmdRecipes();
    void cmdHint();
    void cmdInfo(const std::vector<std::string>& args);
    void cmdInventory();
    void cmdPlayAudio(const std::vector<std::string>& args);
    void cmdTeleport();

    // utility:
    //  get current room ptr from world using player.currentRoomId
    //  get current area ptr from that room using player.currentAreaId
    //  print errors if null
    Room* getCurrentRoom();
    Area* getCurrentArea();

    // after solving a room's final puzzle, unlock the exit Door
    // and allow moving into the next Room.
    void tryRoomFinalPuzzle();
};
