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
uint32_t ACTUAL_DURATION = 0;

// LED Colors
float clrs[4][3] = {{1, 0., 0.},  // RED
                    {0., 1., 0.}, // GREEN
                    {0., 0., 1.}, // BLUE
                    {1., 0, 1.}}; // PURPLE

static SdmmcHandler sdcard;
FatFSInterface fsi;
FIL SDFile;

const int maxFiles = 1;
daisy::WavFileInfo m_file_info_[maxFiles];
int m_file_sizes[maxFiles];

struct waveFile {
    uint16_t format;
    uint16_t channels;
    uint32_t samplerate;
    uint16_t bitrate;
    uint32_t num_samples;
    uint16_t header_size;
};

waveFile parseHeader(FIL * file) {
    waveFile wf;

    uint32_t res;
    UINT bytesread;
    bool dataFound = false;    

    while (!dataFound) {

        f_read(file, &res, sizeof(uint32_t), &bytesread);

        switch (res) {
            case 0x20746D66: {
                    f_read(file, &res, 4, &bytesread);
                    f_read(file, &wf.format, sizeof(uint16_t), &bytesread);
                    f_read(file, &wf.channels, sizeof(uint16_t), &bytesread);
                    f_read(file, &wf.samplerate, sizeof(uint32_t), &bytesread);
                    f_read(file, &res, sizeof(uint32_t), &bytesread);
                    f_read(file, &res, sizeof(uint16_t), &bytesread);
                    f_read(file, &wf.bitrate, sizeof(uint16_t), &bytesread);
                    break;
                }
            case 0x61746164: {
                    f_read(file, &wf.num_samples, sizeof(uint32_t), &bytesread);
                    wf.num_samples = wf.num_samples / wf.channels/ 4;
                    dataFound = true;
                    break;
                }
            default:
                break;
        }

    }
 
    wf.header_size = f_tell(file);
    return wf;

}

void loadWavFiles()
{
    std::string m_sd_debug_msg = "no (fat32) sdcard";
    int m_file_cnt_ = 0;
    FRESULT result = FR_OK;
    FILINFO fno;
    DIR dir;
    char *fn;

    FATFS &fs = fsi.GetSDFileSystem();
    f_mount(&fs, "/", 1);

    if (f_opendir(&dir, "/") == FR_OK)
    {
        m_sd_debug_msg = "sd: no files";
        do
        {
            result = f_readdir(&dir, &fno);
            // Exit if bad read or NULL fname
            if (result != FR_OK || fno.fname[0] == 0)
                break;
            // Skip if its a directory or a hidden file.
            if (fno.fattrib & (AM_HID | AM_DIR))
                continue;
            // Now we'll check if its .wav and add to the list.
            fn = fno.fname;

            if (m_file_cnt_ < maxFiles)
            {
                if (strstr(fn, ".wav") || strstr(fn, ".WAV"))
                {
                    strcpy(m_file_info_[m_file_cnt_].name, fn);

                    UINT bytesread;
                    if (f_open(&SDFile, m_file_info_[m_file_cnt_].name, (FA_OPEN_EXISTING | FA_READ)) == FR_OK)
                    {
                        m_file_sizes[m_file_cnt_] = f_size(&SDFile);

                        waveFile wf = parseHeader(&SDFile);

                        if (wf.bitrate != 32)
                            continue;
                        if (wf.samplerate != 48000)
                            continue;
                        if (wf.channels != 2)
                            continue;
                        if (wf.format != 3)
                            continue;
                        
                        uint32_t size = m_file_sizes[m_file_cnt_] - wf.header_size;
                        ACTUAL_DURATION = size / ( wf.channels * sizeof(float));

                        if (&buffer[0])
                        {
                            if (f_read(&SDFile, &buffer[0], size, &bytesread) == FR_OK)
                            {
                                gran.setNumSamples(size / 8);
                                uint32_t mid = BUFFER_SIZE / 2;
                                uint32_t i, j;
                                for (i = 1; i < BUFFER_SIZE; ++i)
                                {
                                    j = i < mid ? i * 2 : ((i - mid) * 2 + 1);
                                    while (j < i)
                                    {
                                        j = j < mid ? j * 2 : ((j - mid) * 2 + 1);
                                    }
                                    float tmp = buffer[i];
                                    buffer[i] = buffer[j];
                                    buffer[j] = tmp;
                                }

                                m_file_cnt_++;
                            }
                        }
                        f_close(&SDFile);
                    }
                }
            }
            else
            {
                break;
            }
        } while (result == FR_OK);
    }
    f_closedir(&dir);
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{

    for (size_t i = 0; i < size; i++)
    {
        out[0][i] = 0.f;
        out[1][i] = 0.f;

        out[0][i] = volume * gran.play();
        out[1][i] = out[0][i]; 

        float revL, revR;
        rev.Process(out[0][i], out[1][i], &revL, &revR);
        out[0][i] += revL;
        out[1][i] += revR;
    }
}

void UpdateControls() {
    hw.ProcessAllControls();
    
    float ctrl0 = hw.GetKnobValue((Tusenskona::Knob)0);
    float ctrl1 = hw.GetKnobValue((Tusenskona::Knob)1);
    float ctrl2 = hw.GetKnobValue((Tusenskona::Knob)2);
    float ctrl3 = hw.GetKnobValue((Tusenskona::Knob)3);
 
    record ^= hw.button1.FallingEdge(); 

    int enc = hw.encoder.Increment();
    count = std::max(std::min(count + enc,2),0);
 
    switch (static_cast<view>(count)) {
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
            break;
        }

        default:
            break;
    }
    
    volume = ctrl3;

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
    std::fill(&buffer[0], &buffer[BUFFER_SIZE - 1], 0.f);

    rev.Init(48000);
    rev.SetFeedback(0.5f);
    rev.SetLpFreq(5000);

    SdmmcHandler::Config sd_cfg;
    sd_cfg.speed = SdmmcHandler::Speed::MEDIUM_SLOW;
    sd_cfg.width = SdmmcHandler::BusWidth::BITS_4;
    sdcard.Init(sd_cfg);

    // Link hardware and FatFS
    fsi.Init(FatFSInterface::Config::MEDIA_SD);

    loadWavFiles();

    hw.StartAdc();
    hw.StartAudio(AudioCallback);

    while(1) {
        UpdateControls();
        UpdateLEDs();
    }
}
