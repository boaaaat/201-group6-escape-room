#pragma once
#include <map>
#include <optional>
#include <string>
#include <vector>
#include <functional>
#include "../engine/Dialogue.h"
#include "../systems/Inventory.h"

class AudioEngine;

// InteractableObject models richer object behavior than the simple
// "dialogue + single reward" pattern:
//  - can start broken and require items to repair
//  - can expose multiple interactions that unlock one another
//  - can gate interactions on fixed/broken state
//  - can optionally hand out reward items
class InteractableObject {
public:
    struct Interaction {
        std::string id;                     // unique name for later unlocking/locking
        Dialogue dialogue;                  // what to say when this interaction fires
        std::optional<Item> reward;         // optional item to grant
        bool singleUse = true;              // consume after firing once
        bool requiresFixed = false;         // only usable when repaired
        bool requiresBroken = false;        // only usable while broken
        bool startsLocked = false;          // needs unlockInteraction() or unlocksOnUse to enable
        std::optional<Dialogue> lockedDialogue; // optional message when interaction is still locked
        std::vector<std::string> unlocksOnUse;  // ids to unlock after this runs
        std::function<void(Inventory&, AudioEngine*)> onUse; // optional callback for custom effects
    };

    InteractableObject(const Dialogue& exhaustedDialogue = Dialogue("You've already done that here."),
                       bool interactable = true);

    // Add another interaction to this object.
    void addInteraction(const Interaction& interaction);

    // Toggle locks programmatically (or via unlocksOnUse)
    void unlockInteraction(const std::string& id);
    void lockInteraction(const std::string& id);
    void setInteractionDialogue(const std::string& id, const Dialogue& dialogue);

    // Mark the object as broken and specify how to repair it.
    // itemsNeeded is a map of itemName -> count required to repair.
    // If consumeItemsOnRepair is false, the required items are just checked, not removed.
    void markBroken(const Dialogue& brokenDialogue,
                    const std::map<std::string, int>& itemsNeeded,
                    const Dialogue& repairSuccessDialogue = Dialogue("You manage to repair it."),
                    bool consumeItemsOnRepair = true);

    // Perform an interaction given the current state and inventory.
    bool interact(Inventory& inventory, AudioEngine* audioEngine = nullptr);

    bool isInteractable() const;
    bool isBroken() const { return broken; }
    bool hasAvailableInteraction() const;
    void setInteractable(bool value) { interactable = value; }

private:
    struct InteractionState {
        Interaction data;
        bool unlocked = true;
        bool used = false;
    };

    void runInteraction(InteractionState& interaction,
                        Inventory& inventory,
                        AudioEngine* audioEngine);
    bool attemptRepair(Inventory& inventory, AudioEngine* audioEngine);
    int countItem(const Inventory& inventory, const std::string& itemName) const;
    InteractionState* findAvailableInteraction();
    Dialogue* firstLockedDialogue();

    bool interactable;
    bool broken = false;
    bool consumeRepairItems = true;
    Dialogue brokenDialogue;
    Dialogue repairSuccessDialogue;
    Dialogue exhaustedDialogue;
    std::map<std::string, int> repairRequirements;
    std::vector<InteractionState> interactions;
};
