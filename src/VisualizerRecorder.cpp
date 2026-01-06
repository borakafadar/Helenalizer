//
// Created by borak on 13.12.2025.
//

#include "VisualizerRecorder.h"
#include <iostream>

VisualizerRecorder::VisualizerRecorder() {
    // Initialize the flag to false immediately
    isInitialized = false;

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
        // Do NOT set isInitialized to true here.
        // We return early or just let the constructor finish with isInitialized = false.
        return; 
    }

    // Only set this to true if the above function succeeded
    isInitialized = true;
}

VisualizerRecorder::~VisualizerRecorder() {
    // CRITICAL FIX: Only cleanup if we actually initialized successfully
    if (isInitialized) {
        ma_device_stop(&device);    // 1. HARD stop audio thread
        device.pUserData = nullptr; // 2. Make callback harmless
        ma_device_uninit(&device);  // 3. Now safe to destroy
    }
}

void VisualizerRecorder::start() {
    // CRITICAL FIX: Don't start a broken device
    if (!isInitialized) {
        std::cerr << "Cannot start: Audio device not initialized.\n";
        return;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        std::cerr << "Failed to start recording.\n";
    }
}

void VisualizerRecorder::stop() {
    // CRITICAL FIX: Don't stop a broken device
    if (isInitialized) {
        ma_device_stop(&device);
    }
}

std::vector<float> VisualizerRecorder::getAudioData() {
    // Safety check: if not initialized, return empty immediately
    if (!isInitialized) {
        return {}; 
    }

    std::lock_guard<std::mutex> lock(bufferMutex);
    return audioBuffer;
}

// ... data_callback remains the same ...
void VisualizerRecorder::data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount) {
    VisualizerRecorder *recorder = (VisualizerRecorder *) pDevice->pUserData;
    const float *samples = (const float *) pInput;

    if (recorder && samples) {
        std::lock_guard<std::mutex> lock(recorder->bufferMutex);

        for (ma_uint32 i = 0; i < frameCount; ++i) {
            float left = samples[i * 2];
            float right = samples[i * 2 + 1];
            float sample = (left + right) * 0.5f;
            recorder->audioBuffer.push_back(sample);
        }

        while (recorder->audioBuffer.size() > 4096) {
            recorder->audioBuffer.erase(recorder->audioBuffer.begin());
        }
    }
}