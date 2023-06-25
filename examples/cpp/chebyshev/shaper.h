#pragma once
#include "daisy.h"
#include "daisysp.h"

#include <array>
#include <algorithm>

namespace daisysp {

    template<std::size_t Length, typename Generator, std::size_t... Indexes>
    constexpr auto lut_impl(Generator&& f, std::index_sequence<Indexes...>) {
        using content_type = decltype(f(std::size_t{0}));
        return std::array<content_type, Length> {{ f(Indexes)... }};
    }

    template<std::size_t Length, typename Generator>
    constexpr auto lut(Generator&& f){
        return lut_impl<Length>(std::forward<Generator>(f), std::make_index_sequence<Length>{});
    }

    template <size_t n>
    constexpr double cheb(double x) { return 2*x*cheb<n-1>(x) - cheb<n-2>(x); }
    template <>
    constexpr double cheb<0>(double x) { return 1;}
    template <>
    constexpr double cheb<1>(double x) { return x;}

    template <size_t Length, size_t k>
    constexpr float cheby(size_t n) {
        double x = 2.f*(static_cast<double>(n)/static_cast<double>(Length-1)) -1.f;
        return static_cast<double>(cheb<k>(x)); 
    }

    template<std::size_t Length, size_t k>
    constexpr auto chebyshev_lut = lut<Length>(cheby<Length, k>);

    constexpr int NN = 512;
    constexpr auto c1 = chebyshev_lut<NN,1>;
    constexpr auto c2 = chebyshev_lut<NN,2>;
    constexpr auto c3 = chebyshev_lut<NN,3>;
    constexpr auto c4 = chebyshev_lut<NN,4>;
    constexpr auto c5 = chebyshev_lut<NN,5>;
    constexpr auto c6 = chebyshev_lut<NN,6>;
    constexpr auto c7 = chebyshev_lut<NN,7>;
    constexpr auto c8 = chebyshev_lut<NN,8>;
    constexpr auto c9 = chebyshev_lut<NN,9>;
    constexpr auto c10 = chebyshev_lut<NN,10>;
    
    template <size_t N>
    class Shaper {
       
        public:
            Shaper(); 
            void init();
            float process(float in);
#ifdef ARM_MATH
            float process_fast(float in);
#endif
            void setWeight(const int n, const float w);
            const float * getWeights() const;
            float * getWeights();
        private:
        
            std::array<float,10> weights;
            float m_inv_sum;
    };


    template <size_t N>
    Shaper<N>::Shaper() {
        static_assert(not (N & (N - 1)), "N not power of 2");
    }

    template <size_t N>
    void Shaper<N>::init() {
        std::fill(weights.begin(), weights.end(), 1.f); 
        m_inv_sum = std::accumulate(weights.begin(), weights.end(), 0.f);
    }

    template <size_t N>
    const float * Shaper<N>::getWeights() const {
            return weights.data(); 
    }

    template <size_t N>
    float * Shaper<N>::getWeights() {
            return weights.data(); 
    }

// TODO: Add better interpolation
    template <size_t N>
    float Shaper<N>::process(float in) {
        in = fmin(fmax(-1.f, in), 1.f);
        const float ind = static_cast<float>(N) *  (in + 1.f)/2.f;
        const uint16_t int_ind = static_cast<uint16_t>(ind);
        const float frac = ind - static_cast<float>(int_ind);
        m_inv_sum = 1.f/std::accumulate(weights.begin(), weights.end(), 0.f);
        float a = weights[0] * c1[int_ind]
                + weights[1] * c2[int_ind]
                + weights[2] * c3[int_ind]
                + weights[3] * c4[int_ind]
                + weights[4] * c5[int_ind]
                + weights[5] * c6[int_ind]
                + weights[6] * c7[int_ind]
                + weights[7] * c8[int_ind]
                + weights[8] * c9[int_ind]
                + weights[9] * c10[int_ind];
        const uint16_t ind2 = std::min<uint16_t>(N,int_ind+1);
        float b = weights[0] * c1[ind2]
                + weights[1] * c2[ind2]
                + weights[2] * c3[ind2]
                + weights[3] * c4[ind2]
                + weights[4] * c5[ind2]
                + weights[5] * c6[ind2]
                + weights[6] * c7[ind2]
                + weights[7] * c8[ind2]
                + weights[8] * c9[ind2]
                + weights[9] * c10[ind2];
        float r = a + frac*(a-b);
        return r*m_inv_sum;  
    }

#ifdef ARM_MATH
    // faster implementation (?)
    // assumes transposed coefficient matrix
    template <size_t N>
    float Shaper<N>::process_fast(float in) {
        in = fmin(fmax(-1.f, in), 1.f);
        const float ind = static_cast<float>(N) *  (in + 1.f)/2.f;
        const uint_fast16_t int_ind = static_cast<uint_fast16_t>(ind);
        const float frac = ind - static_cast<float>(int_ind);
        m_inv_sum = 1.f/std::accumulate(weights.begin(), weights.end(), 0.f);
        const uint_fast16_t ind2 = std::min<uint_fast16_t>(N,int_ind+1);
        
        float a = arm_weighted_sum_f32(&weights, &C[ind], 10);
        float b = arm_weighted_sum_f32(&weights, &C[ind2], 10);
        
        float r = a + frac*(a-b);
        return r*m_inv_sum;  
    }
#endif

    template <size_t N>
    void Shaper<N>::setWeight(const int n, const float w) {
        weights[n] = w;
    }

} /* namespace daisysp */
