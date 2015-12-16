#ifndef _MEMORY_MBC3_H_
#define _MEMORY_MBC3_H_

#include "Memory.h"

/**************************************************************************//**
 *
 * \brief Specific memory type that implements the MBC3 chip
 *
******************************************************************************/

class MemoryMBC3 : public Memory {

private:
    int currentRAMBank;
    int currentROMBank;
    bool ramEnabled;
    bool RTCEnabled;
    byte* ramBanks;
    int RTCSeconds;
    int RTCMinutes;
    int RTCHours;
    int RTCDays;
    int RTCControl;
    int RTCLatchedSeconds;
    int RTCLatchedMinutes;
    int RTCLatchedHours;
    int RTCLatchedDays;
    int RTCLatchedControl;
    int RTCLatch;
    byte RTCRegister;
    int RTCLastTime;
    int RTCLastTimeCache;
    int currentROMAddress;
    int currentRAMAddress;
    
    void updateRTC();
    
public:
    MemoryMBC3(Cartridge *, Audio *, Joypad *);
    byte readWithChip(const word) const;
    void writeWithChip(const word, const byte);
};


#endif
