//
// Created by borak on 12.12.2025.
//

#ifndef HELENALIZER_AUDIOPROCESSING_H
#define HELENALIZER_AUDIOPROCESSING_H

#include <SFML/Audio.hpp>
#include <vector>

#include "VisualizerRecorder.h"

class AudioProcessing {
    public:
        // bool setListeningDevice();
        // void getListeningDevices();
        static constexpr int AUDIO_SIZE = 4096;
        void testRecording();

        void startRecording();

        void stopRecording();

        std::vector<float> getAudioData();

        std::vector<float> getFFTAudioData();

    private:
        std::vector<std::string> listeningDevices;
        std::string stereoMixDevice;
        VisualizerRecorder recorder;
};


#endif //HELENALIZER_AUDIOPROCESSING_H