//
// Created by borak on 13.12.2025.
//

#ifndef HELENALIZER_VISUALIZERRECORDER_H
#define HELENALIZER_VISUALIZERRECORDER_H
#include <mutex>
#include <SFML/Audio/SoundRecorder.hpp>


class VisualizerRecorder : public sf::SoundRecorder{
public:
    std::vector<int16_t> getAudioData();
protected:
    bool onProcessSamples(const std::int16_t *samples, std::size_t sampleCount) override;

private:
    std::vector<int16_t> audioBuffer;
    std::mutex myMutex;
};


#endif //HELENALIZER_VISUALIZERRECORDER_H