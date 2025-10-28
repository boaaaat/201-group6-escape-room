#include "RoomLostSocks.h"
#include <iostream>

RoomLostSocks::RoomLostSocks(CraftingSystem& craftRef)
    : Room(
        "lost_socks",
        "Lobby of Lost Socks",
        "You step into a warm, sock-scented dimension. Piles of single socks float and whisper.\n"
        "A glowing laundry basket blocks what looks like an exit portal."
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
    pairRecipe.resultDesc  = "Both Socks of Truth finally reunited. They hum in harmony.";
    pairRecipe.resultTip   = "The laundry basket only respects properly matched love.";
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
        std::cout << "BASKET: \"You dare challenge me WITHOUT a matched pair?\"\n";
        std::cout << "BASKET: \"Return when your socks know true love.\"\n";
        return false;
    }

    // run Scramble puzzle:
    bool solved = gatePuzzle.play();
    if (!solved) {
        // wrong answer, still locked
        return false;
    }

    std::cout << "The glowing laundry basket vibrates with approval.\n";
    std::cout << "\"Proceed... and may your pairs be ever complete.\"\n";

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
        "A hill of tangled socks from every known universe. Some of them hum breakup songs."
    );
    sockMountain.addObject(
        "sockpile",
        "You dig into the sock pile. The air smells like static and heartbreak.\n"
        "A lonely sock wriggles into your hand, whispering: \"I miss Righty...\"",
        Item(
            "Sock of Truth (L)",
            "A left sock buzzing with cosmic static. It vibrates with desperate loyalty.",
            true,
            "Probably needs its partner. It won't feel 'complete' alone."
        ),
        /*singleUse=*/true
    );
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
        "An ancient office desk covered in lint and snack crumbs. A sticky note reads:\n"
        "\"KCSO -> SOCK. Remember this.\""
    );
    fuzzyDesk.addObject(
        "desk",
        "You rummage through the fuzzy drawers.\n"
        "You find: Sock of Truth (R). It says \"Did Lefty ask about me?\"",
        Item(
            "Sock of Truth (R)",
            "A right sock pulsing with insecure energy. It just wants to be loved.",
            true,
            "If reunited with Lefty, something powerful might happen."
        ),
        /*singleUse=*/true
    );

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
            "A corner where a dryer hums like a miniature wormhole",
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
        "A dryer shaped like a tiny black hole. A glowing laundry basket floats in front of\n"
        "a shimmering EXIT PORTAL. The basket stares at you without eyes."
    );

    dryerPortal.addObject(
        "basket",
        "The basket booms:\n"
        "\"ONLY MATCHED LOVE MAY PASS. SPELL YOUR WORTH.\"\n"
        "Yeah, it's dramatic.",
        Item("", "", false, ""),
        /*singleUse=*/false
    );

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
            "The laundry basket blocks the way, demanding PROOF OF MATCHING."
        )
    );

    areas["sock_mountain"] = sockMountain;
    areas["fuzzy_desk"] = fuzzyDesk;
    areas["dryer_portal"] = dryerPortal;
}
