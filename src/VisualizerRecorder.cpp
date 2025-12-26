//
// Created by borak on 13.12.2025.
//

#include "VisualizerRecorder.h"
#include <iostream>

VisualizerRecorder::VisualizerRecorder() {
    // 1. Configure for WASAPI LOOPBACK (Captures desktop audio)
    ma_device_config config = ma_device_config_init(ma_device_type_loopback);

    // 2. Use Floats (easier for FFT) and 44.1kHz
    config.capture.format = ma_format_f32;
    config.capture.channels = 2;
    config.sampleRate = 44100;

    // 3. Link our callback and pass "this" instance
    config.dataCallback = data_callback;
    config.pUserData = this;

    // 4. Initialize the device
    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
        std::cerr << "Failed to initialize loopback device! (Check if a device is active)\n";
    }
}

VisualizerRecorder::~VisualizerRecorder() {
    ma_device_uninit(&device);
}

void VisualizerRecorder::start() {
    if (ma_device_start(&device) != MA_SUCCESS) {
        std::cerr << "Failed to start recording.\n";
    }
}

void VisualizerRecorder::stop() {
    ma_device_stop(&device);
}

std::vector<float> VisualizerRecorder::getAudioData() {
    std::lock_guard<std::mutex> lock(bufferMutex);
    return audioBuffer;
}

// === THE BACKGROUND THREAD WORKER ===
void VisualizerRecorder::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    // Recover the pointer to our class instance
    VisualizerRecorder* recorder = (VisualizerRecorder*)pDevice->pUserData;

    // Cast raw bytes to float (because we asked for ma_format_f32)
    const float* samples = (const float*)pInput;

    if (recorder && samples) {
        // Lock to prevent Main Loop from reading while we write
        std::lock_guard<std::mutex> lock(recorder->bufferMutex);

        // Process all frames in this chunk
        for (ma_uint32 i = 0; i < frameCount; ++i) {
            // Stereo split: Left is at index i*2, Right is at i*2+1
            float left = samples[i * 2];
            float right = samples[i * 2 + 1];

            // Mix to Mono
            float sample = (left + right) * 0.5f;

            recorder->audioBuffer.push_back(sample);
        }

        // Keep buffer size fixed at 4096 samples (sliding window)
        while (recorder->audioBuffer.size() > 4096) {
            recorder->audioBuffer.erase(recorder->audioBuffer.begin());
        }
    }
}
