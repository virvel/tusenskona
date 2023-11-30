#pragma once
#include "daisy.h"

namespace dsp {

    class Grain
    {

    public:
        void init(float * const buffer, uint32_t size);
        float play();
        void setActive(const bool active) { m_active = active; }
        void setRate(const float rate) { m_rate = rate; }
        void setOffset(const float pos) { m_next_offset = static_cast<uint32_t>(fmin(pos * float((m_size - m_duration)), float(m_size - m_duration))); }
        void setJitter(const uint32_t jitter) { m_jitter = jitter; }
        void setDuration(const float s);
        void setSize(const uint32_t size) { m_size = size;}

    private:
        bool m_active;
        float *m_buf;
        float m_position;
        float m_prevSample;
        float m_rate;
        uint32_t m_offset;
        uint32_t m_next_offset;
        uint32_t m_duration;
        uint32_t m_size;
        uint32_t m_jitter;
    };

    class Granulator
    {

    public:
        void init(float * const buffer, const uint32_t size);
        void setRate(const float speed);
        void setDuration(const float s);
        void setOffset(const float o);
        void setJitter(const float s) { m_spray = s;}
        float play();
        void setNumSamples(const uint32_t s);
        uint32_t getNumSamples(void) { return m_numSamples; }

    private:
        float *m_buffer;
        float m_spray;
        uint32_t m_numSamples;
        const uint8_t m_num_grains = 3; 
        Grain m_grains[3];
    };

}; /* namespace dsp */