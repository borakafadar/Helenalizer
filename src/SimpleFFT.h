//
// Created by borak on 26.12.2025.
//

#ifndef HELENALIZER_SIMPLEFFT_H
#define HELENALIZER_SIMPLEFFT_H
#include <vector>
#include <complex>


class SimpleFFT {
public:
    // Smooths the edges of the audio chunk to prevent "spectral leakage"
    static void applyHannWindow(std::vector<float>& input);

    // Takes raw audio, runs the math, and returns the volume bars (Magnitudes)
    static std::vector<float> computeAmplitudes(const std::vector<float>& audioData);
private:
    static void fft(std::vector<std::complex<float>>& buffer);
};


#endif //HELENALIZER_SIMPLEFFT_H