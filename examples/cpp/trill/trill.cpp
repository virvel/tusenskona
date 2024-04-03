#include "../../../src/tusenskona.h"
#include "daisysp.h"
#include "../../../libDaisy/src/dev/trill/Trill.h"
#include "/Users/viktorsandstrom/Documents/CPP/dsp/dsp/waveshape.h"
#include "/Users/viktorsandstrom/Documents/CPP/dsp/dsp/wavetable.hpp"
#include "/Users/viktorsandstrom/Documents/CPP/dsp/dsp/interpolation.hpp"
#include <memory>

#define SAMPLE_RATE 44100
#define BLOCK_SIZE 32

using namespace daisy;
using namespace daisysp;
using namespace dspheaders;

Wavetable* wta[10];

int vol[10] = {0};

Tusenskona hw;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {
  for (size_t i = 0; i < size; i++) {
    float sig;
    for (int j = 0; j < 10; j++) {
      sig += wta[j]->play() * (float(vol[j]) / 50000000.f) * 0.2;
    }
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

  float freq = 200.f;
  float ot = 1.f;
  for (auto &wt: wta) {
    wt = new Wavetable(SINE, 1024, SAMPLE_RATE, interpolation::cubic);
    wt->frequency = freq*ot;
    ot+=1.f;
  }

  int i2cBus = 1;

  int ret = trill.setup(i2cBus, Trill::CRAFT);

  if (ret) {
    hw.seed.PrintLine("trill.setup() returned %d", ret);
  }

  hw.StartAdc();
  hw.StartAudio(AudioCallback);

  trill.setPrescaler(4);

  while(1) {
    // hw.seed.DelayMs(100);
    trill.readI2C();
    int i = 0;
    for (auto &x: trill.rawData) {
      int v = int(x*100000.f);
      // hw.seed.Print("%d ", v);
      if (i < 10) {
        vol[i] = v;
      }
      i++;
    }
    hw.seed.PrintLine("");
  }
}

