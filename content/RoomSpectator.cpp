#include "RoomSpectator.h"
#include <iostream>
#include "../world/InteractableObject.h"
#include "../engine/Dialogue.h"

namespace {
const std::string kTokenProjector = "Token:ProjectorQuestStarted";
const std::string kTokenRiddleSocks = "Token:RiddleSocksHeard";
}

RoomSpectator::RoomSpectator()
    : Room(
        "spectator",
        "The Spectator's Trial",
        Dialogue(
            "Your eyes open to the sensation of being watched. Somewhere a foul stench and a locked door await."
        )
      )
{
    buildAreas();
}

std::string RoomSpectator::getStartAreaId() const {
    return "awakening_cell";
}

bool RoomSpectator::attemptFinalPuzzle(Inventory& /*inv*/,
                                       CraftingSystem& /*crafting*/) {
    // This room doesn't gate the overall progression; keep moving forward.
    return solved;
}

bool RoomSpectator::hasToken(const Inventory& inv, const std::string& name) const {
    return inv.hasItem(name);
}

void RoomSpectator::giveToken(Inventory& inv, const std::string& name,
                              const std::string& desc) {
    if (hasToken(inv, name)) return;
    inv.addItem(Item(
        name,
        desc,
        false,
        ""
    ));
}

void RoomSpectator::unlockAwakeningDoor() {
    if (doorUnlocked) return;
    Area* a = getArea("awakening_cell");
    if (!a) return;
    Door* d = a->getDoor("north");
    if (!d) return;
    d->unlock();
    doorUnlocked = true;
}

void RoomSpectator::openSockyExits() {
    Area* field = getArea("socky_field");
    if (!field) return;
    if (Door* east = field->getDoor("east")) {
        east->unlock();
    }
    if (Door* south = field->getDoor("south")) {
        south->unlock();
    }
}

void RoomSpectator::unlockCocoavilleExit() {
    Area* cocoaville = getArea("cocoaville");
    if (!cocoaville) return;
    if (Door* crawl = cocoaville->getDoor("north")) {
        crawl->unlock();
    }
}

void RoomSpectator::buildAreas() {
    Area awakening(
        "awakening_cell",
        "First Area - The Spectator",
        Dialogue(
            "Your eyes open, as they open you can't help but notice a door. Upon seeing the door, you start to get a whiff\n"
            "of something foul around, which shocks you enough to jolt you off a chair, weirdly enough you didn't seem to\n"
            "notice that you were sitting on the chair.\n\n"
            "As you observe your surroundings, you notice these around you.\n"
        )
    );

    awakening.addDoor(
        "north",
        Door(
            "Watchful Door",
            "spectator",
            "socky_field",
            true,
            "As I try to open the door, the door knob doesn't budge. It seems to be locked.",
            Dialogue("I feel as if the door is watching my every movement disappointedly.", "")
        )
    );

    InteractableObject chair(Dialogue("The chair still waits, but you've already given it enough attention."));
    InteractableObject::Interaction chairLook;
    chairLook.id = "chair_look";
    chairLook.dialogue = Dialogue(
        "This is a chair I was sitting on apparently, not sure why, but I feel as if the chair is staring back at me "
        "menacingly to sit back on it."
    );
    chairLook.singleUse = false;
    chair.addInteraction(chairLook);

    InteractableObject doorObject(Dialogue("The door stands open, creaking quietly."));
    InteractableObject::Interaction doorInspect;
    doorInspect.id = "door_inspect";
    doorInspect.dialogue = Dialogue("");
    doorInspect.singleUse = false;
    doorInspect.onUse = [this](Inventory& inv, AudioEngine*) {
        if (!doorUnlocked) {
            if (inv.hasItem("Key of Wisdom")) {
                std::cout << "You try to open the door and you remembered you picked up a door key.\n";
                std::cout << "You used the Key of Wisdom. Now the door is unlocked.\n";
                inv.removeItem("Key of Wisdom");
                unlockAwakeningDoor();
            } else {
                std::cout << "As I try to open the door, the door knob doesn't budge. It seems to be locked.\n";
            }
        } else {
            std::cout << "The door stands unlocked. North is my only option to go anywhere right now.\n";
            std::cout << "- Move North (Socky Field)\n";
        }
    };
    doorObject.addInteraction(doorInspect);

    InteractableObject sockPile(Dialogue("I already received what I wanted, not sure why I would submit myself again to torture."));
    InteractableObject::Interaction sockPileDig;
    sockPileDig.id = "sockpile_dig";
    sockPileDig.dialogue = Dialogue("");
    sockPileDig.singleUse = false;
    sockPileDig.onUse = [this](Inventory& inv, AudioEngine*) {
        if (!keyObtained) {
            std::cout << "So this is where that foul stench was coming from. Do I really want to search through this?\n";
            std::cout << "\t-Sure, why not. - (1)\n";
            std::cout << "\t-Nah, I'm good. - (2)\n";
            std::string choice;
            std::getline(std::cin, choice);
            if (choice == "1") {
                std::cout << "As you decide to search through the disgusting sock pile, you find a door key and a lot of socks.\n";
                if (!inv.hasItem("Key of Wisdom")) {
                    inv.addItem(Item(
                        "Key of Wisdom",
                        "A worn key that smells faintly of socks.",
                        true,
                        "Probably belongs to that watchful door."
                    ));
                }
                std::cout << "Obtained Key of Wisdom\n";
                keyObtained = true;
                sockpileProgress = 1;
            } else {
                std::cout << "You decide to leave the pile for now.\n";
            }
            return;
        }

        // Projector quest must be active (token) before deeper searches become meaningful.
        if (!hasToken(inv, kTokenProjector)) {
            std::cout << "You've already searched through this pile.\n";
            return;
        }

        // Second meaningful interaction: cable
        if (!cableObtained && sockpileProgress == 1) {
            std::cout << "You dig deeper and feel a cable buried under the socks. Pull it free? (y/n): ";
            std::string choice;
            std::getline(std::cin, choice);
            if (!choice.empty() && (choice[0] == 'y' || choice[0] == 'Y')) {
                std::cout << "You wrestle with the socks until a Power Cable pops free and slaps your face.\n";
                if (!inv.hasItem("Power Cable")) {
                    inv.addItem(Item(
                        "Power Cable",
                        "A tug-of-war championed cable, still dusty.",
                        true,
                        "Feels like it belongs to some machinery."
                    ));
                }
                cableObtained = true;
                sockpileProgress = 2;
            } else {
                std::cout << "You leave the buried cable where it is.\n";
            }
            return;
        }

        // Third meaningful interaction: Symbol of Socks unlocked only after statue riddle token.
        if (sockpileProgress >= 2 && hasToken(inv, kTokenRiddleSocks) && !symbolSocksObtained) {
            std::cout << "As you approach the Sock Pile again, you feel a familiar wave of disgust and deja vu. You dig past the dirty socks and half-remembered regrets and find something hard and oddly clean.\n";
            std::cout << "You obtained Symbol of Socks.\n";
            if (!inv.hasItem("Symbol of Socks")) {
                inv.addItem(Item(
                    "Symbol of Socks",
                    "A carved emblem of a sock with a tiny door key engraved on it.",
                    true,
                    "Proof of where you began."
                ));
            }
            symbolSocksObtained = true;
            sockpileProgress = 3;
            return;
        }

        if (!sockpostSeen || sockPlaced) {
            std::cout << "I already received what I wanted, not sure why I would submit myself again to torture.\n";
            return;
        }

        while (true) {
            std::cout << "As you revisit the Sock Pile you remembered that the Sockpost you saw earlier had a missing sock to be a proper post. You decide to search the Sock Pile in which you find\n";
            std::cout << "-Sock(1) - (1)\n";
            std::cout << "-Sock(B) - (2)\n";
            std::cout << "-Sock(@) - (3)\n";
            std::cout << "-Exit - (4)\n";
            std::cout << "Choose an option: ";
            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "2") {
                if (!sockCollected && !inv.hasItem("Sock(B)")) {
                    inv.addItem(Item(
                        "Sock(B)",
                        "A sock stitched with a bold letter B.",
                        true,
                        "Looks like it belongs on that Sockpost."
                    ));
                    sockCollected = true;
                }
                std::cout << "You pocket Sock(B) and try to ignore the smell.\n";
                return;
            } else if (choice == "1" || choice == "3") {
                std::cout << "I'm pretty sure it was not this sock. You decide to search the Sock Pile again...\n";
                return;
            } else if (choice == "4" || choice.empty()) {
                std::cout << "You leave the pile alone for now.\n";
                return;
            } else {
                std::cout << "Please pick 1, 2, 3, or 4.\n";
            }
        }
    };
    sockPile.addInteraction(sockPileDig);

    awakening.addObject("chair", chair);
    awakening.addObject("door", doorObject);
    awakening.addObject("sockpile", sockPile);

    Area sockyField(
        "socky_field",
        "Socky Field",
        Dialogue(
            "I SEE THAT YOU HAVE AWOKEN EXPERIMENT #99, TOOK YOU LONG ENOUGH!, anyways. I will be recording your every movement so don't mind me. If anything just think of me as your partner, though don't expect any help from me, unless you ask nicely then maybe.\n"
            "-You unlocked hint\n"
            "As you observe your surroundings, you notice the grass around you are actually socks with a bunch of soccer balls scattered all around. After getting distracted you notice...\n"
        )
    );

    sockyField.addDoor(
        "back",
        Door(
            "Back to the foul-smelling room",
            "spectator",
            "awakening_cell",
            false,
            ""
        )
    );
    sockyField.addDoor(
        "east",
        Door(
            "Portal toward Cocoaville",
            "spectator",
            "cocoaville",
            true,
            "The Sockpost portal stays dark until you impress it."
        )
    );
    sockyField.addDoor(
        "south",
        Door(
            "Path toward Mount Sockalossus",
            "spectator",
            "mount_sockalossus",
            true,
            "The spectator wants to see the Sockpost completed first."
        )
    );

    InteractableObject sockpost(Dialogue("The Sockpost looks like a proper goal now."));
    InteractableObject::Interaction sockpostInspect;
    sockpostInspect.id = "sockpost_inspect";
    sockpostInspect.dialogue = Dialogue("");
    sockpostInspect.singleUse = false;
    sockpostInspect.onUse = [this](Inventory& inv, AudioEngine*) {
        if (!sockPlaced) {
            sockpostSeen = true;
            if (inv.hasItem("Sock(B)")) {
                std::cout << "The sock magically disappears from your inventory. Wait inventory? Well whatever, the Sockpost looks like a proper Sockpost now I guess.\n";
                inv.removeItem("Sock(B)");
                sockPlaced = true;
            } else {
                std::cout << "It seems as if the Goalpost, no wait. Sockpost, is missing a sock to make it a Sockpost. You can faintly see that the sock has a letter on it.\n";
            }
            return;
        }
        std::cout << "The Sockpost stands complete, humming quietly.\n";
    };
    sockpost.addInteraction(sockpostInspect);

    InteractableObject soccerBall(Dialogue("The Sockpost portal hums softly."));
    InteractableObject::Interaction soccerBallKick;
    soccerBallKick.id = "soccer_ball";
    soccerBallKick.dialogue = Dialogue("");
    soccerBallKick.singleUse = false;
    soccerBallKick.onUse = [this](Inventory& /*inv*/, AudioEngine*) {
        if (!sockPlaced) {
            std::cout << "HEY DON'T YOU DARE GET NEAR THAT SOCCER BALL!\n";
            return;
        }
        if (!soccerPortalOpened) {
            std::cout << "\"Well there's only one thing to do with a Soccer Ball.\" You kick the Soccer Ball into the Sockpost. For some reason you feel as if the Sockpost gave your kick a 5 out 10. The Sockpost glows green and a portal appears.\n";
            soccerPortalOpened = true;
            openSockyExits();
            return;
        }
        std::cout << "The Sockpost glows green and the portal remains open.\n";
    };
    soccerBall.addInteraction(soccerBallKick);

    InteractableObject shiny(Dialogue("The socks rustle quietly; the shiny thing is gone."));
    InteractableObject::Interaction shinyGrab;
    shinyGrab.id = "shiny_grab";
    shinyGrab.dialogue = Dialogue("");
    shinyGrab.singleUse = false;
    shinyGrab.onUse = [this](Inventory& inv, AudioEngine*) {
        if (!hasToken(inv, kTokenProjector)) {
            std::cout << "Just socks and soccer balls. Nothing catches your eye yet.\n";
            return;
        }
        if (lensObtained) {
            std::cout << "You've already pocketed the lens here.\n";
            return;
        }
        std::cout << "As you get near the shiny object, you notice that it seems to resemble a lens. You don't know for what, but you pick it up anyways.\n";
        std::cout << "Obtained Lens.\n";
        inv.addItem(Item(
            "Lens",
            "A clear lens that belongs to something mechanical.",
            true,
            "Maybe part of that projector stand."
        ));
        lensObtained = true;
    };
    shiny.addInteraction(shinyGrab);

    sockyField.addObject("sockpost", sockpost);
    sockyField.addObject("soccer ball", soccerBall);
    sockyField.addObject("shiny object", shiny);

    Area cocoaville(
        "cocoaville",
        "Cocoaville",
        Dialogue(
            "As you observe your surroundings, you notice the ground you're standing on is replaced with cocoa powder whilst the whole room is very Christmas themed.\n"
            "-Fireplace\n"
            "-Cup of cocoa\n"
            "-Christmas Tree"
        )
    );

    cocoaville.addDoor(
        "west",
        Door(
            "Back to Socky Field",
            "spectator",
            "socky_field",
            false,
            ""
        )
    );
    cocoaville.addDoor(
        "north",
        Door(
            "Crawlspace to the 2nd Area",
            "memories",
            "projector_room",
            true,
            "The fireplace heat blocks the crawlspace."
        )
    );

    InteractableObject tree(Dialogue("You already admired the presents under the tree."));
    InteractableObject::Interaction treeAdmire;
    treeAdmire.id = "tree_admire";
    treeAdmire.dialogue = Dialogue("");
    treeAdmire.singleUse = false;
    treeAdmire.onUse = [this](Inventory& inv, AudioEngine*) {
        if (!hasToken(inv, kTokenProjector)) {
            std::cout << "As you get near the Christmas tree, you notice a bunch of presents lying around. You don't know who the presents are for, so you decide to leave them alone and admire them from afar.\n";
            return;
        }
        if (giftBoxObtained) {
            std::cout << "The presents look slightly lighter now that you've pilfered one.\n";
            return;
        }
        std::cout << "As you get near the Christmas tree, you can't help but notice a bunch of presents lying around for god knows how long. You don't know who the presents are for. So you decide to\n";
        std::cout << "-Take one. (1)\n";
        std::cout << "-Leave them alone. (2)\n";
        std::string choice;
        std::getline(std::cin, choice);
        if (choice == "1") {
            std::cout << "You have obtained a Gift box.\n";
            inv.addItem(Item(
                "Gift Box",
                "A box wrapped for someone else. Probably fine.",
                true,
                "Maybe there's a part inside."
            ));
            giftBoxObtained = true;
        } else {
            std::cout << "You leave the presents untouched.\n";
        }
    };
    tree.addInteraction(treeAdmire);

    InteractableObject fireplace(Dialogue("The fireplace is silent now, revealing the opening north."));
    InteractableObject::Interaction fireplaceInt;
    fireplaceInt.id = "fireplace";
    fireplaceInt.dialogue = Dialogue("");
    fireplaceInt.singleUse = false;
    fireplaceInt.onUse = [this](Inventory& inv, AudioEngine*) {
        if (fireplaceCleared) {
            std::cout << "The fire is out and an opening waits; you can crawl through to the north.\n";
            return;
        }

        if (inv.hasItem("Cup of Cocoa")) {
            std::cout << "As you get near the fireplace, you can hear the cup of cocoa hissing. You then throw the cup of cocoa at the fireplace out of fear that it might bite you. Wait bite me?\n";
            std::cout << "\nThe fire then dies out and you can see an opening which you can crawl into.\n";
            inv.removeItem("Cup of Cocoa");
            fireplaceCleared = true;
            unlockCocoavilleExit();
        } else {
            std::cout << "As you get near the fireplace, the fire grows fiercely. And for a moment it felt as if the fire was shooing me away. Scribbles. \"Clearly subject #99 doesn't know what fire is.\"\n";
        }
    };
    fireplace.addInteraction(fireplaceInt);

    InteractableObject cocoa(Dialogue("The Cup of cocoa has already been claimed."));
    InteractableObject::Interaction cocoaChoice;
    cocoaChoice.id = "cocoa_choice";
    cocoaChoice.dialogue = Dialogue("");
    cocoaChoice.singleUse = false;
    cocoaChoice.onUse = [this](Inventory& inv, AudioEngine*) {
        if (cocoaTaken) {
            std::cout << "You already pocketed the Cup of Cocoa.\n";
            return;
        }

        std::cout << "As you approach the Cup of cocoa, the Cup of cocoa stares back intensely. You decide to\n";
        std::cout << "1. Take a sip\n";
        std::cout << "2. Throw hands\n";
        std::cout << "3. Pick up cup of cocoa\n";
        std::cout << "Choice: ";
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "You take a sip. The cocoa judges you silently.\n";
        } else if (choice == "2") {
            std::cout << "You square up to fight the cocoa. It doesn't flinch.\n";
        } else if (choice == "3") {
            std::cout << "You obtained a Cup of Cocoa.\n";
            inv.addItem(Item(
                "Cup of Cocoa",
                "Warm, rich, and seemingly alive.",
                true,
                "Feels like it might calm a fire."
            ));
            cocoaTaken = true;
        } else {
            std::cout << "You leave the Cup of cocoa alone for now.\n";
        }
    };
    cocoa.addInteraction(cocoaChoice);

    cocoaville.addObject("fireplace", fireplace);
    cocoaville.addObject("cup of cocoa", cocoa);
    cocoaville.addObject("christmas tree", tree);

    Area mountSockalossus(
        "mount_sockalossus",
        "Mount Sockalossus",
        Dialogue(
            "A looming mountain of socks towers overhead. The spectator scribbles notes for future trials here."
        )
    );
    mountSockalossus.addDoor(
        "north",
        Door(
            "Trail back to Socky Field",
            "spectator",
            "socky_field",
            false,
            ""
        )
    );

    areas["awakening_cell"] = awakening;
    areas["socky_field"] = sockyField;
    areas["cocoaville"] = cocoaville;
    areas["mount_sockalossus"] = mountSockalossus;
}
