#include "Area.h"
#include <iostream>

#include "../engine/AudioEngine.h"

void Area::observe(AudioEngine* audioEngine) const {
    std::cout << "=== " << displayName << " ===\n";
    description.deliver(audioEngine);
    std::cout << "\n";

    // list objects
    auto objectNames = getVisibleObjectNames();
    if (!objectNames.empty()) {
        std::cout << "You notice:\n";
        for (size_t idx = 0; idx < objectNames.size(); ++idx) {
            std::cout << " " << (idx + 1) << ". " << objectNames[idx] << "\n";
        }
    } else {
        std::cout << "Nothing else here looks interactive.\n";
    }
    std::cout << std::endl;

    // list doors
    auto doorDirections = getDoorDirections();
    if (!doorDirections.empty()) {
        std::cout << "Exits:\n";
        for (size_t idx = 0; idx < doorDirections.size(); ++idx) {
            const auto& dir = doorDirections[idx];
            const auto& d = doors.at(dir);
            std::cout << " " << (idx + 1) << ". " << dir << " -> " << d.getName();
            if (d.locked()) {
                if (d.getLockedDialogue().has_value()) {
                    std::cout << " [LOCKED: " << d.getLockedDialogue()->getText() << "]";
                } else {
                    std::cout << " [LOCKED: " << d.getLockedText() << "]";
                }
            }
            std::cout << "\n";
        }
    }
}

void Area::interact(const std::string& objectName,
                    Inventory& inv,
                    AudioEngine* audioEngine) {
    auto it = objects.find(objectName);
    if (it == objects.end()) {
        std::cout << "There's nothing called '" << objectName << "' here.\n";
        return;
    }

    InteractableObject& object = it->second;
    if (!object.isInteractable()) {
        std::cout << "It doesn't seem interactive right now.\n";
        return;
    }

    object.interact(inv, audioEngine);
}

void Area::addDoor(const std::string& direction, const Door& d) {
    doors[direction] = d;
}

Door* Area::getDoor(const std::string& direction) {
    auto it = doors.find(direction);
    if (it == doors.end()) return nullptr;
    return &(it->second);
}

void Area::addObject(const std::string& objectName, const InteractableObject& object) {
    objects[objectName] = object;
}

void Area::addObject(const std::string& objectName,
                     const std::string& objectDesc,
                     const Item& rewardItem,
                     bool singleUse) {
    addObject(objectName, Dialogue(objectDesc), rewardItem, singleUse);
}

void Area::addObject(const std::string& objectName,
                     const Dialogue& dialogue,
                     const Item& rewardItem,
                     bool singleUse) {
    InteractableObject object(Dialogue("You've already searched that."));
    InteractableObject::Interaction interaction;
    interaction.id = objectName + "_default";
    interaction.dialogue = dialogue;
    if (!rewardItem.getName().empty()) {
        interaction.reward = rewardItem;
    }
    interaction.singleUse = singleUse;
    interaction.requiresFixed = false;
    interaction.requiresBroken = false;
    interaction.startsLocked = false;
    object.addInteraction(interaction);

    addObject(objectName, object);
}

void Area::setObjectDialogue(const std::string& objectName, const Dialogue& dlg) {
    auto it = objects.find(objectName);
    if (it == objects.end()) return;
    it->second.setInteractionDialogue(objectName + "_default", dlg);
}

std::vector<std::string> Area::getVisibleObjectNames() const {
    std::vector<std::string> names;
    for (const auto& kv : objects) {
        const auto& objName = kv.first;
        const auto& object = kv.second;
        if (object.isInteractable()) {
            names.push_back(objName);
        }
    }
    return names;
}

std::vector<std::string> Area::getDoorDirections() const {
    std::vector<std::string> dirs;
    for (const auto& kv : doors) {
        dirs.push_back(kv.first);
    }
    return dirs;
}

InteractableObject* Area::getObject(const std::string& name) {
    auto it = objects.find(name);
    if (it == objects.end()) return nullptr;
    return &(it->second);
}
