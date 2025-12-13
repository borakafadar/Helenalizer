//
// Created by borak on 13.12.2025.
//

#include "VisualizerRecorder.h"

#include <mutex>

std::vector<int16_t> VisualizerRecorder::getAudioData() {
    std::lock_guard<std::mutex> lock(myMutex);
    return audioBuffer;
}

bool VisualizerRecorder::onProcessSamples(const std::int16_t *samples, std::size_t sampleCount){
    std::lock_guard<std::mutex> lock (myMutex);
    for (size_t i = 0; i<sampleCount;i+=2) {
        //this is for stereo to mono, maybe we can change this into two channels in the future.
        int16_t averageSample = (samples[i] + samples[i+1])/2;
        audioBuffer.push_back(averageSample);
    }
    while (audioBuffer.size()>4096) {
        audioBuffer.erase(audioBuffer.begin());
    }

    return true;

}
