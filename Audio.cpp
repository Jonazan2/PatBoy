#include "Audio.h"
#include "Sound_Queue.h"
#include "Gb_Apu.h"

Audio::Audio() {
    soundEnabled = true;
    time = 0;
    sampleRate = 44100;
    
    if ( SDL_Init(SDL_INIT_AUDIO) < 0 ) {
        printf("Error starting SDL Library\n\n\n");
    }
    
    atexit(SDL_Quit);
    
    sampleBuffer = new blip_sample_t[sampleBufferSize];
    
    apu = new Gb_Apu();
    stereoBuffer = new Stereo_Buffer();
    soundQueue = new Sound_Queue();
    
    stereoBuffer->clock_rate(4194304);
    stereoBuffer->set_sample_rate(sampleRate);
    
    apu->treble_eq(-15.0);
    stereoBuffer->bass_freq(100);
    
    apu->set_output(stereoBuffer->center(), stereoBuffer->left(), stereoBuffer->right());
    
    soundQueue->start(sampleRate, 2);
}

Audio::~Audio() {
    delete stereoBuffer;
}

void Audio::reset() {
    soundEnabled = true;
    Gb_Apu::mode_t mode = Gb_Apu::mode_dmg;
    apu->reset(mode);
    stereoBuffer->clear();
    
    for (int reg = 0xFF10; reg <= 0xFF3F; reg++) {
        byte value = initialValuesForFFXX[reg - 0xFF00];
        apu->write_register(0, reg, value);
    }
    
    time = 0;
}

void Audio::enable(bool enabled) {
    this->soundEnabled = enabled;
}

bool Audio::isEnabled() const {
    return soundEnabled;
}

void Audio::setSampleRate(int rate) {
    if ( rate != sampleRate ) {
        sampleRate = rate;
        stereoBuffer->set_sample_rate(sampleRate);
        soundQueue->stop();
        soundQueue->start(sampleRate, 2);
    }
}

byte Audio::readAudioRegister(word address) {
    return apu->read_register((int)time, address);
}

void Audio::writeAudioRegister(word address, byte value) {
    apu->write_register((int)time, address, value);
}

void Audio::endFrame() {
    apu->end_frame((int) soundFrameLength);
    stereoBuffer->end_frame((int) soundFrameLength);

    if ( stereoBuffer->samples_avail() >= sampleBufferSize ) {
        long count = stereoBuffer->read_samples(sampleBuffer, sampleBufferSize);
        if ( soundEnabled ) {
            soundQueue->write(sampleBuffer, (int) count);
        }
    }
}

void Audio::update(unsigned int clockCycles) {
    
    time += clockCycles;
    
    if ( time >= soundFrameLength ) {
        time -= soundFrameLength;
        endFrame();
    }
}
