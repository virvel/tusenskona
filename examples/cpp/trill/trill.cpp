#include "src/tusenskona.h"
#include "DaisySP/Source/daisysp.h"
#include "libDaisy/src/dev/trill/Trill.h"
#include "dsp-headers/dsp/dsp.h"
#include "dsp-headers/dsp/wavetable.hpp"
#include "dsp-headers/dsp/interpolation.hpp"
#include "th.h"

#define SAMPLE_RATE 44100
#define BLOCK_SIZE 32
#define VOICES 9

using namespace daisy;
using namespace daisysp;
namespace dsp = dspheaders;

dsp::Wavetable* wta[VOICES];

int vol[VOICES] = {0};

Tusenskona hw;
Trill trill;
TrillHandler th;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {
  float sig = 0.f;
  for (size_t i = 0; i < size; i++) {
    for (int j = 0; j < VOICES; j++) {
      sig += wta[j]->play() * th.getPinValue(j) * 0.2;
    }
    out[0][i] = sig;
    out[1][i] = sig;
  }
}

int main(void) {
  hw.Init();
  hw.seed.StartLog();

  hw.SetAudioSampleRate(daisy::SaiHandle::Config::SampleRate(SAMPLE_RATE));
  hw.SetAudioBlockSize(BLOCK_SIZE); // number of samples handled per callback

  float freq = 200.f;
  float ot = 1.f;
  for (auto &wt: wta) {
    wt = new dsp::Wavetable(dsp::SINE, 1024, SAMPLE_RATE, dsp::interpolation::cubic);
    wt->frequency = freq*ot;
    ot+=1.f;
  }

  hw.StartAdc();
  hw.StartAudio(AudioCallback);

  int ret = th.init(&trill);
  if (ret) {
    hw.seed.PrintLine("trill.setup() returned %d", ret);
  }

  while(1) {
    th.poll();
  }
}

