#include "daisy.h"
#include "daisysp.h"

namespace daisysp
{

    class Grain
    {

    public:
        void init(float *buffer, uint32_t size);
        const float play();
        const inline void setActive(bool active) { m_active = active; }
        const inline void setRate(float rate) { m_rate = rate; }
        const inline void setOffset(float pos) { m_next_offset = static_cast<uint32_t>(fmin(pos * float((m_size - m_duration)), float(m_size - m_duration))); }
        const inline void setJitter(uint32_t jitter) { m_jitter = jitter; }
        const inline void setDuration(float s);
        const inline void setSize(uint32_t size) { m_size = size;}

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
        void init(float *buffer, uint32_t size);
        void setRate(float speed);
        void setDuration(float s);
        void setOffset(float o);
        void setJitter(float s) { m_spray = s;}
        const float play();
        void setNumSamples(uint32_t s);
        const inline uint32_t getNumSamples(void) { return m_numSamples; }

    private:
        float *m_buffer;
        float m_spray;
        uint32_t m_numSamples;
        const uint16_t m_num_grains = 3; 
        std::default_random_engine gen;
        std::uniform_real_distribution<> dist;
        Grain m_grains[3];
    };

}
