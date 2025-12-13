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
        bool setListeningDevice();
        void getListeningDevices();
        void testRecording();

        void startRecording();

        void stopRecording();

    private:
        std::vector<std::string> listeningDevices;
        std::string stereoMixDevice;
        VisualizerRecorder recorder;
};


#endif //HELENALIZER_AUDIOPROCESSING_H