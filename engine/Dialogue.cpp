#include "Dialogue.h"
#include "AudioEngine.h"
#include <iostream>

void Dialogue::deliver(AudioEngine* audioEngine, bool waitForAudio) const {
    if (!text.empty()) {
        std::cout << text << "\n";
    }
    if (audioEngine && !audioPath.empty()) {
        audioEngine->playMp3(audioPath, waitForAudio);
    }
}
