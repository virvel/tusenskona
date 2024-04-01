#include "../../../src/tusenskona.h"
#include "daisysp.h"
#include "/Users/viktorsandstrom/Documents/CPP/dsp/dsp/grain.hpp"
#include "/Users/viktorsandstrom/Documents/CPP/dsp/dsp/buffer.hpp"
#include "/Users/viktorsandstrom/Documents/CPP/dsp/dsp/interpolation.hpp"
#include "/Users/viktorsandstrom/Documents/CPP/dsp/dsp/trigger.hpp"
#include "/Users/viktorsandstrom/Documents/CPP/dsp/dsp/dsp.h"
#include "/Users/viktorsandstrom/Documents/CPP/dsp/dsp/waveshape.h"
#include "../../../libDaisy/src/dev/trill/Trill.h";
#include <memory>

using namespace daisy;
using namespace daisysp;

Tusenskona hw;
Trill trill;

#define SAMPLE_RATE 48000.f
#define BLOCK_SIZE 512

float g_position = 0.5f;
float g_interval = 1.5f;
float g_jitter = 0.05;
float g_rate = 1.f;


constexpr unsigned buflength = 512 * 512;
constexpr unsigned envlength = 512;
const unsigned MAXGRAINS = 12;
const float min_interval = 0.2;

DSY_SDRAM_BSS float innerBuffer[buflength];
DSY_SDRAM_BSS float envBuffer[envlength];
static dspheaders::Granulator* gryn;
// static dspheaders::Dust trig = dspheaders::Dust(g_interval, SAMPLE_RATE);
static dspheaders::Impulse trig = dspheaders::Impulse(g_interval, SAMPLE_RATE);
std::shared_ptr<dspheaders::Buffer> buf = std::shared_ptr<Buffer>(new dspheaders::Buffer(0.f, SAMPLE_RATE, dspheaders::interpolation::linear)
);

unsigned writeptr = 0;
bool sw1 = false;
bool sw2 = false;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
  for (size_t i = 0; i < size; i++) {
    if (writeptr < buflength) {
      buf -> writesample(in[0][i], writeptr++);
      out[0][i] = in[0][i];
      out[1][i] = in[1][i];
    } else {
      hw.led.Set(0.f, 0.f, 1.f);
      // out[ch][i] = gryn->process(g_position, g_rate, dTrig.play(g_interval));
      out[0][i] = gryn->process(g_position, g_rate, trig.play(g_interval));
      out[1][i] = out[0][i];
    }
  }
}

void UpdateControls() {
    hw.ProcessAllControls();
    //knobs
    float ctrl = hw.GetKnobValue((Tusenskona::Knob)0);
    float ctrl1 = hw.GetKnobValue((Tusenskona::Knob)1);
    float ctrl2 = hw.GetKnobValue((Tusenskona::Knob)2);
    float ctrl3 = hw.GetKnobValue((Tusenskona::Knob)3);

    if (sw2) {
      // hw.led.Set(0.4, 0.5, 0.9);
      g_interval = dspheaders::map(ctrl, 0.f, 1.f, 0.1, 4.f);
      gryn->setJitter(ctrl1);
    } else {
      gryn->setGrainSize(dspheaders::map(ctrl, 0.f, 1.f, 0.05, 2.f));
      gryn->setNumGrains((unsigned)dspheaders::map(ctrl1, 0.f, 1.f, 1.f, MAXGRAINS));
    }

    g_position = ctrl2;
    g_rate = dspheaders::map(ctrl3, 0.f, 1.f, 0.f, 2.f);

    // int enc = hw.encoder.Increment();
    if (hw.encoder.Pressed()) {
      writeptr = 0;
      hw.led.Set(1.f, 1.f, 1.f);
    }
    sw1 = hw.button1.Pressed();
    sw2 = hw.button2.Pressed();
    hw.UpdateLeds();
}

int main(void) {
  hw.Init();
  hw.SetAudioSampleRate(daisy::SaiHandle::Config::SampleRate(SAMPLE_RATE));
  hw.SetAudioBlockSize(BLOCK_SIZE); // number of samples handled per callback
  
  int i2cBus = 1;
  int ret = trill.setup(i2cBus, Trill::CRAFT);

  if (ret) {
    hw.seed.PrintLine("trill.setup() returned %d", ret);
  }

  trill.setPrescaler(2);

  while(1) {
    hw.seed.DelayMs(100);
    trill.readI2C();
    for (auto &x: trill.rawData) {
      hw.seed.Print("%d ", int(x*100000.f));
    }
    hw.seed.PrintLine("");
  }
  

  // Hacky way to put buffer on SDRAM instead of rewrite
  buf->buffer = innerBuffer;
  buf->bufferlength = buflength;
  buf->initbuffer();

  hanning(envBuffer, envlength);

  gryn = new dspheaders::Granulator(
    0.2, 
    SAMPLE_RATE, 
    MAXGRAINS, 
    envBuffer,
    envlength,
    dspheaders::interpolation::cubic,
    buf
  );

  hw.led.Set(1.f, 1.f, 1.f);
  
  hw.StartAdc();
  hw.StartAudio(AudioCallback);

  while(1) {
    UpdateControls();
  }
}
