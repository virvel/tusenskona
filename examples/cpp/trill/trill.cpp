#include "../../../src/tusenskona.h"
#include "daisysp.h"
#include "../../../libDaisy/src/dev/trill/Trill.h"
#include "/Users/viktorsandstrom/Documents/CPP/dsp/dsp/waveshape.h"
#include "/Users/viktorsandstrom/Documents/CPP/dsp/dsp/wavetable.hpp"
#include "/Users/viktorsandstrom/Documents/CPP/dsp/dsp/interpolation.hpp"
#include <memory>

#define SAMPLE_RATE 48000
#define BLOCK_SIZE 64

using namespace daisy;
using namespace daisysp;
using namespace dspheaders;

Wavetable wt0, wt1, wt2, wt3;

int vol[4] = {0};

Tusenskona hw;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {
  for (size_t i = 0; i < size; i++) {
    float sig = wt0.play() * (vol[0] / 26000.f) * 0.2;
    sig += wt1.play() * (vol[1] / 16000.f) * 0.2;
    sig += wt2.play() * (vol[2] / 18000.f) * 0.2;
    sig += wt3.play() * (vol[3] / 26000.f) * 0.2;
    out[0][i] = sig;
    out[1][i] = sig;
  }
}

int main(void) {
  hw.Init();
  hw.seed.StartLog();
  Trill trill;

  hw.SetAudioSampleRate(daisy::SaiHandle::Config::SampleRate(SAMPLE_RATE));
  hw.SetAudioBlockSize(BLOCK_SIZE); // number of samples handled per callback

  wt0 = Wavetable(TRIANGLE, 512, SAMPLE_RATE, interpolation::linear);
  wt1 = Wavetable(TRIANGLE, 512, SAMPLE_RATE, interpolation::linear);
  wt2 = Wavetable(TRIANGLE, 512, SAMPLE_RATE, interpolation::linear);
  wt3 = Wavetable(TRIANGLE, 512, SAMPLE_RATE, interpolation::linear);

  float freq = 240;

  wt0.frequency = freq;
  wt1.frequency = freq * 9/8;
  wt2.frequency = freq * 6/2;
  wt3.frequency = freq * 5/4;
  

  int i2cBus = 1;

  int ret = trill.setup(i2cBus, Trill::CRAFT);

  if (ret) {
    hw.seed.PrintLine("trill.setup() returned %d", ret);
  }

  hw.StartAdc();
  hw.StartAudio(AudioCallback);

  trill.setNoiseThreshold(0.25);
  trill.setPrescaler(4);

  while(1) {
    hw.seed.DelayMs(100);
    trill.readI2C();
    int i = 0;
    for (auto &x: trill.rawData) {
      int v = int(x*100000.f);
      hw.seed.Print("%d ", v);
      if (i < 4) {
        vol[i] = v;
      }
      i++;
    }
    hw.seed.PrintLine("");
  }
}

