#include "RoomMemories.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "../world/InteractableObject.h"
#include "../engine/Dialogue.h"

namespace {
const std::string kTokenProjector = "Token:ProjectorQuestStarted";
const std::string kTokenRiddleMemory = "Token:RiddleMemoryHeard";
const std::string kTokenRiddleSelf = "Token:RiddleSelfHeard";
}

RoomMemories::RoomMemories()
    : Room(
        "memories",
        "Memories of the Past",
        Dialogue(
            "HAHAHA. You know what Subject #99, because you made me laugh I'm willing to help you out, if you look to the left\n"
            "you will notice a teleporter. Cool right? You now have access to the teleporter."
        )
      )
{
    buildAreas();
}

std::string RoomMemories::getStartAreaId() const {
    return "projector_room";
}

bool RoomMemories::attemptFinalPuzzle(Inventory& /*inv*/,
                                      CraftingSystem& /*crafting*/) {
    return solved;
}

void RoomMemories::unlockProjectorDoor() {
    Area* projector = getArea("projector_room");
    if (!projector) return;
    if (Door* west = projector->getDoor("west")) {
        west->unlock();
    }
}

void RoomMemories::unlockMazeExit() {
    Area* maze = getArea("forgotten_maze");
    if (!maze) return;
    if (Door* north = maze->getDoor("north")) {
        north->unlock();
    }
}

void RoomMemories::unlockThirdAreaDoor() {
    Area* observatory = getArea("observatory");
    if (!observatory) return;
    if (Door* north = observatory->getDoor("north")) {
        north->unlock();
    }
}

void RoomMemories::buildAreas() {
    Area projectorRoom(
        "projector_room",
        "Second Area - Memories of the past",
        Dialogue(
            "As you observe your surroundings, you notice that you're in a dusty room with what seems to be a stand in the middle.\n"
            "-Stand\n"
            "-Door"
        )
    );

    projectorRoom.addDoor(
        "south",
        Door(
            "Back to Cocoaville",
            "spectator",
            "cocoaville",
            false,
            ""
        )
    );
    projectorRoom.addDoor(
        "west",
        Door(
            "Toward The Forgotten Maze",
            "memories",
            "forgotten_maze",
            true,
            "The door won't budge until the projector is complete."
        )
    );

    InteractableObject teleporter(Dialogue("The teleporter hums patiently, waiting for you to pick a destination with the move command."));
    InteractableObject::Interaction teleporterUse;
    teleporterUse.id = "teleporter_use";
    teleporterUse.dialogue = Dialogue(
        "The teleporter crackles. You could 'move south' back to Cocoaville or explore west once the door is unlocked."
    );
    teleporterUse.singleUse = false;
    teleporter.addInteraction(teleporterUse);

    InteractableObject stand(Dialogue("The movie projector waits, either finished or as complete as it will ever be."));
    InteractableObject::Interaction standUse;
    standUse.id = "stand_use";
    standUse.dialogue = Dialogue("");
    standUse.singleUse = false;
    standUse.onUse = [this](Inventory& inv, AudioEngine*) {
        if (!projectorQuestStarted) {
            projectorQuestStarted = true;
            std::cout << "You get closer to the stand, and as you do, you can see that this stand is a movie projector stand pointing towards the door. Getting a closer look, you realize it's not working. It's missing some parts.\n";
            if (!inv.hasItem(kTokenProjector)) {
                inv.addItem(Item(
                    kTokenProjector,
                    "Quest flag: Projector parts are now findable.",
                    false,
                    ""
                ));
            }
            return;
        }

        if (projectorComplete) {
            std::cout << "The projector hums softly, a beam aimed at the door.\n";
            return;
        }

        bool hasLens = inv.hasItem("Lens") && !lensInstalled;
        bool hasCable = inv.hasItem("Power Cable") && !cableInstalled;
        bool hasFilm = inv.hasItem("Film Reel") && !filmInstalled;

        std::vector<std::pair<std::string, std::string>> options;
        int optionNumber = 1;
        if (hasLens) options.push_back({std::to_string(optionNumber++), "Lens"});
        if (hasCable) options.push_back({std::to_string(optionNumber++), "Power Cable"});
        if (hasFilm) options.push_back({std::to_string(optionNumber++), "Film Reel"});

        if (options.empty()) {
            std::cout << "You still need missing parts. A lens, a cable, and a film reel come to mind.\n";
            return;
        }

        std::cout << "You take a closer look at the movie projector, as you do you realize you have one of the missing parts. You decide to\n";
        for (const auto& opt : options) {
            std::cout << "-Connect the " << opt.second << " (" << opt.first << ")\n";
        }
        std::cout << "-Exit (0)\n";
        std::string choice;
        std::getline(std::cin, choice);

        auto it = std::find_if(options.begin(), options.end(), [&](const auto& p){ return p.first == choice; });
        if (it == options.end()) {
            std::cout << "You step back from the projector for now.\n";
            return;
        }

        const std::string part = it->second;
        inv.removeItem(part);
        if (part == "Lens") lensInstalled = true;
        if (part == "Power Cable") cableInstalled = true;
        if (part == "Film Reel") filmInstalled = true;
        projectorPartsInstalled = static_cast<int>(lensInstalled) + static_cast<int>(cableInstalled) + static_cast<int>(filmInstalled);

        if (projectorPartsInstalled == 1) {
            std::cout << "As you connect the missing part to the movie projector, you realize it's still not complete.\n";
        } else if (projectorPartsInstalled == 2) {
            std::cout << "As you connect the missing part to the movie projector, it feels as if it's almost complete.\n";
        } else if (projectorPartsInstalled >= 3) {
            projectorComplete = true;
            std::cout << "As you connect the missing part to the movie projector. You feel as if the movie projector is finally complete.\n";
            std::cout << "You decide to turn it on feeling accomplished. After turning it on, you can see that it's projecting a small circle towards the door. You hear a click.\n";
            unlockProjectorDoor();
        }
    };
    stand.addInteraction(standUse);

    InteractableObject projectorDoor(Dialogue("The door is already open, leading west."));
    InteractableObject::Interaction projectorDoorCheck;
    projectorDoorCheck.id = "projector_door_check";
    projectorDoorCheck.dialogue = Dialogue("");
    projectorDoorCheck.singleUse = false;
    projectorDoorCheck.onUse = [this](Inventory& /*inv*/, AudioEngine*) {
        if (!projectorComplete) {
            std::cout << "As you get near the door you attempt to open it, surprisingly enough it's locked. Who would have thought.\n";
        } else {
            std::cout << "As you approach the door, you can faintly see a small boy being displayed, but care not enough to stop you in your tracks. You open the door.\n";
            std::cout << "- Move West (The Forgotten Maze)\n";
            std::cout << "- Move South (Cocoaville)\n";
        }
    };
    projectorDoor.addInteraction(projectorDoorCheck);

    InteractableObject giftBox(Dialogue("You've already opened the gift box and taken what's inside."));
    InteractableObject::Interaction giftBoxOpen;
    giftBoxOpen.id = "giftbox_open";
    giftBoxOpen.dialogue = Dialogue("");
    giftBoxOpen.singleUse = false;
    giftBoxOpen.onUse = [this](Inventory& inv, AudioEngine*) {
        if (!inv.hasItem("Gift Box")) {
            std::cout << "You glance around for that gift box. Maybe it's still under the tree.\n";
            return;
        }
        if (filmReelObtained) {
            std::cout << "The box is empty now.\n";
            return;
        }

        std::cout << "A gift box you stole, I mean found.\n";
        std::cout << "-Open (1)\n";
        std::cout << "-Leave alone (2)\n";
        std::string choice;
        std::getline(std::cin, choice);
        if (choice == "1") {
            std::cout << "As you open the gift box you find a Film Reel. You don't know what it's for but you keep it anyway.\n";
            std::cout << "You have obtained a Film Reel.\n";
            inv.removeItem("Gift Box");
            inv.addItem(Item(
                "Film Reel",
                "A dusty reel of film ready to be played.",
                true,
                "Should slot into the projector."
            ));
            filmReelObtained = true;
        } else {
            std::cout << "You leave the box closed.\n";
        }
    };
    giftBox.addInteraction(giftBoxOpen);

    projectorRoom.addObject("teleporter", teleporter);
    projectorRoom.addObject("stand", stand);
    projectorRoom.addObject("door", projectorDoor);
    projectorRoom.addObject("gift box", giftBox);

    Area forgottenMaze(
        "forgotten_maze",
        "The Forgotten Maze",
        Dialogue(
            "AMAZING!, you actually did worse now than you did before. Scribbles.\n"
            "As you observe your surroundings, you notice that you're in a dark gloomy area. \"Wow it's actually dark in here. Maybe I should observe my surroundings once more to see what's really in here.\""
        )
    );
    forgottenMaze.addDoor(
        "east",
        Door(
            "Back to the Projector Room",
            "memories",
            "projector_room",
            false,
            ""
        )
    );
    forgottenMaze.addDoor(
        "north",
        Door(
            "Path to an Observatory",
            "memories",
            "observatory",
            true,
            "The maze hasn't revealed the way forward yet."
        )
    );

    InteractableObject mazePaths(Dialogue("The maze remembers your route now; a path north remains clear."));
    mazePaths.setInteractable(true);
    InteractableObject::Interaction mazeNavigate;
    mazeNavigate.id = "maze_navigate";
    mazeNavigate.dialogue = Dialogue("");
    mazeNavigate.singleUse = false;
    mazeNavigate.onUse = [this](Inventory& /*inv*/, AudioEngine*) {
        if (mazeSolved) {
            std::cout << "You remember the maze clearly. The path to the north stays open.\n";
            return;
        }

        if (mazeStage == 0) {
            std::cout << "As you try to squint your eyes you can see 3 paths that you can take.\n";
            std::cout << "-Take left path (1)\n";
            std::cout << "-Take middle path (2)\n";
            std::cout << "-Take right path (3)\n";
            std::string choice;
            std::getline(std::cin, choice);
            if (choice == "3") {
                std::cout << "As you take this path, you see a light ahead of you. As you approach the light you find yourself in a bright white room.\n";
                mazeStage = 1;
            } else {
                std::cout << "As you take this path, you feel as if you keep walking forever just to end up at the beginning. \"Weird, I could have sworn this was the right way.\"\n";
                mazeStage = 0;
            }
            return;
        }

        if (mazeStage == 1) {
            std::cout << "As you observe your surroundings, you notice within all the brightness 5 paths.\n";
            std::cout << "-Take left path (1)\n";
            std::cout << "-Take middle path (2)\n";
            std::cout << "-Take right path (3)\n";
            std::cout << "-Take east path (4)\n";
            std::cout << "-Take west path (5)\n";
            std::string choice;
            std::getline(std::cin, choice);
            if (choice == "1") {
                std::cout << "As you take this path, you find yourself in a different room. Seems like I took the right path.\n";
                mazeStage = 2;
            } else {
                std::cout << "As you take this path, you feel as if you keep walking forever just to end up at the beginning. \"Weird, I could have sworn this was the right way.\" And as you say that, you can hear laughs around you.\n";
                mazeStage = 0;
            }
            return;
        }

        if (mazeStage == 2) {
            std::cout << "As you observe your surroundings, you notice the room you're in is actually more like a hallway of some sort. You see 7 doors you can enter through.\n";
            std::cout << "-Take 1st door (1)\n";
            std::cout << "-Take 2nd door (2)\n";
            std::cout << "-Take 3rd door (3)\n";
            std::cout << "-Take 4th door (4)\n";
            std::cout << "-Take 5th door (5)\n";
            std::cout << "-Take 6th door (6)\n";
            std::cout << "-Take 7th door (7)\n";
            std::string choice;
            std::getline(std::cin, choice);
            if (choice == "4") {
                std::cout << "As you open this door and follow the path within, you find yourself in a different room. Seems like I took the right path.\n";
                mazeStage = 3;
            } else {
                std::cout << "As you open this door and go down the path, you feel as if you keep walking forever just to end up at the beginning. \"Weird, I could have sworn this was the right way.\" And as you say that, you can hear laughs around you feeling closer.\n";
                mazeStage = 0;
            }
            return;
        }

        if (mazeStage == 3) {
            std::cout << "As you observe your surroundings, you notice the room you're in is an observatory. Weirdly enough you don't see any telescopes around, but you notice 2 doors.\n";
            std::cout << "-Take the door to Telescope of Justice (1)\n";
            std::cout << "-Take the door to Telescope of Truth (2)\n";
            std::string choice;
            std::getline(std::cin, choice);
            if (choice == "2") {
                std::cout << "As you open this door and go down the path, you feel the room around you congratulate you for making it this far. Rewarding you with the Badge of Honor. But, you actually don't receive anything as the room doesn't have hands. So instead, you actually remembered the maze as a result.\n";
                std::cout << "Unlocked maze fast travel.\n";
                mazeSolved = true;
                unlockMazeExit();
                // Unlock the reward object visibility
                Area* a = getAreaMutable("forgotten_maze");
                if (a) {
                    if (auto* rewardObj = a->getObject("maze reward")) {
                        rewardObj->setInteractable(true);
                    }
                }
            } else {
                std::cout << "As you open this door and go down the path, you feel as if you keep walking forever just to end up at the beginning. \"Weird, I could have sworn this was the right way.\" And as you say that, you stopped hearing the laughs. After realizing this you hear from a distance call you a loser.\n";
                mazeStage = 0;
            }
            return;
        }
    };
    mazePaths.addInteraction(mazeNavigate);
    forgottenMaze.addObject("paths", mazePaths);

    InteractableObject mazeReward(Dialogue("The maze already rewarded you."));
    InteractableObject::Interaction mazeRewardUse;
    mazeRewardUse.id = "maze_reward";
    mazeRewardUse.dialogue = Dialogue("");
    mazeRewardUse.singleUse = false;
    mazeRewardUse.onUse = [this](Inventory& inv, AudioEngine*) {
        if (!mazeSolved) {
            std::cout << "Maybe solve the maze first; the room still feels confusing.\n";
            return;
        }
        if (!inv.hasItem(kTokenRiddleSelf)) {
            std::cout << "A calm silence fills the room, but nothing manifests. Perhaps something in the Final Chamber will hint at what to look for.\n";
            return;
        }
        if (symbolSelfObtained) {
            std::cout << "You already picked up the strange emblem left behind.\n";
            return;
        }
        std::cout << "In the calm after the maze, a soft glow appears on the floor. You pick up a small emblem.\n";
        std::cout << "You obtained Symbol of Self.\n";
        inv.addItem(Item(
            "Symbol of Self",
            "A stone emblem carved like a tiny silhouette with a faint halo.",
            true,
            "Found after accepting the maze's truth."
        ));
        symbolSelfObtained = true;
    };
    mazeReward.addInteraction(mazeRewardUse);
    forgottenMaze.addObject("maze reward", mazeReward);

    Area observatory(
        "observatory",
        "Observatory",
        Dialogue(
            "As you observe your surroundings, you notice that you're in an observatory with an actual telescope.\n"
            "-Telescope"
        )
    );
    observatory.addDoor(
        "east",
        Door(
            "Back to The Forgotten Maze",
            "memories",
            "forgotten_maze",
            false,
            ""
        )
    );
    observatory.addDoor(
        "north",
        Door(
            "Path to the 3rd Area",
            "alternative_routes",
            "entrance",
            true,
            "The telescope doesn't seem right yet."
        )
    );

    InteractableObject telescope(Dialogue("The telescope sits ready, pointed where it should be."));
    InteractableObject::Interaction telescopeUse;
    telescopeUse.id = "telescope_use";
    telescopeUse.dialogue = Dialogue("");
    telescopeUse.singleUse = false;
    telescopeUse.onUse = [this](Inventory& inv, AudioEngine*) {
        if (telescopeSolved) {
            if (!symbolMemoryObtained && inv.hasItem(kTokenRiddleMemory)) {
                std::cout << "The stars shimmer and drop a token near your feet.\n";
                std::cout << "You obtained Symbol of Memory.\n";
                inv.addItem(Item(
                    "Symbol of Memory",
                    "A stone circle engraved with a tiny projector beam and a faint constellation.",
                    true,
                    "Remember where you saw yourself."
                ));
                symbolMemoryObtained = true;
            } else {
                std::cout << "The telescope is aligned. The stars give you a thumbs up.\n";
            }
            return;
        }

        auto resetTelescope = [this]() {
            telescopeOrientationUp = false;
            std::cout << "You decide to just give up and reset the telescope, leaving it back to the way it originally was.\n";
        };

        auto markSolved = [this, &inv]() {
            std::cout << "You feel as if the telescope is properly pointing towards the night sky, as you feel accomplished you look into the telescope. And by looking through the telescope, you can see the stars align to what seems to be a thumbs up. Which results in a click sound as well as a door magically appearing.\n";
            telescopeSolved = true;
            solved = true;
            unlockThirdAreaDoor();
            if (!symbolMemoryObtained && inv.hasItem(kTokenRiddleMemory)) {
                std::cout << "The stars briefly form the shape of a small emblem and drop it by your feet.\n";
                inv.addItem(Item(
                    "Symbol of Memory",
                    "A stone circle engraved with a tiny projector beam and a faint constellation.",
                    true,
                    "Remember where you saw yourself."
                ));
                symbolMemoryObtained = true;
            }
        };

        while (true) {
            if (!telescopeOrientationUp) {
                std::cout << "As you approach the telescope you can't help but notice that the telescope isn't pointing towards the night sky. In fact it's actually pointing the opposite direction facing downwards. What do you want to do?\n";
                std::cout << "-Rotate the telescope 360 degrees (1)\n";
                std::cout << "-Rotate the telescope 180 degrees (2)\n";
                std::cout << "-Point the telescope upwards (3)\n";
                std::cout << "-Leave alone. (4)\n";
            } else {
                std::cout << "You move the telescope to point upwards. You also realize that the telescope still doesn't look right. What do you want to do?\n";
                std::cout << "-Rotate the telescope 360 degrees (1)\n";
                std::cout << "-Rotate the telescope 180 degrees (2)\n";
                std::cout << "-Point the telescope downwards (3)\n";
                std::cout << "-Reset. (4)\n";
            }

            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "4") {
                resetTelescope();
                return;
            }

            if (choice == "3") {
                telescopeOrientationUp = !telescopeOrientationUp;
                if (telescopeOrientationUp) {
                    std::cout << "You point the telescope upwards. It still doesn't feel quite right.\n";
                } else {
                    std::cout << "You point the telescope downwards. It still doesn't feel quite right.\n";
                }
                continue;
            }

            if (choice == "1") {
                std::cout << "You rotated the telescope 360 degrees. As you do so, you hear the person watching you say \"Clearly Subject #99 doesn't know their sense of direction.\" Scribbles. What do you want to do?\n";
                if (telescopeOrientationUp) {
                    std::cout << "-Rotate the telescope 180 degrees (1)\n";
                    std::cout << "-Point the telescope downward (2)\n";
                    std::cout << "-Reset (3)\n";
                    std::getline(std::cin, choice);
                    if (choice == "1") {
                        markSolved();
                        return;
                    } else if (choice == "2") {
                        telescopeOrientationUp = false;
                        continue;
                    } else {
                        resetTelescope();
                        return;
                    }
                } else {
                    std::cout << "-Rotate the telescope 180 degrees (1)\n";
                    std::cout << "-Point the telescope upward (2)\n";
                    std::cout << "-Reset (3)\n";
                    std::getline(std::cin, choice);
                    if (choice == "1") {
                        std::cout << "It spins and settles pointing downward again.\n";
                        continue;
                    } else if (choice == "2") {
                        telescopeOrientationUp = true;
                        continue;
                    } else {
                        resetTelescope();
                        return;
                    }
                }
            } else if (choice == "2") {
                if (telescopeOrientationUp) {
                    markSolved();
                    return;
                } else {
                    std::cout << "You rotate it 180 degrees, but it still isn't right.\n";
                    telescopeOrientationUp = true;
                }
            } else {
                std::cout << "You decide to step away from the telescope for now.\n";
                return;
            }
        }
    };
    telescope.addInteraction(telescopeUse);

    observatory.addObject("telescope", telescope);

    areas["projector_room"] = projectorRoom;
    areas["forgotten_maze"] = forgottenMaze;
    areas["observatory"] = observatory;
}
