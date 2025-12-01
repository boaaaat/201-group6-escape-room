#include "RoomLostSocks.h"
#include <iostream>
#include "../engine/Dialogue.h"
#include "../world/InteractableObject.h"

RoomLostSocks::RoomLostSocks(CraftingSystem& craftRef)
    : Room(
        "lost_socks",
        "Lobby of Lost Socks",
        Dialogue("You step into a warm, sock-scented dimension. Piles of single socks float and whisper.\n"
        "A glowing laundry basket blocks what looks like an exit portal.", "speech\\RoomLostSocks\\room_intro.mp3")
      ),
      gatePuzzle({
          {"KCSO",  "SOCK"},
          {"HCTAM", "MATCH"},
          {"RIAP",  "PAIR"}
      }),
      craftingRef(craftRef)
{
    // Teach recipe for Matched Pair at room creation
    CraftRecipe pairRecipe;
    pairRecipe.ingredients = { "Sock of Truth (L)", "Sock of Truth (R)" };
    pairRecipe.resultName  = "Matched Pair";
    pairRecipe.resultDesc  = "A pair of matching socks.";
    pairRecipe.resultTip   = "The laundry basket only respects properly matched socks.";
    craftingRef.addRecipe(pairRecipe);

    buildAreas();
}

std::string RoomLostSocks::getStartAreaId() const {
    return "sock_mountain";
}

bool RoomLostSocks::attemptFinalPuzzle(Inventory& inv,
                                       CraftingSystem& crafting) {
    // require the crafted "Matched Pair"
    if (!inv.hasItem("Matched Pair")) {
        std::cout << "BASKET: \"You dare challenge me WITHOUT a \"Matched Pair\"?!\n";
        std::cout << "BASKET: \"Do not return until you have proven yourself.\"\n";
        return false;
    }

    // run Scramble puzzle:
    bool solved = gatePuzzle.play();
    if (!solved) {
        // wrong answer, still locked
        return false;
    }

    std::cout << "The laundry basket shimmers with light brightly.\n";
    std::cout << "\"You can now proceed.\"\n";

    solved = true;
    this->solved = true;

    // unlock exit door
    unlockExitDoor();

    return true;
}

void RoomLostSocks::unlockExitDoor() {
    // We know the exit door lives in dryer_portal, direction "exit"
    Area* a = getArea("dryer_portal");
    if (!a) return;
    Door* d = a->getDoor("exit");
    if (!d) return;
    d->unlock();
}

// buildAreas constructs the Areas and their doors/objects.
void RoomLostSocks::buildAreas() {
    // sock_mountain
    Area sockMountain(
        "sock_mountain",
        "Mount Sockmore",
        Dialogue("A hill of tangled socks with a variety of different colors.")
    );

    InteractableObject sockPile(Dialogue("You've already disturbed this part of the pile."));
    InteractableObject::Interaction digPile;
    digPile.id = "sockpile_dig";
    digPile.dialogue = Dialogue(
        "You dig into the sockpile.\n"
        "As you dig into the sockpile, a sock wriggles into your hand, whispering: \"I miss Righty...\""
    );
    digPile.reward = Item(
        "Sock of Truth (L)",
        "A left sock with a very interesting color palette...",
        true,
        "Is it just me or did this sock just whisper me something about missing a Righty?"
    );
    digPile.singleUse = true;
    sockPile.addInteraction(digPile);
    sockMountain.addObject("sockpile", sockPile);
    sockMountain.addDoor(
        "east",
        Door(
            "Path toward a fuzzy desk covered in crumbs",
            "lost_socks",
            "fuzzy_desk",
            false,
            ""
        )
    );

    // fuzzy_desk
    Area fuzzyDesk(
        "fuzzy_desk",
        "The Fuzzy Desk",
        Dialogue("An ancient office desk covered in dust and snack crumbs. A sticky note reads:\n"
        "\"KCSO -> SOCK. Remember this.\"")
    );

    InteractableObject desk(Dialogue("You already emptied the drawers here."));
    InteractableObject::Interaction clearDesk;
    clearDesk.id = "desk_clear";
    clearDesk.dialogue = Dialogue(
        "You brush aside the crumbs and sticky notes. A drawer handle loosens-maybe rummage again."
    );
    clearDesk.singleUse = true;
    clearDesk.unlocksOnUse = { "desk_rummage" };
    desk.addInteraction(clearDesk);

    InteractableObject::Interaction rummageDesk;
    rummageDesk.id = "desk_rummage";
    rummageDesk.dialogue = Dialogue(
        "You rummage through the fuzzy drawers.\n"
        "You find: Sock of Truth (R). Maybe there is a left one..."
    );
    rummageDesk.reward = Item(
        "Sock of Truth (R)",
        "A right sock with a interesting color palette.",
        true,
        "If reunited with Lefty, something powerful might happen."
    );
    rummageDesk.singleUse = true;
    rummageDesk.startsLocked = true;
    desk.addInteraction(rummageDesk);

    fuzzyDesk.addObject("desk", desk);

    fuzzyDesk.addDoor(
        "west",
        Door(
            "Back toward Mount Sockmore",
            "lost_socks",
            "sock_mountain",
            false,
            ""
        )
    );
    fuzzyDesk.addDoor(
        "south",
        Door(
            "A corner where a loud vacuum sound is coming from.",
            "lost_socks",
            "dryer_portal",
            false,
            ""
        )
    );

    // dryer_portal
    Area dryerPortal(
        "dryer_portal",
        "Dryer Portal Corner",
        Dialogue("A dryer shaped like a tiny black hole. A glowing laundry basket floats in front of\n"
        "a shimmering EXIT PORTAL. The basket stares at you menancingly.")
    );

    InteractableObject basket(Dialogue("The basket hovers quietly, waiting for you to prove yourself."));
    basket.markBroken(
        Dialogue(
            "The basket vibrates impatiently. It refuses to listen until it sees perfectly matched socks."
        ),
        { { "Matched Pair", 1 } },
        Dialogue(
            "The basket glows, satisfied by your matching prowess. It's willing to hear you out now."
        ),
        /*consumeItemsOnRepair=*/false
    );

    InteractableObject::Interaction basketChallenge;
    basketChallenge.id = "basket_challenge";
    basketChallenge.dialogue = Dialogue(
        "The basket booms:\n"
        "\"ONLY MATCHES MAY PASS. SPELL YOUR WORTH.\"\n"
        "Yeah, it's dramatic."
    );
    basketChallenge.singleUse = false;
    basketChallenge.requiresFixed = true;
    basket.addInteraction(basketChallenge);

    dryerPortal.addObject("basket", basket);

    dryerPortal.addDoor(
        "north",
        Door(
            "Back to the fuzzy desk",
            "lost_socks",
            "fuzzy_desk",
            false,
            ""
        )
    );
    dryerPortal.addDoor(
        "exit",
        Door(
            "Portal to The Pantry of Misplaced Snacks",
            "pantry",          // <- next big room id
            "pantry_entry",    // <- starting area in that room
            true,
            "The laundry basket blocks the way, demanding PROOF OF MATCHING.",
            Dialogue("The laundry basket blocks the way, demanding PROOF OF MATCHING.", "")
        )
    );

    areas["sock_mountain"] = sockMountain;
    areas["fuzzy_desk"] = fuzzyDesk;
    areas["dryer_portal"] = dryerPortal;
}
