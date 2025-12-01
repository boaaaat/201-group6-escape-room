#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "Dialogue.h"

class AudioEngine;

// Very small event router for command-in-area triggers.
class EventSystem {
public:
    void registerCommandEvent(const std::string& areaId,
                              const std::string& command,
                              const Dialogue& dialogue,
                              bool fireOnce = true);

    void handleCommand(const std::string& areaId,
                       const std::string& command,
                       AudioEngine* audioEngine);

private:
    struct CommandEvent {
        std::string areaId;
        std::string command;
        Dialogue dialogue;
        bool fireOnce = true;
        bool alreadyFired = false;
    };

    std::vector<CommandEvent> commandEvents;

    static std::string toLower(const std::string& s) {
        std::string out = s;
        std::transform(out.begin(), out.end(), out.begin(), ::tolower);
        return out;
    }
};
