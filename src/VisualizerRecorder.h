//
// Created by borak on 13.12.2025.
//

#ifndef HELENALIZER_VISUALIZERRECORDER_H
#define HELENALIZER_VISUALIZERRECORDER_H

#include <mutex>
#include <SFML/Audio/SoundRecorder.hpp>
#include <vector>
#include "../libs/miniaudio.h"

class VisualizerRecorder {
public:
    VisualizerRecorder();
    ~VisualizerRecorder();

    // Controls
    void start();
    void stop();

    // Safe way to get audio data from the main loop
    std::vector<float> getAudioData();

private:
    ma_device device  =  {};
    std::vector<float> audioBuffer;
    std::mutex bufferMutex;


    // The static callback function required by miniaudio
    static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
};


#endif //HELENALIZER_VISUALIZERRECORDER_H