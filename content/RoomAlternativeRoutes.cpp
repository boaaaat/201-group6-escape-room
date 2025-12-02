#include "RoomAlternativeRoutes.h"
#include <iostream>
#include <algorithm>
#include "../engine/Dialogue.h"
#include "../world/InteractableObject.h"

namespace {
const std::string kTokenRiddlePaths = "Token:RiddlePathsHeard";
}

namespace {
bool hasItem(const Inventory& inv, const std::string& name) {
    return inv.hasItem(name);
}
}

RoomAlternativeRoutes::RoomAlternativeRoutes()
    : Room(
        "alternative_routes",
        "Alternative Routes",
        Dialogue(
            "As you step into this area, a chill runs down your spine. The air smells like... nostalgia? Regret? Burnt toast? Hard to tell.\n"
            "SPECTATOR:\n"
            "\"Welcome to the Alternative Routes, Subject #99. A special place where we store all the accomplishments you didn't achieve. No offense, probably.\" Scribbles. \"Try not to cry.\""
        )
      )
{
    buildAreas();
}

std::string RoomAlternativeRoutes::getStartAreaId() const {
    return "entrance";
}

bool RoomAlternativeRoutes::attemptFinalPuzzle(Inventory& /*inv*/,
                                               CraftingSystem& /*crafting*/) {
    if (adminExitUnlocked) {
        solved = true;
        return true;
    }
    std::cout << "The Admin Office still wants its exhibits restored.\n";
    return false;
}

void RoomAlternativeRoutes::markHallVisited() {
    visitedAnyHall = true;
}

void RoomAlternativeRoutes::tryUnlockAdminExit() {
    if (adminExitUnlocked) return;
    if (!ballPlaced || !mapPlaced || !cocoaPlaced) return;

    adminExitUnlocked = true;
    solved = true;
    Area* office = getArea("admin_office");
    if (office) {
        if (Door* exitDoor = office->getDoor("north")) {
            exitDoor->unlock();
        }
    }
    std::cout << "A triumphant chime echoes across the entire area.\n";
    std::cout << "SPECTATOR:\n";
    std::cout << "\"Congrats, Subject #99. You finally cleaned up after yourself.\n";
    std::cout << "Try not to faint from the responsibility.\"\n";
}

void RoomAlternativeRoutes::buildAreas() {
    Area entrance(
        "entrance",
        "Crossroads of Could-Have-Beens",
        Dialogue(
            "As you observe your surroundings, you notice a grand hallway with three glowing doors up ahead and one rusty metal door labeled ADMIN OFFICE in faded marker.\n"
            "You see:\n"
            "Door: Hall of Fame - The Athlete\n\n"
            "Door: Hall of Fame - The Adventurer\n\n"
            "Door: Hall of Fame - The Cocoa Connoisseur\n\n"
            "Rusty Admin Door (locked)\n\n"
            "Old Bulletin Board"
        )
    );

    entrance.addDoor(
        "north",
        Door(
            "Hall of Fame - The Athlete",
            "alternative_routes",
            "hall_athlete",
            false,
            ""
        )
    );
    entrance.addDoor(
        "east",
        Door(
            "Hall of Fame - The Adventurer",
            "alternative_routes",
            "hall_adventurer",
            false,
            ""
        )
    );
    entrance.addDoor(
        "west",
        Door(
            "Hall of Fame - The Cocoa Connoisseur",
            "alternative_routes",
            "hall_cocoa",
            false,
            ""
        )
    );
    entrance.addDoor(
        "south",
        Door(
            "Return to the 2nd Area",
            "memories",
            "observatory",
            false,
            ""
        )
    );
    entrance.addDoor(
        "admin",
        Door(
            "Rusty Admin Door",
            "alternative_routes",
            "admin_office",
            true,
            "ACCESS REVOKED. PLEASE REPORT DISPLAY THEFT BEFORE CONTINUING."
        )
    );

    InteractableObject bulletin(Dialogue("The bulletin board creaks softly; the memos still flutter."));
    InteractableObject::Interaction boardRead;
    boardRead.id = "board_read";
    boardRead.dialogue = Dialogue("");
    boardRead.singleUse = false;
    boardRead.onUse = [this](Inventory& inv, AudioEngine*) {
        std::cout << "The bulletin board is covered in overdue memos, half-erased doodles, and a sticky note:\n";
        std::cout << "\"TO WHOEVER KEEPS STEALING DISPLAY ITEMS - RETURN THEM OR ELSE. - Management\"\n";
        std::cout << "Scribbles. \"Oh wow, somebody here is even more strict than me.\"\n";

        if (!mapPieceCFound) {
            std::cout << "A loose wood plank sticks out near the base. Pull it? (y/n): ";
            std::string choice;
            std::getline(std::cin, choice);
            if (!choice.empty() && (choice[0] == 'y' || choice[0] == 'Y')) {
                if (!hasItem(inv, "Map Piece C")) {
                    inv.addItem(Item(
                        "Map Piece C",
                        "A torn fragment of a map with frayed edges.",
                        true,
                        "Combine with the other pieces."
                    ));
                }
                mapPieceCFound = true;
                std::cout << "You pry up the plank and find Map Piece C.\n";
            } else {
                std::cout << "You leave the plank alone.\n";
            }
        }

        if (visitedAnyHall && !ballObtained) {
            std::cout << "A faint glow appears behind the bulletin board. Investigate? (y/n): ";
            std::string choice;
            std::getline(std::cin, choice);
            if (!choice.empty() && (choice[0] == 'y' || choice[0] == 'Y')) {
                if (!hasItem(inv, "Worn Soccer Ball")) {
                    inv.addItem(Item(
                        "Worn Soccer Ball",
                        "A scuffed ball still echoing with imaginary cheers.",
                        true,
                        "Return it to the Athlete hall pedestal."
                    ));
                }
                ballObtained = true;
                std::cout << "You obtain the Worn Soccer Ball.\n";
            } else {
                std::cout << "The glow fades for now.\n";
            }
        }
    };
    bulletin.addInteraction(boardRead);
    entrance.addObject("bulletin board", bulletin);

    InteractableObject adminDoorObj(Dialogue("The Admin Door awaits proper exhibit returns."));
    InteractableObject::Interaction adminDoorInteract;
    adminDoorInteract.id = "admin_door";
    adminDoorInteract.dialogue = Dialogue("");
    adminDoorInteract.singleUse = false;
    adminDoorInteract.onUse = [this](Inventory& /*inv*/, AudioEngine*) {
        Area* a = getArea("entrance");
        if (!a) return;
        Door* d = a->getDoor("admin");
        if (!d) return;
        if (!d->locked()) {
            std::cout << "The Admin Door creaks, already unlocked.\n";
            return;
        }
        std::cout << "The Admin Door rattles but doesn't budge.\n";
        std::cout << "A metallic voice states:\n";
        std::cout << "\"ACCESS REVOKED. PLEASE REPORT DISPLAY THEFT BEFORE CONTINUING.\"\n";
        std::cout << "The latch grudgingly clicks open anyway, as if it expects you to fix things.\n";
        d->unlock();
    };
    adminDoorObj.addInteraction(adminDoorInteract);
    entrance.addObject("admin door", adminDoorObj);

    Area hallAthlete(
        "hall_athlete",
        "Hall of Fame - The Athlete",
        Dialogue(
            "You enter a stadium-like room with spotlights, echoing cheers, and confetti falling for someone who... isn't you."
        )
    );
    hallAthlete.addDoor(
        "south",
        Door(
            "Back to the Entrance",
            "alternative_routes",
            "entrance",
            false,
            ""
        )
    );

    InteractableObject athletePedestal(Dialogue("The pedestal glows with the returned glory of the Worn Soccer Ball."));
    InteractableObject::Interaction athletePlace;
    athletePlace.id = "athlete_place";
    athletePlace.dialogue = Dialogue("");
    athletePlace.singleUse = false;
    athletePlace.onUse = [this](Inventory& inv, AudioEngine*) {
        markHallVisited();
        if (ballPlaced) {
            std::cout << "The stadium roars again in your memory. The ball rests proudly.\n";
            return;
        }
        if (!hasItem(inv, "Worn Soccer Ball")) {
            std::cout << "The plaque reads:\n";
            std::cout << "\"Here lies the legendary Worn Soccer Ball used in the Unbelievable Match.\"\n";
            std::cout << "Except the pedestal is empty... unlike your resume.\n";
            std::cout << "SPECTATOR: \"Oh look, something missing... just like your teamwork skills.\"\n";
            return;
        }
        inv.removeItem("Worn Soccer Ball");
        ballPlaced = true;
        std::cout << "As you place the Worn Soccer Ball, the stadium roars with imaginary applause.\n";
        std::cout << "The spotlight shines green.\n";
        tryUnlockAdminExit();
    };
    athletePedestal.addInteraction(athletePlace);

    InteractableObject trophyCase(Dialogue("The trophy case remains locked and empty, mocking you silently."));
    InteractableObject::Interaction trophyInspect;
    trophyInspect.id = "trophy_inspect";
    trophyInspect.dialogue = Dialogue("You jiggle the trophy case. Locked. Figures.");
    trophyInspect.singleUse = false;
    trophyCase.addInteraction(trophyInspect);

    InteractableObject bleachers(Dialogue("The bleachers look no more interesting than before."));
    InteractableObject::Interaction bleacherSearch;
    bleacherSearch.id = "bleacher_search";
    bleacherSearch.dialogue = Dialogue(
        "You find a concession wrapper, a foam finger, and a depressing echo. Nothing else."
    );
    bleacherSearch.singleUse = false;
    bleachers.addInteraction(bleacherSearch);

    InteractableObject spotlight(Dialogue("The spotlight keeps shining. Stage fright optional."));
    InteractableObject::Interaction spotlightStep;
    spotlightStep.id = "spotlight_step";
    spotlightStep.dialogue = Dialogue(
        "You step into the spotlight. Nothing happens except your crippling stage fright returning."
    );
    spotlightStep.singleUse = false;
    spotlight.addInteraction(spotlightStep);

    hallAthlete.addObject("pedestal", athletePedestal);
    hallAthlete.addObject("trophy case", trophyCase);
    hallAthlete.addObject("bleachers", bleachers);
    hallAthlete.addObject("spotlight", spotlight);

    Area hallAdventurer(
        "hall_adventurer",
        "Hall of Fame - The Adventurer",
        Dialogue(
            "You step into a dimly lit explorer's hall, complete with jungle vines, fake torches, and dramatic wind sounds that are definitely just a fan."
        )
    );
    hallAdventurer.addDoor(
        "west",
        Door(
            "Back to the Entrance",
            "alternative_routes",
            "entrance",
            false,
            ""
        )
    );

    InteractableObject mapStand(Dialogue("The Map glows on its stand, finally whole."));
    InteractableObject::Interaction mapPlace;
    mapPlace.id = "map_place";
    mapPlace.dialogue = Dialogue("");
    mapPlace.singleUse = false;
    mapPlace.onUse = [this](Inventory& inv, AudioEngine*) {
        markHallVisited();
        if (mapPlaced) {
            std::cout << "The torches flare softly; the map already rests here.\n";
            return;
        }

        bool hasA = hasItem(inv, "Map Piece A");
        bool hasB = hasItem(inv, "Map Piece B");
        bool hasC = hasItem(inv, "Map Piece C");
        int pieceCount = static_cast<int>(hasA) + static_cast<int>(hasB) + static_cast<int>(hasC);

        if (pieceCount < 3) {
            if (pieceCount > 0) {
                std::cout << "You place the piece on the stand, but the stand asks: \"Insert remaining fragments.\" How demanding.\n";
            } else {
                std::cout << "The Map Display Stand is empty. SPECTATOR: \"Wow, you were supposed to be a famous explorer. Now you're exploring... my patience.\"\n";
            }
            return;
        }

        inv.removeItem("Map Piece A");
        inv.removeItem("Map Piece B");
        inv.removeItem("Map Piece C");
        mapPlaced = true;
        std::cout << "They fuse together magically. The display sparkles. You hear a distant click.\n";
        std::cout << "The torches flare up. Wind dramatically blows your hair.\n";
        std::cout << "SPECTATOR: \"Wow. Overly dramatic, much?\"\n";
        tryUnlockAdminExit();
    };
    mapStand.addInteraction(mapPlace);

    InteractableObject vines(Dialogue("The vines sway gently now."));
    InteractableObject::Interaction vinesShake;
    vinesShake.id = "vines_shake";
    vinesShake.dialogue = Dialogue("");
    vinesShake.singleUse = false;
    vinesShake.onUse = [this](Inventory& inv, AudioEngine*) {
        markHallVisited();
        if (!mapPieceAFound) {
            mapPieceAFound = true;
            if (!hasItem(inv, "Map Piece A")) {
                inv.addItem(Item(
                    "Map Piece A",
                    "A torn fragment of a map with jungle scribbles.",
                    true,
                    "Combine with other pieces."
                ));
            }
            std::cout << "The vines slightly shake. Something drops. You obtain Map Piece A.\n";
        } else {
            std::cout << "The vines have nothing else to give.\n";
        }
    };
    vines.addInteraction(vinesShake);

    InteractableObject boulder(Dialogue("The boulder sits right back where it was—unimpressed."));
    InteractableObject::Interaction boulderPush;
    boulderPush.id = "boulder_push";
    boulderPush.dialogue = Dialogue("");
    boulderPush.singleUse = false;
    boulderPush.onUse = [this](Inventory& inv, AudioEngine*) {
        markHallVisited();
        if (!mapPieceBFound) {
            mapPieceBFound = true;
            if (!hasItem(inv, "Map Piece B")) {
                inv.addItem(Item(
                    "Map Piece B",
                    "A map fragment hidden under faux rock.",
                    true,
                    "Combine with other pieces."
                ));
            }
            std::cout << "You push the giant boulder. It moves exactly one inch. Underneath is Map Piece B.\n";
        } else {
            std::cout << "You've already claimed the fragment beneath the boulder.\n";
        }
    };
    boulder.addInteraction(boulderPush);

    InteractableObject sand(Dialogue("The sand gets everywhere. Literally."));
    InteractableObject::Interaction sandObs;
    sandObs.id = "sand_obs";
    sandObs.dialogue = Dialogue("Coarse, rough, irritating, and it gets everywhere. Nothing useful here.");
    sandObs.singleUse = false;
    sand.addInteraction(sandObs);

    hallAdventurer.addObject("map stand", mapStand);
    hallAdventurer.addObject("vines", vines);
    hallAdventurer.addObject("boulder", boulder);
    hallAdventurer.addObject("sand", sand);

    Area hallCocoa(
        "hall_cocoa",
        "Hall of Fame - The Cocoa Connoisseur",
        Dialogue(
            "You enter a cozy but over-the-top artisan cocoa museum with walls decorated in cocoa art and \"world's best hot cocoa maker\" medals."
        )
    );
    hallCocoa.addDoor(
        "east",
        Door(
            "Back to the Entrance",
            "alternative_routes",
            "entrance",
            false,
            ""
        )
    );

    InteractableObject cauldron(Dialogue("The cauldron bubbles happily, filled with returned cocoa."));
    InteractableObject::Interaction cauldronUse;
    cauldronUse.id = "cauldron_use";
    cauldronUse.dialogue = Dialogue("");
    cauldronUse.singleUse = false;
    cauldronUse.onUse = [this](Inventory& inv, AudioEngine*) {
        markHallVisited();
        if (cocoaPlaced) {
            std::cout << "The cauldron steams contentedly. The exhibit is restored.\n";
            return;
        }
        if (!hasItem(inv, "Expired Cocoa")) {
            std::cout << "The cauldron is empty... and disappointed.\n";
            return;
        }
        inv.removeItem("Expired Cocoa");
        cocoaPlaced = true;
        std::cout << "The cauldron bubbles happily. A cocoa-scented mist fills the air. You feel oddly proud.\n";
        tryUnlockAdminExit();
    };
    cauldron.addInteraction(cauldronUse);

    InteractableObject mixer(Dialogue("The mixer spins idly, satisfied now."));
    InteractableObject::Interaction mixerSpin;
    mixerSpin.id = "mixer_spin";
    mixerSpin.dialogue = Dialogue("The Cocoa Mixer Machine hums, but you've got bigger priorities.");
    mixerSpin.singleUse = false;
    mixer.addInteraction(mixerSpin);

    InteractableObject mugs(Dialogue("The pyramid rebuilds itself again; still looks fragile."));
    InteractableObject::Interaction mugsTap;
    mugsTap.id = "mugs_tap";
    mugsTap.dialogue = Dialogue("");
    mugsTap.singleUse = false;
    mugsTap.onUse = [this](Inventory& inv, AudioEngine*) {
        markHallVisited();
        if (cocoaObtained) {
            std::cout << "You tap the pyramid. It collapses and rebuilds itself aggressively. No more cocoa inside.\n";
            return;
        }
        std::cout << "You tap the pyramid. It collapses and rebuilds itself aggressively. A mug launches at you. It contains Expired Cocoa.\n";
        cocoaObtained = true;
        if (!hasItem(inv, "Expired Cocoa")) {
            inv.addItem(Item(
                "Expired Cocoa",
                "A mug of cocoa that's... past its prime.",
                true,
                "Return it to the display cauldron."
            ));
        }
    };
    mugs.addInteraction(mugsTap);

    InteractableObject lamp(Dialogue("The heating lamp glows warmly now."));
    InteractableObject::Interaction lampTouch;
    lampTouch.id = "lamp_touch";
    lampTouch.dialogue = Dialogue("");
    lampTouch.singleUse = false;
    lampTouch.onUse = [this](Inventory& /*inv*/, AudioEngine*) {
        markHallVisited();
        if (!cocoaPlaced) {
            std::cout << "It flickers intensely as if judging your incompetence.\n";
        } else {
            std::cout << "The lamp approves of the restored cocoa.\n";
        }
    };
    lamp.addInteraction(lampTouch);

    hallCocoa.addObject("display cauldron", cauldron);
    hallCocoa.addObject("cocoa mixer", mixer);
    hallCocoa.addObject("mug pyramid", mugs);
    hallCocoa.addObject("heating lamp", lamp);

    Area adminOffice(
        "admin_office",
        "The Admin Office",
        Dialogue(
            "The Admin Office is a cramped workplace with paperwork mountains, angry sticky notes, and zero windows.\n"
            "SPECTATOR:\n"
            "\"Okay, Subject #99. Time to do actual responsibilities.\""
        )
    );
    adminOffice.addDoor(
        "south",
        Door(
            "Back to the Entrance",
            "alternative_routes",
            "entrance",
            false,
            ""
        )
    );
    adminOffice.addDoor(
        "north",
        Door(
            "Staircase to the Fourth Area",
            "final_chamber",
            "central",
            true,
            "ACCESS DENIED. Please restore stolen items to continue."
        )
    );

    InteractableObject fileCabinet(Dialogue("You rummage through paperwork. It's all budget cuts and sock requisitions."));
    InteractableObject::Interaction fileSearch;
    fileSearch.id = "file_search";
    fileSearch.dialogue = Dialogue("Nothing but old memos and coffee stains.");
    fileSearch.singleUse = false;
    fileCabinet.addInteraction(fileSearch);

    InteractableObject missingLog(Dialogue("You've already memorized the Missing Items Log."));
    InteractableObject::Interaction logRead;
    logRead.id = "log_read";
    logRead.dialogue = Dialogue("");
    logRead.singleUse = false;
    logRead.onUse = [this](Inventory& /*inv*/, AudioEngine*) {
        if (!adminLogRead) {
            adminLogRead = true;
            std::cout << "You read:\n";
            std::cout << "\"Three artifacts have been stolen from the Halls:\n";
            std::cout << "Worn Soccer Ball\n\n";
            std::cout << "The Map (three fragments)\n\n";
            std::cout << "Expired Cocoa\n";
            std::cout << "Return all items to their proper exhibits to restore order.\"\n\n";
            std::cout << "SPECTATOR:\n";
            std::cout << "\"Translation: Put the stuff back where it belongs, genius.\"\n";
        } else {
            std::cout << "The log still demands the three artifacts be returned.\n";
        }
    };
    missingLog.addInteraction(logRead);

    InteractableObject loudspeaker(Dialogue("The loudspeaker crackles, bored of you already."));
    InteractableObject::Interaction speakerUse;
    speakerUse.id = "speaker_use";
    speakerUse.dialogue = Dialogue("It emits a dismissive beep. Rude.");
    speakerUse.singleUse = false;
    loudspeaker.addInteraction(speakerUse);

    InteractableObject adminReward(Dialogue("The desk looks tidier now."));
    InteractableObject::Interaction adminRewardUse;
    adminRewardUse.id = "admin_reward";
    adminRewardUse.dialogue = Dialogue("");
    adminRewardUse.singleUse = false;
    adminRewardUse.onUse = [this](Inventory& inv, AudioEngine*) {
        if (!adminExitUnlocked) {
            std::cout << "Paperwork still clutters the desk. Maybe restore the exhibits first.\n";
            return;
        }
        if (!inv.hasItem(kTokenRiddlePaths)) {
            std::cout << "The desk sits quietly. Perhaps something in the Final Chamber will hint at what to look for.\n";
            return;
        }
        if (symbolPathsObtained) {
            std::cout << "Nothing else of value sits on the desk.\n";
            return;
        }
        std::cout << "The Admin Office feels quieter now. On the desk where the Missing Items Log sat, you see a new emblem.\n";
        std::cout << "You obtained Symbol of Paths.\n";
        inv.addItem(Item(
            "Symbol of Paths",
            "A stone disc engraved with three branching lines converging in the center.",
            true,
            "Awarded for cleaning up your alternate routes."
        ));
        symbolPathsObtained = true;
    };
    adminReward.addInteraction(adminRewardUse);

    adminOffice.addObject("file cabinet", fileCabinet);
    adminOffice.addObject("missing items log", missingLog);
    adminOffice.addObject("loudspeaker", loudspeaker);
    adminOffice.addObject("admin reward", adminReward);

    areas["entrance"] = entrance;
    areas["hall_athlete"] = hallAthlete;
    areas["hall_adventurer"] = hallAdventurer;
    areas["hall_cocoa"] = hallCocoa;
    areas["admin_office"] = adminOffice;
}
