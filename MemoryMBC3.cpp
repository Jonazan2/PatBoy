#include "MemoryMBC3.h"

MemoryMBC3::MemoryMBC3(Cartridge *cartridge, Audio *audio, Joypad *joypad):Memory(cartridge, audio, joypad)  {
    ramBanks = new byte[0x8000];
    currentRAMBank = 0;
    currentROMBank = 1;
    ramEnabled = false;
    RTCEnabled = false;
    
    for ( int i = 0; i < 0x8000; i++ ) {
        ramBanks[i] = 0xFF;
    }
    
    RTCSeconds = 0;
    RTCMinutes = 0;
    RTCHours = 0;
    RTCDays = 0;
    RTCControl = 0;
    RTCLatchedSeconds = 0;
    RTCLatchedMinutes = 0;
    RTCLatchedHours = 0;
    RTCLatchedDays = 0;
    RTCLatchedControl = 0;
    RTCLatch = 0;
    RTCRegister = 0;
    RTCLastTime = -1;
    RTCLastTimeCache = 0;
    currentROMAddress = 0x4000;
    currentRAMAddress = 0;
}

byte MemoryMBC3::readWithChip(const word address) const {
    switch ( address & 0xE000 ) {
        case 0x4000:
        case 0x6000: {
            byte* rom = cartridge->getRom();
            return rom[(address - 0x4000) + currentROMAddress];
            
        } case 0xA000: {
            if ( currentRAMBank >= 0 ) {
                if ( ramEnabled ) {
                    return ramBanks[(address - 0xA000) + currentRAMAddress];
                } else {
                    return 0xFF;
                }
            } else if ( cartridge->hasRTC() && RTCEnabled ) {
                switch ( RTCRegister ) {
                    case 0x08:
                        return RTCLatchedSeconds;
                        break;
                    case 0x09:
                        return RTCLatchedMinutes;
                        break;
                    case 0x0A:
                        return RTCLatchedHours;
                        break;
                    case 0x0B:
                        return RTCLatchedDays;
                        break;
                    case 0x0C:
                        return RTCLatchedControl;
                        break;
                    default:
                        return 0xFF;
                }
            }  else {
                return 0xFF;
            }
            
        }default: {
            return this->readDirectly(address);
        }
    }
}

void MemoryMBC3::writeWithChip(const word address, const byte data) {
    switch ( address & 0xE000 ) {
        case 0x0000: {
            if ( cartridge->getRamSize() > 0 ) {
                ramEnabled = (data & 0x0F) == 0x0A;
            }
            RTCEnabled = (data & 0x0F) == 0x0A;
            break;
            
        } case 0x2000: {
            currentROMBank = data & 0x7F;
            if ( currentROMBank == 0 ) {
                currentROMBank = 1;
            }
            currentROMBank &= (cartridge->getROMBanks() - 1);
            currentROMAddress = currentROMBank * 0x4000;
            break;
            
        } case 0x4000: {
            if ( (data >= 0x08) && (data <= 0x0C) ) {
                if ( cartridge->hasRTC() && RTCEnabled ) {
                    RTCRegister = data;
                    currentRAMBank = -1;
                }
            } else if ( data <= 0x03 ) {
                currentRAMBank = data;
                currentRAMBank &= (cartridge->getRamBanks() - 1);
                currentRAMAddress = currentRAMBank * 0x2000;
            }
            break;
            
        } case 0x6000: {
            if ( cartridge->hasRTC() ) {
                
                if ( (RTCLatch == 0x00) && (data == 0x01) ) {
                    updateRTC();
                    RTCLatchedSeconds = RTCSeconds;
                    RTCLatchedMinutes = RTCMinutes;
                    RTCLatchedHours = RTCHours;
                    RTCLatchedDays = RTCDays;
                    RTCLatchedControl = RTCControl;
                }
                
                if ((data == 0x00) || (data == 0x01)) {
                    RTCLatch = data;
                }
            }
            break;
            
        } case 0xA000: {
            if ( currentRAMBank >= 0 ) {
                if ( ramEnabled ) {
                    ramBanks[(address - 0xA000) + currentRAMAddress] = data;
                }
                
            } else if ( cartridge->hasRTC() && RTCEnabled ) {
                RTCLastTime = cartridge->getCurrentRTCValue();
                switch ( RTCRegister ) {
                    case 0x08:
                        RTCSeconds = data;
                        break;
                    case 0x09:
                        RTCMinutes = data;
                        break;
                    case 0x0A:
                        RTCHours = data;
                        break;
                    case 0x0B:
                        RTCDays = data;
                        break;
                    case 0x0C:
                        if ( RTCControl & 0x80 )
                            RTCControl = 0x80 | data;
                        else
                            RTCControl = data;
                        break;
                }
            }
            break;
        } default: {
            this->writeDirectly(address, data);
            break;
        }
    }
}

void MemoryMBC3::updateRTC()
{
    int now = cartridge->getCurrentRTCValue();
    
    if ( RTCLastTimeCache != now ) {
        RTCLastTimeCache = now;
        long difference = now - RTCLastTime;
        
        if ( difference > 0 ) {
            RTCSeconds += (int) (difference % 60);
            if (RTCSeconds > 59) {
                RTCSeconds -= 60;
                RTCMinutes++;
            }
            
            difference /= 60;
            
            RTCMinutes += (int) (difference % 60);
            if ( RTCMinutes > 59 ) {
                RTCMinutes -= 60;
                RTCHours++;
            }
            
            difference /= 60;
            
            RTCHours += (int) (difference % 24);
            if ( RTCHours > 23 ) {
                RTCHours -= 24;
                RTCDays++;
            }
            difference /= 24;
            
            RTCDays += (int) (difference & 0xffffffff);
            if ( RTCDays > 0xFF ) {
                if ( RTCDays > 511 ) {
                    RTCDays %= 512;
                    RTCControl |= 0x80;
                }
                RTCControl = (RTCControl & 0xFF) | (RTCDays > 0xFF ? 1 : 0);
            }
        }
        RTCLastTime = now;
    }
}