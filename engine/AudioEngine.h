#pragma once
#include <string>

// Lightweight audio helper that can play MP3 files for dialogue or SFX.
// On Windows it uses the multimedia subsystem; on macOS/Linux it shells out
// to common CLI players (afplay/ffplay/mpg123) with graceful fallback.
class AudioEngine {
public:
    AudioEngine() = default;

    // Play an MP3 file. If waitForCompletion is false, playback happens on a
    // detached thread so the game loop keeps running.
    bool playMp3(const std::string& filePath, bool waitForCompletion = false);

private:
    bool playBlocking(const std::string& filePath);
    std::string sanitizeForShell(const std::string& path) const;

#ifdef _WIN32
    unsigned int aliasCounter = 0;
#endif
};
