#include "RoomFinalChamber.h"
#include <iostream>
#include "../engine/Dialogue.h"
#include "../world/InteractableObject.h"

#include "../systems/Inventory.h"

namespace {
const std::string kTokenRiddleSocks = "Token:RiddleSocksHeard";
const std::string kTokenRiddleMemory = "Token:RiddleMemoryHeard";
const std::string kTokenRiddlePaths = "Token:RiddlePathsHeard";
const std::string kTokenRiddleSelf = "Token:RiddleSelfHeard";
}

RoomFinalChamber::RoomFinalChamber()
    : Room(
        "final_chamber",
        "Where All Routes Meet",
        Dialogue(
            "You step through the golden door and find yourself in a vast, circular chamber.\n"
            "Four massive statues stand evenly spaced along the walls, each facing inward toward the center.\n"
            "The floor is carved with symbols from everywhere you've been, and the air feels... heavier. Final.\n"
            "SPECTATOR:\n"
            "\"Well, Subject #99... you've actually made it this far.\n"
            "This is the Climax\u2122. Try not to mess it up. I'm low on paper for my notes.\""
        )
      )
{
    buildAreas();
}

std::string RoomFinalChamber::getStartAreaId() const {
    return "central";
}

bool RoomFinalChamber::attemptFinalPuzzle(Inventory& /*inv*/,
                                          CraftingSystem& /*crafting*/) {
    return solved;
}

void RoomFinalChamber::checkAllPlaced() {
    if (finalDoorAppeared) return;
    if (!socksPlaced || !memoryPlaced || !pathsPlaced || !selfPlaced) return;
    finalDoorAppeared = true;
    std::cout << "A triumphant chime echoes. The four runes around the central platform flare in unison.\n";
    std::cout << "A massive, ornate door appears above the platform and settles gently onto the ground.\n";
    std::cout << "SPECTATOR: \"Well, look at that. You actually collected yourself. Literally. You know what comes next, right?\"\n";
}

void RoomFinalChamber::buildAreas() {
    Area chamber(
        "central",
        "Final Chamber",
        Dialogue(
            "As you observe your surroundings, you notice:\n"
            "Statue of Socks\n"
            "Statue of Memory\n"
            "Statue of Paths\n"
            "Statue of Self\n"
            "Empty circular platform in the center of the room\n"
            "SPECTATOR:\n"
            "\"Each statue wants its matching symbol. All from your little field trip through Areas 1, 2, and 3.\n"
            "Go on, revisit your past mistakes. I'll be here. Judging.\""
        )
    );

    InteractableObject statueSocks(Dialogue("The statue of Socks glows faintly."));
    InteractableObject::Interaction socksUse;
    socksUse.id = "socks_use";
    socksUse.dialogue = Dialogue("");
    socksUse.singleUse = false;
    socksUse.onUse = [this](Inventory& inv, AudioEngine*) {
        if (socksPlaced) {
            std::cout << "The Symbol of Socks rests firmly in the statue.\n";
            return;
        }
        if (!inv.hasItem("Symbol of Socks")) {
            std::cout << "The statue's empty eyes glow faintly.\n";
            std::cout << "\"I am the stench of beginnings,\nA pile of what you tried to ignore.\nI watched you search through filth and fabric,\nWhere did you first unlock the door?\"\n";
            std::cout << "SPECTATOR: \"Mm, yes. Nothing says character development like digging through a disgusting sock pile.\"\n";
            if (!inv.hasItem(kTokenRiddleSocks)) {
                inv.addItem(Item(
                    kTokenRiddleSocks,
                    "You heard the Sock statue riddle in the Final Chamber.",
                    false,
                    ""
                ));
            }
            return;
        }
        std::cout << "You hold up the Symbol of Socks. The statue's chest indent glows softly.\n";
        std::cout << "Where did you find this symbol?\n";
        std::cout << "1) In the Sock Pile of the first room (Area 1 - Introduction)\n";
        std::cout << "2) In the maze\n";
        std::cout << "3) In the Admin Office\n";
        std::cout << "4) In Cocoaville\n";
        std::string choice;
        std::getline(std::cin, choice);
        if (choice != "1") {
            std::cout << "The glow fades. \"Incorrect. You still run from the truth of where it began.\"\n";
            std::cout << "SPECTATOR: \"Oof. Even the statue is disappointed.\"\n";
            return;
        }
        inv.removeItem("Symbol of Socks");
        socksPlaced = true;
        std::cout << "The statue glows bright green as the Symbol of Socks sinks into its chest.\n";
        std::cout << "\"You remember well where you first chose to dig. The path of discomfort... is where you awakened.\"\n";
        checkAllPlaced();
    };
    statueSocks.addInteraction(socksUse);

    InteractableObject statueMemory(Dialogue("The statue of Memory hums quietly."));
    InteractableObject::Interaction memoryUse;
    memoryUse.id = "memory_use";
    memoryUse.dialogue = Dialogue("");
    memoryUse.singleUse = false;
    memoryUse.onUse = [this](Inventory& inv, AudioEngine*) {
        if (memoryPlaced) {
            std::cout << "The Symbol of Memory projects a faint beam of starlight.\n";
            return;
        }
        if (!inv.hasItem("Symbol of Memory")) {
            std::cout << "The statue's eyes shimmer like a dim projection.\n";
            std::cout << "\"I show you who you were,\nIn flickering light and silent sky.\nI needed three pieces to awaken,\nAnd pointed you toward yourself on high.\nReturn with the mark of what you remembered.\"\n";
            std::cout << "SPECTATOR: \"Oh, this one is dramatic. It's talking about when you actually did something right with that projector, remember?\"\n";
            if (!inv.hasItem(kTokenRiddleMemory)) {
                inv.addItem(Item(
                    kTokenRiddleMemory,
                    "You heard the Memory statue riddle in the Final Chamber.",
                    false,
                    ""
                ));
            }
            return;
        }
        std::cout << "You carry the Symbol of Memory. Where did you reclaim this memory?\n";
        std::cout << "1) In the Telescope Room (Observatory, 3rd room Area 2)\n";
        std::cout << "2) In the Socky Fields\n";
        std::cout << "3) In the Admin Office\n";
        std::cout << "4) In the Hall of Fame - Athlete\n";
        std::string choice;
        std::getline(std::cin, choice);
        if (choice != "1") {
            std::cout << "The film reel stops spinning. \"The memory slips through your fingers. That is not where it was truly found.\"\n";
            std::cout << "SPECTATOR: \"Wrong again. You're really speedrunning disappointment.\"\n";
            return;
        }
        inv.removeItem("Symbol of Memory");
        memoryPlaced = true;
        std::cout << "The Symbol of Memory sinks into the statue's chest. Its eyes project a tiny beam of starlight across the room.\n";
        std::cout << "\"You faced the sky and saw yourself within it. That truth now anchors you here.\"\n";
        checkAllPlaced();
    };
    statueMemory.addInteraction(memoryUse);

    InteractableObject statuePaths(Dialogue("The statue of Paths waits, arms slowly flexing."));
    InteractableObject::Interaction pathsUse;
    pathsUse.id = "paths_use";
    pathsUse.dialogue = Dialogue("");
    pathsUse.singleUse = false;
    pathsUse.onUse = [this](Inventory& inv, AudioEngine*) {
        if (pathsPlaced) {
            std::cout << "The Symbol of Paths glows softly on the statue.\n";
            return;
        }
        if (!inv.hasItem("Symbol of Paths")) {
            std::cout << "The statue speaks in a layered echo, like multiple versions of you talking at once.\n";
            std::cout << "\"I am what might have been,\nAll routes branching and twisting away.\nI watched your halls of praise and shame,\nAnd the office where you owned your mistakes.\nBring me the symbol of the routes you could have taken.\"\n";
            std::cout << "SPECTATOR: \"Yeah, this one's about all the alternate lives where you actually got your stuff together. Wild concept.\"\n";
            if (!inv.hasItem(kTokenRiddlePaths)) {
                inv.addItem(Item(
                    kTokenRiddlePaths,
                    "You heard the Paths statue riddle in the Final Chamber.",
                    false,
                    ""
                ));
            }
            return;
        }
        std::cout << "Where did you accept the routes you did and did not take?\n";
        std::cout << "1) In the Admin Office (3rd Area)\n";
        std::cout << "2) In Cocoaville\n";
        std::cout << "3) In the Forgotten Maze\n";
        std::cout << "4) In the first room with the chair\n";
        std::string choice;
        std::getline(std::cin, choice);
        if (choice != "1") {
            std::cout << "The arms freeze in place. \"That is not where you faced the record of your choices.\"\n";
            std::cout << "SPECTATOR: \"You really want it to be the cocoa room, huh?\"\n";
            return;
        }
        inv.removeItem("Symbol of Paths");
        pathsPlaced = true;
        std::cout << "The Symbol of Paths sinks into the statue. Its arms fold into a calm, centered pose.\n";
        std::cout << "\"You looked at what was missing, and chose to restore it. The path you walk now is no accident.\"\n";
        checkAllPlaced();
    };
    statuePaths.addInteraction(pathsUse);

    InteractableObject statueSelf(Dialogue("The statue of Self stands quietly, almost breathing."));
    InteractableObject::Interaction selfUse;
    selfUse.id = "self_use";
    selfUse.dialogue = Dialogue("");
    selfUse.singleUse = false;
    selfUse.onUse = [this](Inventory& inv, AudioEngine*) {
        if (selfPlaced) {
            std::cout << "The Symbol of Self sits in the statue's heart, pulsing softly.\n";
            return;
        }
        if (!inv.hasItem("Symbol of Self")) {
            std::cout << "The statue's stone lips move.\n";
            std::cout << "\"All routes converge here.\nThe socks, the film, the cocoa, the halls.\nBut I am not made of any of those.\nI am where you truly stood,\nWhen you realized this was never real at all.\nBring me the symbol of who you are in this place.\"\n";
            std::cout << "SPECTATOR: \"Open-book quiz: where did you realize this is all... off?\"\n";
            if (!inv.hasItem(kTokenRiddleSelf)) {
                inv.addItem(Item(
                    kTokenRiddleSelf,
                    "You heard the Self statue riddle in the Final Chamber.",
                    false,
                    ""
                ));
            }
            return;
        }
        std::cout << "You bring the Symbol of Self. Where did you truly see yourself?\n";
        std::cout << "1) In the Telescope of Truth room at the end of the maze\n";
        std::cout << "2) In the Sock Pile\n";
        std::cout << "3) In the Admin Office\n";
        std::cout << "4) In the Cup of Cocoa\n";
        std::string choice;
        std::getline(std::cin, choice);
        if (choice != "1") {
            std::cout << "The statue's eyes dim. \"That is where you were distracted, not where you woke up.\"\n";
            std::cout << "SPECTATOR: \"Imagine saying you 'found yourself' in a sock pile. Tragic.\"\n";
            return;
        }
        inv.removeItem("Symbol of Self");
        selfPlaced = true;
        std::cout << "The Symbol of Self merges into the statue's chest. The stone figure exhales, as if freed.\n";
        std::cout << "\"You walked in circles, then stepped into the light and saw who was watching. Now you may choose what to do with that truth.\"\n";
        checkAllPlaced();
    };
    statueSelf.addInteraction(selfUse);

    InteractableObject finalDoor(Dialogue("The ornate door waits in the center."));
    InteractableObject::Interaction finalDoorUse;
    finalDoorUse.id = "final_door";
    finalDoorUse.dialogue = Dialogue("");
    finalDoorUse.singleUse = false;
    finalDoorUse.onUse = [this](Inventory& /*inv*/, AudioEngine*) {
        if (!finalDoorAppeared) {
            std::cout << "An empty circular platform hums faintly. Perhaps you must satisfy all statues first.\n";
            return;
        }
        std::cout << "The door speaks in a calm, echoing voice.\n";
        std::cout << "\"You have walked through socks, memories, and possibilities.\n"
                     "You have seen what you were, what you could have been,\n"
                     "and what you believed about yourself.\n"
                     "One question remains.\"\n";
        std::cout << "\"What is this place, truly?\"\n";
        std::cout << "1) A test facility\n";
        std::cout << "2) A game\n";
        std::cout << "3) A dream\n";
        std::cout << "4) A punishment\n";
        std::string choice;
        std::getline(std::cin, choice);
        if (choice != "3") {
            std::cout << "The door hums softly. \"You still cling to the story that hurts you the most. Try again, once you are ready.\"\n";
            std::cout << "SPECTATOR: \"Wow. Door said 'self-awareness check failed'.\"\n";
            return;
        }
        std::cout << "The patterns on the door begin to swirl. The carved closed eyes slowly open.\n";
        std::cout << "\"Correct. You are not in a facility, a maze, or a trial.\n"
                     "You are in your own dream, Subject #99.\n"
                     "One last choice remains.\"\n";
        std::cout << "\"Now that you know this is a dream...\n"
                     "what would you like to do, to finish it?\"\n";
        std::cout << "1) Stay here forever\n";
        std::cout << "2) Keep exploring\n";
        std::cout << "3) Forget everything\n";
        std::cout << "4) Wake up\n";
        std::getline(std::cin, choice);
        if (choice != "4") {
            std::cout << "The door sighs. \"You may wander a little longer, then return when you are ready.\"\n";
            std::cout << "SPECTATOR: \"A bold choice. Wrong, but bold.\"\n";
            return;
        }
        std::cout << "The entire chamber begins to dissolve into soft light. The statues fade. The symbols, the halls, the maze, the socks... all blur into mist.\n";
        std::cout << "SPECTATOR: \"Well. That's that, then. No more experiments, no more scribbles. You finally picked the route you never dared to choose.\"\n";
        std::cout << "Their voice starts to distort, growing distant.\n";
        std::cout << "\"Good morning, Subject #99... or should I finally stop calling you that?\"\n";
        std::cout << "The floor beneath you vanishes. You feel weightless. Everything goes white.\n";
        solved = true;
    };
    finalDoor.addInteraction(finalDoorUse);

    chamber.addObject("statue of socks", statueSocks);
    chamber.addObject("statue of memory", statueMemory);
    chamber.addObject("statue of paths", statuePaths);
    chamber.addObject("statue of self", statueSelf);
    chamber.addObject("final door", finalDoor);

    areas["central"] = chamber;
}
