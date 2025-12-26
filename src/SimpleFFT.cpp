//
// Created by borak on 26.12.2025.
//

#include "SimpleFFT.h"

#include "SimpleFFT.h"
#include <complex>
#include <cmath>
#include <algorithm>

// Internal definitions (not visible to main.cpp)
const float PI = 3.14159265358979f;

// --- INTERNAL HELPER: The Recursive FFT Algorithm ---
// We keep this separate so the code below is cleaner
void SimpleFFT::fft(std::vector<std::complex<float> > &buffer) {
    size_t n = buffer.size();
    if (n <= 1) return;

    // Split into even and odd indices
    std::vector<std::complex<float> > even(n / 2);
    std::vector<std::complex<float> > odd(n / 2);

    for (size_t i = 0; i < n / 2; ++i) {
        even[i] = buffer[i * 2];
        odd[i] = buffer[i * 2 + 1];
    }

    // Recursion
    fft(even);
    fft(odd);

    // Recombine (Butterfly operation)
    for (size_t k = 0; k < n / 2; ++k) {
        std::complex<float> t = std::polar(1.0f, -2.0f * PI * k / n) * odd[k];
        buffer[k] = even[k] + t;
        buffer[k + n / 2] = even[k] - t;
    }
}

void SimpleFFT::applyHannWindow(std::vector<float> &input) {
    size_t n = input.size();
    for (size_t i = 0; i < n; ++i) {
        float multiplier = 0.5f * (1.0f - std::cos(2.0f * PI * i / (n - 1)));
        input[i] = input[i] * multiplier;
    }
}

std::vector<float> SimpleFFT::computeAmplitudes(const std::vector<float> &audioData) {
    // 1. Convert float audio to Complex numbers
    std::vector<std::complex<float> > complexBuffer;
    complexBuffer.reserve(audioData.size());

    for (float sample: audioData) {
        complexBuffer.push_back(std::complex<float>(sample, 0.0f));
    }

    // 2. Run the recursive FFT helper
    fft(complexBuffer);

    // 3. Calculate Magnitudes for the first half
    std::vector<float> output;
    size_t halfSize = complexBuffer.size() / 2;
    output.reserve(halfSize);

    for (size_t i = 0; i < halfSize; ++i) {
        float mag = std::abs(complexBuffer[i]);
        // Logarithmic scaling: log10(1 + magnitude)
        output.push_back(std::log10(1.0f + mag));
    }

    return output;
}
