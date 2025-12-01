#include "AudioEngine.h"
#include <filesystem>
#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#endif

namespace fs = std::filesystem;

std::string AudioEngine::sanitizeForShell(const std::string& path) const {
    std::string safe = path;
    for (size_t i = 0; i < safe.size(); ++i) {
        if (safe[i] == '"') {
            safe.insert(i, "\\");
            ++i;
        }
    }
    return safe;
}

bool AudioEngine::playMp3(const std::string& filePath, bool waitForCompletion) {
    if (!fs::exists(filePath)) {
        std::cout << "[Audio] File not found: " << filePath << "\n";
        return false;
    }

    if (!waitForCompletion) {
        std::thread([this, filePath]() {
            this->playMp3(filePath, true);
        }).detach();
        return true;
    }

    return playBlocking(filePath);
}

bool AudioEngine::playBlocking(const std::string& filePath) {
#ifdef _WIN32
    std::string alias = "snd" + std::to_string(++aliasCounter);
    std::string openCmd = "open \"" + filePath + "\" type mpegvideo alias " + alias;
    MCIERROR openErr = mciSendStringA(openCmd.c_str(), nullptr, 0, nullptr);
    if (openErr != 0) {
        char errText[256] = {0};
        mciGetErrorStringA(openErr, errText, sizeof(errText));
        std::cout << "[Audio] Unable to open MP3: " << errText << "\n";
        return false;
    }

    std::string playCmd = "play " + alias + " wait";
    MCIERROR playErr = mciSendStringA(playCmd.c_str(), nullptr, 0, nullptr);
    mciSendStringA(("close " + alias).c_str(), nullptr, 0, nullptr);
    if (playErr != 0) {
        char errText[256] = {0};
        mciGetErrorStringA(playErr, errText, sizeof(errText));
        std::cout << "[Audio] Unable to play MP3: " << errText << "\n";
        return false;
    }
    return true;
#else
    const std::string safePath = sanitizeForShell(filePath);
#ifdef __APPLE__
    std::vector<std::string> commands = {
        "afplay \"" + safePath + "\""
    };
#else
    std::vector<std::string> commands = {
        "ffplay -nodisp -autoexit -loglevel quiet \"" + safePath + "\"",
        "mpg123 -q \"" + safePath + "\""
    };
#endif

    for (const auto& cmd : commands) {
        int result = std::system(cmd.c_str());
        if (result == 0) {
            return true;
        }
    }

    std::cout << "[Audio] Unable to play MP3. Install ffplay (ffmpeg) or mpg123 so playback commands succeed.\n";
    return false;
#endif
}
