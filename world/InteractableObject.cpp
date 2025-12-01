#include "InteractableObject.h"
#include <algorithm>
#include <iostream>
#include "../engine/AudioEngine.h"

InteractableObject::InteractableObject(const Dialogue& exhaustedDialogue,
                                       bool interactableFlag)
    : interactable(interactableFlag),
      broken(false),
      consumeRepairItems(true),
      brokenDialogue(Dialogue("")),
      repairSuccessDialogue(Dialogue("You manage to repair it.")),
      exhaustedDialogue(exhaustedDialogue) {}

void InteractableObject::addInteraction(const Interaction& interaction) {
    InteractionState state;
    state.data = interaction;
    state.unlocked = !interaction.startsLocked;
    state.used = false;
    interactions.push_back(state);
}

void InteractableObject::unlockInteraction(const std::string& id) {
    for (auto& state : interactions) {
        if (state.data.id == id) {
            state.unlocked = true;
        }
    }
}

void InteractableObject::lockInteraction(const std::string& id) {
    for (auto& state : interactions) {
        if (state.data.id == id) {
            state.unlocked = false;
        }
    }
}

void InteractableObject::setInteractionDialogue(const std::string& id,
                                                const Dialogue& dialogue) {
    for (auto& state : interactions) {
        if (state.data.id == id) {
            state.data.dialogue = dialogue;
            return;
        }
    }
    // Fall back to updating the first interaction if the id was not found.
    if (!interactions.empty()) {
        interactions.front().data.dialogue = dialogue;
    }
}

void InteractableObject::markBroken(const Dialogue& brokenDlg,
                                    const std::map<std::string, int>& itemsNeeded,
                                    const Dialogue& repairSuccessDlg,
                                    bool consumeItemsOnRepair) {
    broken = true;
    repairRequirements = itemsNeeded;
    brokenDialogue = brokenDlg;
    repairSuccessDialogue = repairSuccessDlg;
    consumeRepairItems = consumeItemsOnRepair;
}

bool InteractableObject::interact(Inventory& inventory, AudioEngine* audioEngine) {
    if (!interactable) {
        std::cout << "You can't interact with that right now.\n";
        return false;
    }

    // While broken, prefer any interactions that are allowed in the broken state.
    if (broken) {
        InteractionState* availableWhileBroken = findAvailableInteraction();
        if (availableWhileBroken) {
            runInteraction(*availableWhileBroken, inventory, audioEngine);
            return true;
        }

        bool repaired = attemptRepair(inventory, audioEngine);
        if (!repaired) {
            if (!brokenDialogue.getText().empty()) {
                brokenDialogue.deliver(audioEngine);
            } else {
                std::cout << "It looks broken. You need specific items to fix it.\n";
            }
            return false;
        }
    }

    // At this point the object is either not broken or has been repaired.
    InteractionState* interaction = findAvailableInteraction();
    if (!interaction) {
        Dialogue* lockedDlg = firstLockedDialogue();
        if (lockedDlg) {
            lockedDlg->deliver(audioEngine);
            return false;
        }

        if (!exhaustedDialogue.getText().empty()) {
            exhaustedDialogue.deliver(audioEngine);
        } else {
            std::cout << "Nothing else seems interesting here.\n";
        }
        return false;
    }

    runInteraction(*interaction, inventory, audioEngine);
    return true;
}

bool InteractableObject::isInteractable() const {
    if (!interactable) return false;
    if (broken) return true; // allow attempts to repair

    bool hasLocked = std::any_of(
        interactions.begin(), interactions.end(),
        [](const InteractionState& state) { return !state.unlocked; });

    return hasAvailableInteraction() || hasLocked;
}

bool InteractableObject::hasAvailableInteraction() const {
    if (!interactable) return false;
    for (const auto& state : interactions) {
        if (!state.unlocked) continue;
        if (state.used && state.data.singleUse) continue;
        if (state.data.requiresFixed && broken) continue;
        if (state.data.requiresBroken && !broken) continue;
        return true;
    }
    return false;
}

void InteractableObject::runInteraction(InteractionState& interaction,
                                        Inventory& inventory,
                                        AudioEngine* audioEngine) {
    interaction.data.dialogue.deliver(audioEngine);
    if (interaction.data.reward.has_value()) {
        const auto& reward = interaction.data.reward.value();
        std::cout << "You obtain: " << reward.getName() << "\n";
        inventory.addItem(reward);
    }

    if (interaction.data.singleUse) {
        interaction.used = true;
    }

    for (const auto& id : interaction.data.unlocksOnUse) {
        unlockInteraction(id);
    }
}

bool InteractableObject::attemptRepair(Inventory& inventory, AudioEngine* audioEngine) {
    if (!broken) return true;
    if (repairRequirements.empty()) return false;

    // Check requirements
    bool hasAll = true;
    for (const auto& req : repairRequirements) {
        int have = countItem(inventory, req.first);
        if (have < req.second) {
            hasAll = false;
        }
    }
    if (!hasAll) {
        std::cout << "You need to repair this with:\n";
        for (const auto& req : repairRequirements) {
            int have = countItem(inventory, req.first);
            int missing = std::max(0, req.second - have);
            std::cout << " - " << req.first << " x" << req.second;
            if (missing > 0) {
                std::cout << " (missing " << missing << ")";
            }
            std::cout << "\n";
        }
        return false;
    }

    if (consumeRepairItems) {
        for (const auto& req : repairRequirements) {
            for (int i = 0; i < req.second; ++i) {
                inventory.removeItem(req.first);
            }
        }
    }

    broken = false;
    if (!repairSuccessDialogue.getText().empty()) {
        repairSuccessDialogue.deliver(audioEngine);
    }
    return true;
}

int InteractableObject::countItem(const Inventory& inventory,
                                  const std::string& itemName) const {
    int count = 0;
    for (const auto& item : inventory.getItems()) {
        if (item.getName() == itemName) {
            ++count;
        }
    }
    return count;
}

InteractableObject::InteractionState* InteractableObject::findAvailableInteraction() {
    for (auto& state : interactions) {
        if (!state.unlocked) continue;
        if (state.used && state.data.singleUse) continue;
        if (state.data.requiresFixed && broken) continue;
        if (state.data.requiresBroken && !broken) continue;
        return &state;
    }
    return nullptr;
}

Dialogue* InteractableObject::firstLockedDialogue() {
    for (auto& state : interactions) {
        if (state.unlocked) continue;
        if (state.data.lockedDialogue.has_value()) {
            return &(state.data.lockedDialogue.value());
        }
    }
    return nullptr;
}
