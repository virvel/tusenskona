#ifndef TUSENSKONA_H
#define TUSENSKONA_H

#include "daisy_seed.h"

namespace daisy
{

class Tusenskona
{
  public:
    /** Switches */
    enum Sw
    {
        BUTTON_1,    /** & */
        BUTTON_2,    /** & */
        BUTTON_LAST, /** &  */
    };

    /** Knobs */
    enum Knob
    {
        KNOB_1,    /** &  */
        KNOB_2,    /** & */
        KNOB_3,    /** & */
        KNOB_4,    /** & */
        KNOB_LAST, /** & */
    };

    Tusenskona() {}
    ~Tusenskona() {}

    /** Init related stuff. */
    void Init(bool boost = false);

    /** Wait for a bit
    \param del Time to wait in ms.
    */
    void DelayMs(size_t del);

    /** Starts the callback
    \param cb Interleaved callback function
    */
    void StartAudio(AudioHandle::InterleavingAudioCallback cb);

    /** Starts the callback
    \param cb multichannel callback function
    */
    void StartAudio(AudioHandle::AudioCallback cb);

    /**
       Switch callback functions
       \param cb New interleaved callback function.
    */
    void ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb);

    /**
       Switch callback functions
       \param cb New multichannel callback function.
    */
    void ChangeAudioCallback(AudioHandle::AudioCallback cb);

    /** Stops the audio if it is running. */
    void StopAudio();

    /** Updates the Audio Sample Rate, and reinitializes.
     ** Audio must be stopped for this to work.
     */
    void SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate);

    /** Returns the audio sample rate in Hz as a floating point number.
     */
    float AudioSampleRate();

    /** Sets the number of samples processed per channel by the audio callback.
     */
    void SetAudioBlockSize(size_t blocksize);

    /** Returns the number of samples per channel in a block of audio. */
    size_t AudioBlockSize();

    /** Returns the rate in Hz that the Audio callback is called */
    float AudioCallbackRate();

    /**
       Start analog to digital conversion.
     */
    void StartAdc();

    /** Stops Transfering data from the ADC */
    void StopAdc();

    /** Call at same rate as analog reads for smooth reading.*/
    void ProcessAnalogControls();

    /** Process Analog and Digital Controls */
    inline void ProcessAllControls()
    {
        ProcessAnalogControls();
        ProcessDigitalControls();
    }

    /** & */
    float GetKnobValue(Knob k);

    /** Process digital controls */
    void ProcessDigitalControls();

    /** Reset Leds*/
    void ClearLeds();

    /** Update Leds to set colors*/
    void UpdateLeds();

    /** Public Members */
    DaisySeed     seed;        /**<# */
    Encoder       encoder;     /**< & */
    AnalogControl knob1,       /**< & */
        knob2,                 /**< & */
        knob3,
        knob4,
        *knobs[KNOB_LAST];     /**< & */
    Switch button1,            /**< & */
        button2,               /**< & */
        *buttons[BUTTON_LAST]; /**< & */
    RgbLed led;              /**< & */

  private:
    void SetHidUpdateRates();
    void InitButtons();
    void InitEncoder();
    void InitLeds();
    void InitKnobs();
    void InitMidi();
};

} // namespace daisy
#endif

