#pragma once
#include "daisy.h"
#include "daisysp.h"

#include <array>
#include <algorithm>

namespace daisysp {
            
    template <int num_harmonics>
    class Chebyshev {
       
        public:
            void init();
            float process(float in);
            void setAmplitude(const int n, const float w);
            const float * getAmplitudes() const;
            float * getAmplitudes();
        private:
        
            std::array<float,num_harmonics> amplitudes;
    };

    template <int num_harmonics>
    void Chebyshev<num_harmonics>::init() {
        std::fill(amplitudes.begin(), amplitudes.end(), 1.f); 
    }

    template <int num_harmonics>
    const float * Chebyshev<num_harmonics>::getAmplitudes() const {
            return amplitudes.data(); 
    }

    template <int num_harmonics>
    float * Chebyshev<num_harmonics>::getAmplitudes() {
            return amplitudes.data(); 
    }

    template <int num_harmonics>
    float Chebyshev<num_harmonics>::process(float in) {
        in = fmin(fmax(-1.f, in), 1.f);
  
        float prev = in;
        float curr = 2.f * in * in - 1.f;

        float tmp = 0.f;
        float sum = amplitudes[0]*prev + amplitudes[1]*curr;

        for (int i = 2; i < num_harmonics; ++i) {
            tmp = curr;
            curr = 2.f*in*curr - prev; 
            prev = tmp;

            sum += amplitudes[i] * curr;
         }

        return sum/num_harmonics; 
    }

    template <int num_harmonics>
    void Chebyshev<num_harmonics>::setAmplitude(const int n, const float w) {
        amplitudes[n] = w;
    }

} /* namespace daisysp */
