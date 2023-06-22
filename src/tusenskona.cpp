#include "tusenskona.h"

#ifndef SAMPLE_RATE
//#define SAMPLE_RATE DSY_AUDIO_SAMPLE_RATE
#define SAMPLE_RATE 48014.f
#endif


#define SW_1_PIN 27
#define SW_2_PIN 24

#define ENC_A_PIN 26
#define ENC_B_PIN 25
#define ENC_CLICK_PIN 14

#define LED_R_PIN 28
#define LED_G_PIN 23
#define LED_B_PIN 22

#define KNOB_1_PIN 16
#define KNOB_2_PIN 17
#define KNOB_3_PIN 18
#define KNOB_4_PIN 19

using namespace daisy;

void Tusenskona::Init(bool boost)
{
    seed.Configure();
    seed.Init(boost);
    InitButtons();
    InitEncoder();
    InitLeds();
    InitKnobs();
    SetAudioBlockSize(48);
}

void Tusenskona::DelayMs(size_t del)
{
    seed.DelayMs(del);
}


void Tusenskona::SetHidUpdateRates()
{
    for(int i = 0; i < KNOB_LAST; i++)
    {
        knobs[i]->SetSampleRate(AudioCallbackRate());
    }
}

void Tusenskona::StartAudio(AudioHandle::InterleavingAudioCallback cb)
{
    seed.StartAudio(cb);
}

void Tusenskona::StartAudio(AudioHandle::AudioCallback cb)
{
    seed.StartAudio(cb);
}

void Tusenskona::ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void Tusenskona::ChangeAudioCallback(AudioHandle::AudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void Tusenskona::StopAudio()
{
    seed.StopAudio();
}

void Tusenskona::SetAudioBlockSize(size_t size)
{
    seed.SetAudioBlockSize(size);
    SetHidUpdateRates();
}

size_t Tusenskona::AudioBlockSize()
{
    return seed.AudioBlockSize();
}

void Tusenskona::SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate)
{
    seed.SetAudioSampleRate(samplerate);
    SetHidUpdateRates();
}

float Tusenskona::AudioSampleRate()
{
    return seed.AudioSampleRate();
}

float Tusenskona::AudioCallbackRate()
{
    return seed.AudioCallbackRate();
}

void Tusenskona::StartAdc()
{
    seed.adc.Start();
}

void Tusenskona::StopAdc()
{
    seed.adc.Stop();
}


void Tusenskona::ProcessAnalogControls()
{
    knob1.Process();
    knob2.Process();
    knob3.Process();
    knob4.Process();
}

float Tusenskona::GetKnobValue(Knob k)
{
    size_t idx;
    idx = k < KNOB_LAST ? k : KNOB_1;
    return knobs[idx]->Value();
}

void Tusenskona::ProcessDigitalControls()
{
    encoder.Debounce();
    button1.Debounce();
    button2.Debounce();
}

void Tusenskona::ClearLeds()
{
    led.Set(0.0f, 0.0f, 0.0f);
}

void Tusenskona::UpdateLeds()
{
    led.Update();
}

void Tusenskona::InitButtons()
{
    // button1
    button1.Init(seed.GetPin(SW_1_PIN));
    // button2
    button2.Init(seed.GetPin(SW_2_PIN));

    buttons[BUTTON_1] = &button1;
    buttons[BUTTON_2] = &button2;
}

void Tusenskona::InitEncoder()
{
    dsy_gpio_pin a, b, click;
    a     = seed.GetPin(ENC_A_PIN);
    b     = seed.GetPin(ENC_B_PIN);
    click = seed.GetPin(ENC_CLICK_PIN);
    encoder.Init(a, b, click);
}

void Tusenskona::InitLeds()
{
    dsy_gpio_pin rpin, gpin, bpin;

    rpin = seed.GetPin(LED_R_PIN);
    gpin = seed.GetPin(LED_G_PIN);
    bpin = seed.GetPin(LED_B_PIN);
    led.Init(rpin, gpin, bpin, false);

    ClearLeds();
    UpdateLeds();
}
void Tusenskona::InitKnobs()
{
    // Configure the ADC channels using the desired pin
    AdcChannelConfig knob_init[KNOB_LAST];
    knob_init[KNOB_1].InitSingle(seed.GetPin(KNOB_1_PIN));
    knob_init[KNOB_2].InitSingle(seed.GetPin(KNOB_2_PIN));
    knob_init[KNOB_3].InitSingle(seed.GetPin(KNOB_3_PIN));
    knob_init[KNOB_4].InitSingle(seed.GetPin(KNOB_4_PIN));
    // Initialize with the knob init struct w/ 2 members
    // Set Oversampling to 32x
    seed.adc.Init(knob_init, KNOB_LAST);
    // Make an array of pointers to the knobs.
    knobs[KNOB_1] = &knob1;
    knobs[KNOB_2] = &knob2;
    knobs[KNOB_3] = &knob3;
    knobs[KNOB_4] = &knob4;
    for(int i = 0; i < KNOB_LAST; i++)
    {
        knobs[i]->Init(seed.adc.GetPtr(i), seed.AudioCallbackRate());
    }
}

