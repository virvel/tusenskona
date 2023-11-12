#include "../../src/tusenskona.h"
#include "daisysp.h"
#include "granulator.h"

using namespace daisy;
using namespace daisysp;


Tusenskona hw;

ReverbSc rev;
Granulator gran;
float volume;

// Size yields buffer length of ~21.8 secs, while maximum for 48000Khz stereo float is ~43 secs
// number_of_entries/ (num_channels * sampleRate * sizeof(float))
const uint32_t BUFFER_SIZE = 8*1024*1024;

DSY_SDRAM_BSS float buffer[BUFFER_SIZE];

enum view {
    GRAIN1,
    GRAIN2,
    FX
};

int count;
bool record = false;
bool blink = true;

// LED Colors
float clrs[4][3] = {{1, 0., 0.},  // RED
                    {0., 1., 0.}, // GREEN
                    {0., 0., 1.}, // BLUE
                    {1., 0, 1.}}; // PURPLE

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{

    for (size_t i = 0; i < size; i++)
    {
        out[0][i] = volume * gran.play();
        out[1][i] = out[1][i]; 
        float revL, revR;
        rev.Process(in[0][i], in[1][i], &revL, &revR);
    }
}


void UpdateControls()
{
    hw.ProcessAllControls();
    
    //knobs
    float ctrl0 = hw.GetKnobValue((Tusenskona::Knob)0);
    float ctrl1 = hw.GetKnobValue((Tusenskona::Knob)1);
    float ctrl2 = hw.GetKnobValue((Tusenskona::Knob)2);
    float ctrl3 = hw.GetKnobValue((Tusenskona::Knob)3);
 
    record = record^hw.button1.FallingEdge(); 

    int enc = hw.encoder.Increment();
    count = std::max(std::min(count + enc,2),0);
 

    switch (view(count)) {

        case GRAIN1: {
            gran.setRate(ctrl0);
            gran.setOffset(ctrl1);
            gran.setDuration(ctrl2);
            break;
        }
        case GRAIN2: {
            gran.setJitter(ctrl0);
            break;
        }

        case FX: {
            rev.SetFeedback(ctrl0);
            rev.SetLpFreq(ctrl1*10000);
            volume = ctrl3;
            break;
        }

        default:
            break;
    }
    

    hw.UpdateLeds();

}

void UpdateLEDs() {

    if (record) {
        hw.DelayMs(200);
        if (blink) {
            hw.led.Set(clrs[count][0], clrs[count][1], clrs[count][2]);
        } 
        else {
            hw.led.Set(0., 0., 0.);
        }
        blink = !blink;
    }
    else {
            hw.led.Set(clrs[count][0], clrs[count][1], clrs[count][2]);
    }

}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(48); 
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

    gran.init(&buffer[0], BUFFER_SIZE);
    rev.Init(48000);
    rev.SetFeedback(0.5f);
    rev.SetLpFreq(5000);

    hw.StartAdc();

    while(1) {
        UpdateControls();
        UpdateLEDs();
    }
}
