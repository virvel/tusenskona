#include "../../src/tusenskona.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;


Tusenskona hw;

void UpdateControls()
{
    hw.ProcessAllControls();
    
    //knobs
    float ctrl = hw.GetKnobValue((Tusenskona::Knob)0);
    float ctrl1 = hw.GetKnobValue((Tusenskona::Knob)1);
    float ctrl2 = hw.GetKnobValue((Tusenskona::Knob)2);
    float ctrl3 = hw.GetKnobValue((Tusenskona::Knob)3);
 
    hw.led.Set(ctrl*ctrl3, ctrl1*ctrl3, ctrl2*ctrl3);
    hw.UpdateLeds();

}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(48); 
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

    hw.StartAdc();

    while(1) {
        UpdateControls();
    }
}
