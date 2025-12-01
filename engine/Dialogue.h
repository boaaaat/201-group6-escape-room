#pragma once
#include <string>
#include <utility>

class AudioEngine;

// Dialogue represents spoken or narrated text that can optionally trigger
// a text-to-speech (MP3) clip via AudioEngine.
class Dialogue {
public:
    Dialogue(std::string text = "", std::string audioPath = "")
        : text(std::move(text)), audioPath(std::move(audioPath)) {}

    const std::string& getText() const { return text; }
    const std::string& getAudioPath() const { return audioPath; }

    void setText(const std::string& t) { text = t; }
    void setAudioPath(const std::string& path) { audioPath = path; }

    // Print the text and optionally play the audio clip if provided.
    void deliver(AudioEngine* audioEngine = nullptr,
                 bool waitForAudio = false) const;

    bool hasAudio() const { return !audioPath.empty(); }

private:
    std::string text;
    std::string audioPath;
};
