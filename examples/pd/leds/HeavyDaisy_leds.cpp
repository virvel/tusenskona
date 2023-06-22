/**
 * Copyright (c) 2023 Enzien Audio, Ltd.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the phrase "powered by heavy",
 *    the heavy logo, and a hyperlink to https://enzienaudio.com, all in a visible
 *    form.
 * 
 *   2.1 If the Application is distributed in a store system (for example,
 *       the Apple "App Store" or "Google Play"), the phrase "powered by heavy"
 *       shall be included in the app description or the copyright text as well as
 *       the in the app itself. The heavy logo will shall be visible in the app
 *       itself as well.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "Heavy_leds.h"
#include "Heavy_leds.hpp" 
#include "HeavyDaisy_leds.hpp"                                                                 

#define SAMPLE_RATE 48000.f

using namespace daisy;

json2daisy::DaisyTusenskona hardware;

Heavy_leds hv(SAMPLE_RATE);

void audiocallback(daisy::AudioHandle::InputBuffer in, daisy::AudioHandle::OutputBuffer out, size_t size);
static void sendHook(HeavyContextInterface *c, const char *receiverName, uint32_t receiverHash, const HvMessage * m);
void CallbackWriteIn(Heavy_leds& hv);
void LoopWriteIn(Heavy_leds& hv);
void CallbackWriteOut();
void LoopWriteOut();
void PostProcess();
void Display();

constexpr int DaisyNumOutputParameters = 1;
/** This array holds the output values received from PD hooks. These values are
 *  then written at the appropriate time in the following callback or loop.
 */
float output_data[DaisyNumOutputParameters];

struct DaisyHvParamOut
{
	uint32_t hash;
	uint32_t index;
  void (*hook_write_out)(float);

	void Process(float sig)
	{
		output_data[index] = sig;
    if (hook_write_out)
      (*hook_write_out)(sig);
	}
};


DaisyHvParamOut DaisyOutputParameters[DaisyNumOutputParameters] = {
		{ (uint32_t) HV_LEDS_PARAM_OUT_LED, 0, nullptr }, // led
};

int main(void)
{
  hardware.Init(true);
  hardware.StartAudio(audiocallback);

  hv.setSendHook(sendHook);

  for(;;)
  {
    hardware.LoopProcess();
    Display();
    LoopWriteOut();
  }
}

/** The audio processing function. At the standard 48KHz sample rate and a block
 *  size of 48, this will fire every millisecond.
 */
void audiocallback(daisy::AudioHandle::InputBuffer in, daisy::AudioHandle::OutputBuffer out, size_t size)
{
  // A zero fill to keep I/O quiet for a patch lacking ADC~/DAC~
  for (size_t chn = 0; chn < 2; chn++)
  {
    for (size_t i = 0; i < size; i++)
      out[chn][i] = 0;
  }
  hv.process((float**)in, (float**)out, size);
  CallbackWriteOut();
  hardware.PostProcess();
}

/** Receives messages from PD and writes them to the appropriate 
 *  index in the `output_data` array, to be written later.
 */
static void sendHook(HeavyContextInterface *c, const char *receiverName, uint32_t receiverHash, const HvMessage * m) 
{
  for (int i = 0; i < DaisyNumOutputParameters; i++)
  {
    if (DaisyOutputParameters[i].hash == receiverHash)
    {
      DaisyOutputParameters[i].Process(msg_getFloat(m, 0));
    }
  }
}

/** Sends signals from the Daisy hardware to the PD patch via the receive objects during the main loop
 * 
 */
void LoopWriteIn(Heavy_leds& hv)
{
}

/** Sends signals from the Daisy hardware to the PD patch via the receive objects during the audio callback
 * 
 */
void CallbackWriteIn(Heavy_leds& hv)
{
}

/** Writes the values sent to PD's receive objects to the Daisy hardware during the main loop
 * 
 */
void LoopWriteOut() {
}

/** Writes the values sent to PD's receive objects to the Daisy hardware during the audio callback
 * 
 */
void CallbackWriteOut() {
  hardware.led.Set(output_data[0],output_data[0],output_data[0]);
}

/** Handles the display code if the hardware defines a display
 * 
 */
void Display() {
  
}