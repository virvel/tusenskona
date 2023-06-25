#include "../../../src/tusenskona.h"
#include "daisysp.h"
#include "shaper.h"

#include <array>
using namespace daisy;
using namespace daisysp;


void UpdateControls();

Tusenskona hw;

float freq = 440.f;

Shaper<512> shaper;
Shaper<512> shaper2;
ReverbSc rev;

Oscillator sine;

namespace mcon {
    constexpr float pi = 3.14159265358979323846f; 
    constexpr float twopi = 2.f*pi; 
    constexpr float e = 2.71828182845904523536f;
}

float waver[512];
float x = 0,y = 0;
float ph = 0.f;
float ph2 = 0.4f;
float inc = 0.00001f;
float inc2 = 0.0001f;
float z = 0.f;

uint_fast8_t selected = 0;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    UpdateControls();
    for (size_t i = 0; i < size; i++)
    {
        out[0][i] = 0.f;
        out[1][i] = 0.f;
        out[2][i] = 0.f;
        out[3][i] = 0.f;
        float s = sine.Process();
        float y = shaper2.process(shaper.process(s))*0.5 + s*0.5;
        out[0][i] = y;
        out[1][i] = y; 
        float revL, revR; 
        rev.Process(out[1][i], out[1][i], &revL, &revR); 
        out[0][i] += revL;
        out[1][i] += revR;
        out[0][i] *= z;
        out[1][i] *= z;
        float * ws = shaper.getWeights();
        for (uint_fast8_t i = 0; i < 10; ++i) {
             ws[i] = waver[(52*i + static_cast<int>(ph*512))%512]; 
        }
        ws = shaper2.getWeights();
        for (uint_fast8_t i = 0; i < 10; ++i) {
             ws[i] = waver[(52*i + static_cast<int>(ph2*512))%512]; 
       }
        ph = fmod(ph + inc, 1.f);
        ph2 = fmod(ph2 + inc2, 1.f);
    }
}

void UpdateControls()
{
    hw.ProcessAllControls();

    float ctrl = hw.GetKnobValue((Tusenskona::Knob)0);
        
    freq = powf(2.f,6.0f*ctrl) * 30.f; 
    sine.SetFreq(freq);
    inc = hw.GetKnobValue((Tusenskona::Knob)1)/1000;
    inc2 = hw.GetKnobValue((Tusenskona::Knob)2)/1000;
    z = hw.GetKnobValue((Tusenskona::Knob)3);
    
}

void UpdateLed() {
    hw.led.Set(sin(ph*mcon::twopi), 0, 1-sin(ph2*mcon::twopi));
    hw.UpdateLeds();
}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(48); // number of samples handled per callback
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    rev.Init(48000); 
    rev.SetFeedback(0.3);
    rev.SetLpFreq(5000);
    shaper.init();
    shaper2.init();
    sine.Init(48000);
    sine.SetFreq(40);
    sine.SetAmp(1.f);
    sine.Reset();
    constexpr float maxn = 512.f; 
    for (int i = 0; i < 512; ++i) {
        float index = static_cast<float>(i)/maxn;
        waver[i] = exp(-pow(mcon::e*(index-0.5)*4, 2));
    } 
 
    hw.StartAdc();
    hw.StartAudio(AudioCallback);
    while(1) { 
        UpdateLed();
    }
}
