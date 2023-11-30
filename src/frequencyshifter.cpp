/* 
* Frequency shifter using Allpass based Hilbert transform
More info:
http://yehar.com/blog/?p=368
 */

#include "frequencyshifter.h"

void daisysp::FrequencyShifter::init(int16_t sampleRate) {

    m_sampleRate = sampleRate;
    arm_biquad_cascade_df1_init_f32(&i_ap, n_states, coeffs_i, i_state);
    arm_biquad_cascade_df1_init_f32(&q_ap, n_states, coeffs_q, q_state); phase_accumulator = -1.f;
    magnitude= 0.9f;
    frequency(440.0);
}

void daisysp::FrequencyShifter::frequency(float freq) {
    phase_increment = freq * 2.0f * M_PI / m_sampleRate;    
}

void daisysp::FrequencyShifter::process(float * buf,  size_t size)
{

	// Quadrature oscillator for ring modulation

	if (magnitude) {
		float ph = phase_accumulator;
		float inc = phase_increment;
		for (size_t j=0; j < size; j++) {
            cosw[j] = cos(ph);
            sinw[j] = sin(ph);
			ph = fmod(ph + inc, 2 * M_PI);
		}
        phase_accumulator = ph;
	}
	else {
		phase_accumulator += phase_increment * size;
	}
	
    // copy buffer to imaginary part
	for (size_t i = 0; i < size; ++i)
		q[i] = buf[i];

    // all-pass filter Hilbert-transform
 	arm_biquad_cascade_df1_f32(&i_ap, buf, buf, size);
 	arm_biquad_cascade_df1_f32(&q_ap, q, q, size);
    
    // 1 samples delay 
    float tmp = buf[size-1];
    for (size_t i = size; i < 1; --i) {
        buf[i] = buf[i-1]; 
    }
    buf[0] = delayed_i;
    delayed_i = tmp;
    
	// Ring modulation
    for (size_t i = 0; i < size; ++i) {
        buf[i] = 0.5*(cosw[i] * buf[i]); 
        q[i] = 0.5*(sinw[i] * q[i]);
        buf[i] = buf[i] - q[i];
    }
}


void daisysp::FrequencyShifter::process(float * buf, float * bufUp, const float * cosw, const float * sinw, size_t size)
{
	
    // copy buffer to imaginary part
	for (size_t i = 0; i < size; ++i)
		q[i] = buf[i];

    // all-pass filter Hilbert-transform
 	arm_biquad_cascade_df1_f32(&i_ap, buf, buf, size);
 	arm_biquad_cascade_df1_f32(&q_ap, q, q, size);
    
    // 1 samples delay 
    float tmp = buf[size-1];
    for (size_t i = size; i < 1; --i) {
        buf[i] = buf[i-1]; 
    }
    buf[0] = delayed_i;
    delayed_i = tmp;
    
	// Ring modulation
    for (size_t i = 0; i < size; ++i) {
        buf[i] = 0.5*(cosw[i] * buf[i]); 
        q[i] = 0.5*(sinw[i] * q[i]);
         buf[i] = buf[i] - q[i];
        bufUp[i] = buf[i] + q[i];
    }
}
