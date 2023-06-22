#include "../../src/tusenskona.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;


Tusenskona hw;

float freq = 440.f;

Oscillator saw;
Oscillator sine;
ReverbSc rev;
DelayLine<float, 24000> delay;
float revAmount;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{

    for (size_t i = 0; i < size; i++)
    {
//        out[0][i] = sine.Process();
//        out[1][i] = saw.Process(); 
        float revL, revR;
        rev.Process(in[0][i], in[1][i], &revL, &revR);
        out[0][i] = (1.f-revAmount)*in[0][i];
        out[1][i] = (1.f-revAmount)*in[1][i];
        out[0][i] += revAmount*revL; 
        out[1][i] += revAmount*revR; 
        out[0][i] = tanh(out[0][i]);
        out[1][i] = tanh(out[1][i]);
//        delay.Write(out[0][i] + 0.5*delay.Read());
//        out[0][i] += 0.25*delay.Read();
    }
}

int inc = 0;
int count = 0;
float clrs[4][3] = {{0.5, 0., 0.},
                {0., 0.5, 0.},
                {0., 0., 0.5},
                {0.5, 0, 0.5}};



void UpdateControls()
{
    hw.ProcessAllControls();
    
    //knobs
    float ctrl = hw.GetKnobValue((Tusenskona::Knob)0);
    float ctrl1 = hw.GetKnobValue((Tusenskona::Knob)1);
    float ctrl2 = hw.GetKnobValue((Tusenskona::Knob)2);
    float ctrl3 = hw.GetKnobValue((Tusenskona::Knob)3);

    freq = powf(2.f,6.0f*ctrl) * 40.f; 
    sine.SetFreq(freq);
    freq = powf(2.f,6.0f*ctrl1) * 40.f; 
    saw.SetFreq(freq);

    revAmount = ctrl3;


    int enc = hw.encoder.Increment();
    bool sw1 = hw.encoder.Pressed();
    bool sw2 = hw.button1.Pressed();
    bool sw3 = hw.button2.Pressed();
    count = std::max(std::min(count + enc,3),0);
    count = std::max(std::min(count + sw1,3),0);
    count = std::max(std::min(count - static_cast<int>(sw2),3),0);
    count = std::max(std::min(count + static_cast<int>(sw3),3),0);

    sine.SetAmp(ctrl2);
    saw.SetAmp(ctrl2);
    
    hw.led.Set(clrs[count][0], clrs[count][1],clrs[count][2]);
    hw.UpdateLeds();

}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(48); // number of samples handled per callback
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    sine.Init(48000);
    sine.SetAmp(0.25f);
    sine.Reset();
    saw.Init(48000);
    saw.SetWaveform(3);
    saw.SetAmp(0.25f);
    saw.Reset();
    
    delay.Init();
    delay.SetDelay(static_cast<size_t>(24000));
    
    rev.Init(48000);
    rev.SetFeedback(0.99);
    rev.SetLpFreq(5000);

//    clrs[0].Init(0.5f, 0.f, 0.f);
//    clrs[1].Init(0.f, 0.5f, 0.f);
//    clrs[2].Init(0.f, 0.f, 0.5f);
//    clrs[3].Init(0.5f, 0.f, 0.5f);
    hw.StartAdc();
    hw.StartAudio(AudioCallback);

    while(1) {
        UpdateControls();
    }
}
