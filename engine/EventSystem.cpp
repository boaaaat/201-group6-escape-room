#include "EventSystem.h"
#include "AudioEngine.h"

void EventSystem::registerCommandEvent(const std::string& areaId,
                                       const std::string& command,
                                       const Dialogue& dialogue,
                                       bool fireOnce) {
    commandEvents.push_back({
        toLower(areaId),
        toLower(command),
        dialogue,
        fireOnce,
        false
    });
}

void EventSystem::handleCommand(const std::string& areaId,
                                const std::string& command,
                                AudioEngine* audioEngine) {
    const std::string areaLower = toLower(areaId);
    const std::string cmdLower = toLower(command);

    for (auto& evt : commandEvents) {
        if (evt.fireOnce && evt.alreadyFired) continue;

        bool areaMatch = (evt.areaId == "*" || evt.areaId == areaLower);
        bool cmdMatch = (evt.command == "*" || evt.command == cmdLower);
        if (!areaMatch || !cmdMatch) continue;

        evt.dialogue.deliver(audioEngine, false);
        if (evt.fireOnce) {
            evt.alreadyFired = true;
        }
    }
}
