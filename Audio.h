#ifndef _AUDIO_H_
#define _AUDIO_H_
#include "Types.h"
#include "Multi_Buffer.h"

class Gb_Apu;
class Sound_Queue;

/*************************************************************//**
 * \brief Class that handle the audio system of the Nintendo Game Boy
 *
 * This class implements the audio system of the Nintendo Game Boy.
 * It includes some types and methods from the Snd_Gb_Sound library.
*****************************************************************/
class Audio {

    const int sampleBufferSize = 8192;
    const long soundFrameLength = 20000;
    
    constexpr static const byte soundMask[] = {
        0x80, 0x3F, 0x00, 0xFF, 0xBF,                       // NR10-NR14 (0xFF10-0xFF14)
        0xFF, 0x3F, 0x00, 0xFF, 0xBF,                       // NR20-NR24 (0xFF15-0xFF19)
        0x7F, 0xFF, 0x9F, 0xFF, 0xBF,                       // NR30-NR34 (0xFF1A-0xFF1E)
        0xFF, 0xFF, 0x00, 0x00, 0xBF,                       // NR40-NR44 (0xFF1F-0xFF23)
        0x00, 0x00, 0x70, 0xFF, 0xFF,                       // NR50-NR54 (0xFF24-0xFF28)
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                       // --------- (0xFF29-0xFF2D)
        0xFF, 0xFF,                                         // --------- (0xFF2E-0xFF2F)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // WaveRAM - (0xFF30-0xFF37)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // WaveRAM - (0xFF38-0xFF3F)
    };
    
public:
    Audio();
    ~Audio();
    void reset();
    void enable(bool);
    bool isEnabled() const;
    void setSampleRate(int);
    byte readAudioRegister(word);
    void writeAudioRegister(word, byte);
    void endFrame();
    void update(unsigned int);
    
private:
    bool soundEnabled;
    Gb_Apu* apu;
    Stereo_Buffer* stereoBuffer;
    long time;
    Sound_Queue* soundQueue;
    int sampleRate;
    blip_sample_t* sampleBuffer;
};


#endif
