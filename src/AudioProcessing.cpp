//
// Created by borak on 12.12.2025.
//

#include "AudioProcessing.h"
#include <iostream>
#include "SimpleFFT.h"
#include <SFML/Audio.hpp>
#include "../libs/miniaudio.h"

// bool AudioProcessing::setListeningDevice() {
//     //TODO set processing with wasapi loopback
//
//     getListeningDevices();
//     bool stereoMixFound = false;
//     if (listeningDevices.empty()) {
//         std::cout << "No devices are available!" <<std::endl;
//         return false;
//     }
//
//     for (int i = 0; i<listeningDevices.size();i++) {
//         if (listeningDevices.at(i).find("Stereo Mix") != -1) {
//             stereoMixFound = true;
//             stereoMixDevice = listeningDevices.at(i);
//         }
//     }
//
//     if (stereoMixFound) {
//         recorder.setDevice(stereoMixDevice);
//     }
//     return stereoMixFound;
//
// }
//
// void AudioProcessing::getListeningDevices() {
//     listeningDevices = sf::SoundRecorder::getAvailableDevices();
//     for (int i = 0; i<listeningDevices.size();i++) {
//         std::cout << listeningDevices.at(i) << std::endl;
//     }
//
// }

void AudioProcessing::testRecording() {
    //std::cout << recorder.getDevice();
    if (recorder.getAudioData().size()>2000)
    std::cout << recorder.getAudioData().at(2000) << std::endl;

}
void AudioProcessing::startRecording() {
    recorder.start();
}
void AudioProcessing::stopRecording() {
    recorder.stop();
}

std::vector<float> AudioProcessing::getAudioData() {
    std::vector<float> audio = recorder.getAudioData();
    if (audio.size()>=4096) {
        audio.resize(4096);
        SimpleFFT::applyHannWindow(audio);

        SimpleFFT::computeAmplitudes(audio);
        return audio;
    }
    audio.clear();
    //return empty audio
    return audio;

}

