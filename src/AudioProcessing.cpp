//
// Created by borak on 12.12.2025.
//

#include "AudioProcessing.h"
#include <iostream>
#include "SimpleFFT.h"
#include <SFML/Audio.hpp>
#include "../libs/miniaudio.h"


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
    if (audio.size()>=AUDIO_SIZE) {
        audio.resize(AUDIO_SIZE);
        SimpleFFT::applyHannWindow(audio);

        //SimpleFFT::computeAmplitudes(audio);
        return audio;
    }
    audio.clear();
    //return empty audio
    return audio;

}
std::vector<float> AudioProcessing::getFFTAudioData() {
    std::vector<float> audio = recorder.getAudioData();
    if (audio.size()>=AUDIO_SIZE) {
        audio.resize(AUDIO_SIZE);
        SimpleFFT::applyHannWindow(audio);

        return SimpleFFT::computeAmplitudes(audio);;
    }
    audio.clear();
    return audio;
}

