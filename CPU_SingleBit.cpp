#include "CPU.h"

////////////////////////////////////////// BIT

/*!
 Method that checks a specific bit from a register.
 If the bit checked is set, then the zero flag is raised and cleared otherwise.
 The half carry flag is raised and the add sub flag cleared.
*/
void CPU::bit(const byte reg, const short position) {
    if ( utils->isBitSet(reg, position) ) {
        clearFlag(ZERO_FLAG);
    } else {
        raiseFlag(ZERO_FLAG);
    }
    raiseFlag(HALF_CARRY_FLAG);
    clearFlag(ADD_SUB_FLAG);
}

short CPU::extendedOpcode0x40() {
    bit(BC.hi, 0);
    return 8;
}// bit 0, B

short CPU::extendedOpcode0x41() {
    bit(BC.low, 0);
    return 8;
}// bit 0, C

short CPU::extendedOpcode0x42() {
    bit(DE.hi, 0);
    return 8;
}// bit 0, D

short CPU::extendedOpcode0x43() {
    bit(DE.low, 0);
    return 8;
}// bit 0, E

short CPU::extendedOpcode0x44() {
    bit(HL.hi, 0);
    return 8;
}// bit 0, H

short CPU::extendedOpcode0x45() {
    bit(HL.low, 0);
    return 8;
}// bit 0, L

short CPU::extendedOpcode0x46() {
    byte data = memory->read(HL.value);
    bit(data, 0);
    return 12;
}// bit 0, (HL)

short CPU::extendedOpcode0x47() {
    bit(AF.hi, 0);
    return 8;
}// bit 0, A


short CPU::extendedOpcode0x48() {
    bit(BC.hi, 1);
    return 8;
}// bit 1, B

short CPU::extendedOpcode0x49() {
    bit(BC.low, 1);
    return 8;
}// bit 1, C

short CPU::extendedOpcode0x4A() {
    bit(DE.hi, 1);
    return 8;
}// bit 1, D

short CPU::extendedOpcode0x4B() {
    bit(DE.low, 1);
    return 8;
}// bit 1, E

short CPU::extendedOpcode0x4C() {
    bit(HL.hi, 1);
    return 8;
}// bit 1, H

short CPU::extendedOpcode0x4D() {
    bit(HL.low, 1);
    return 8;
}// bit 1, L

short CPU::extendedOpcode0x4E() {
    byte data = memory->read(HL.value);
    bit(data, 1);
    return 12;
}// bit 1, (HL)

short CPU::extendedOpcode0x4F() {
    bit(AF.hi, 1);
    return 8;
}// bit 1, A


short CPU::extendedOpcode0x50() {
    bit(BC.hi, 2);
    return 8;
}// bit 2, B

short CPU::extendedOpcode0x51() {
    bit(BC.low, 2);
    return 8;
}// bit 2, C

short CPU::extendedOpcode0x52() {
    bit(DE.hi, 2);
    return 8;
}// bit 2, D

short CPU::extendedOpcode0x53() {
    bit(DE.low, 2);
    return 8;
}// bit 2, E

short CPU::extendedOpcode0x54() {
    bit(HL.hi, 2);
    return 8;
}// bit 2, H

short CPU::extendedOpcode0x55() {
    bit(HL.low, 2);
    return 8;
}// bit 2, L

short CPU::extendedOpcode0x56() {
    byte data = memory->read(HL.value);
    bit(data, 2);
    return 12;
}// bit 2, (HL)

short CPU::extendedOpcode0x57() {
    bit(AF.hi, 2);
    return 8;
}// bit 2, A


short CPU::extendedOpcode0x58() {
    bit(BC.hi, 3);
    return 8;
}// bit 3, B

short CPU::extendedOpcode0x59() {
    bit(BC.low, 3);
    return 8;
}// bit 3, C

short CPU::extendedOpcode0x5A() {
    bit(DE.hi, 3);
    return 8;
}// bit 3, D

short CPU::extendedOpcode0x5B() {
    bit(DE.low, 3);
    return 8;
}// bit 3, E

short CPU::extendedOpcode0x5C() {
    bit(HL.hi, 3);
    return 8;
}// bit 3, H

short CPU::extendedOpcode0x5D() {
    bit(HL.low, 3);
    return 8;
}// bit 3, L

short CPU::extendedOpcode0x5E() {
    byte data = memory->read(HL.value);
    bit(data, 3);
    return 12;
}// bit 3, (HL)

short CPU::extendedOpcode0x5F() {
    bit(AF.hi, 3);
    return 8;
}// bit 3, A


short CPU::extendedOpcode0x60() {
    bit(BC.hi, 4);
    return 8;
}// bit 4, B

short CPU::extendedOpcode0x61() {
    bit(BC.low, 4);
    return 8;
}// bit 4, C

short CPU::extendedOpcode0x62() {
    bit(DE.hi, 4);
    return 8;
}// bit 4, D

short CPU::extendedOpcode0x63() {
    bit(DE.low, 4);
    return 8;
}// bit 4, E

short CPU::extendedOpcode0x64() {
    bit(HL.hi, 4);
    return 8;
}// bit 4, H

short CPU::extendedOpcode0x65() {
    bit(HL.low, 4);
    return 8;
}// bit 4, L

short CPU::extendedOpcode0x66() {
    byte data = memory->read(HL.value);
    bit(data, 4);
    return 12;
}// bit 4, (HL)

short CPU::extendedOpcode0x67() {
    bit(AF.hi, 4);
    return 8;
}// bit 4, A


short CPU::extendedOpcode0x68() {
    bit(BC.hi, 5);
    return 8;
}// bit 5, B

short CPU::extendedOpcode0x69() {
    bit(BC.low, 5);
    return 8;
}// bit 5, C

short CPU::extendedOpcode0x6A() {
    bit(DE.hi, 5);
    return 8;
}// bit 5, D

short CPU::extendedOpcode0x6B() {
    bit(DE.low, 5);
    return 8;
}// bit 5, E

short CPU::extendedOpcode0x6C() {
    bit(HL.hi, 5);
    return 8;
}// bit 5, H

short CPU::extendedOpcode0x6D() {
    bit(HL.low, 5);
    return 8;
}// bit 5, L

short CPU::extendedOpcode0x6E() {
    byte data = memory->read(HL.value);
    bit(data, 5);
    return 12;
}// bit 5, (HL)

short CPU::extendedOpcode0x6F() {
    bit(AF.hi, 5);
    return 8;
}// bit 5, A


short CPU::extendedOpcode0x70() {
    bit(BC.hi, 6);
    return 8;
}// bit 6, B

short CPU::extendedOpcode0x71() {
    bit(BC.low, 6);
    return 8;
}// bit 6, C

short CPU::extendedOpcode0x72() {
    bit(DE.hi, 6);
    return 8;
}// bit 6, D

short CPU::extendedOpcode0x73() {
    bit(DE.low, 6);
    return 8;
}// bit 6, E

short CPU::extendedOpcode0x74() {
    bit(HL.hi, 6);
    return 8;
}// bit 6, H

short CPU::extendedOpcode0x75() {
    bit(HL.low, 6);
    return 8;
}// bit 6, L

short CPU::extendedOpcode0x76() {
    byte data = memory->read(HL.value);
    bit(data, 6);
    return 12;
}// bit 6, (HL)

short CPU::extendedOpcode0x77() {
    bit(AF.hi, 6);
    return 8;
}// bit 6, A


short CPU::extendedOpcode0x78() {
    bit(BC.hi, 7);
    return 8;
}// bit 7, B

short CPU::extendedOpcode0x79() {
    bit(BC.low, 7);
    return 8;
}// bit 7, C

short CPU::extendedOpcode0x7A() {
    bit(DE.hi, 7);
    return 8;
}// bit 7, D

short CPU::extendedOpcode0x7B() {
    bit(DE.low, 7);
    return 8;
}// bit 7, E

short CPU::extendedOpcode0x7C() {
    bit(HL.hi, 7);
    return 8;
}// bit 7, H

short CPU::extendedOpcode0x7D() {
    bit(HL.low, 7);
    return 8;
}// bit 7, L

short CPU::extendedOpcode0x7E() {
    byte data = memory->read(HL.value);
    bit(data, 7);
    return 12;
}// bit 7, (HL)

short CPU::extendedOpcode0x7F() {
    bit(AF.hi, 7);
    return 8;
}// bit 7, A

//////////////////////////////////////// RES

/*!
 Method that reset a specific bit from a register.
 */
void CPU::res(byte *reg, const short position) {
    utils->clearBit(reg, position);
}

short CPU::extendedOpcode0x80() {
    res(&BC.hi, 0);
    return 8;
}// res 0, B

short CPU::extendedOpcode0x81() {
    res(&BC.low, 0);
    return 8;
}// res 0, C

short CPU::extendedOpcode0x82() {
    res(&DE.hi, 0);
    return 8;
}// res 0, D

short CPU::extendedOpcode0x83() {
    res(&DE.low, 0);
    return 8;
}// res 0, E

short CPU::extendedOpcode0x84() {
    res(&HL.hi, 0);
    return 8;
}// res 0, H

short CPU::extendedOpcode0x85() {
    res(&HL.low, 0);
    return 8;
}// res 0, L

short CPU::extendedOpcode0x86() {
    byte data = memory->read(HL.value);
    utils->clearBit(&data, 0);
    memory->write(HL.value, data);
    return 16;
}// res 0, (HL)

short CPU::extendedOpcode0x87() {
    res(&AF.hi, 0);
    return 8;
}// res 0, A


short CPU::extendedOpcode0x88() {
    res(&BC.hi, 1);
    return 8;
}// res 1, B

short CPU::extendedOpcode0x89() {
    res(&BC.low, 1);
    return 8;
}// res 1, C

short CPU::extendedOpcode0x8A() {
    res(&DE.hi, 1);
    return 8;
}// res 1, D

short CPU::extendedOpcode0x8B() {
    res(&DE.low, 1);
    return 8;
}// res 1, E

short CPU::extendedOpcode0x8C() {
    res(&HL.hi, 1);
    return 8;
}// res 1, H

short CPU::extendedOpcode0x8D() {
    res(&HL.low, 1);
    return 8;
}// res 1, L

short CPU::extendedOpcode0x8E() {
    byte data = memory->read(HL.value);
    utils->clearBit(&data, 1);
    memory->write(HL.value, data);
    return 16;
}// res 1, (HL)

short CPU::extendedOpcode0x8F() {
    res(&AF.hi, 1);
    return 8;
}// res 1, A


short CPU::extendedOpcode0x90() {
    res(&BC.hi, 2);
    return 8;
}// res 2, B

short CPU::extendedOpcode0x91() {
    res(&BC.low, 2);
    return 8;
}// res 2, C

short CPU::extendedOpcode0x92() {
    res(&DE.hi, 2);
    return 8;
}// res 2, D

short CPU::extendedOpcode0x93() {
    res(&DE.low, 2);
    return 8;
}// res 2, E

short CPU::extendedOpcode0x94() {
    res(&HL.hi, 2);
    return 8; // res 2, H
}

short CPU::extendedOpcode0x95() {
    res(&HL.low, 2);
    return 8;
}// res 2, L

short CPU::extendedOpcode0x96() {
    byte data = memory->read(HL.value);
    utils->clearBit(&data, 2);
    memory->write(HL.value, data);
    return 16;
}// res 2, (HL)

short CPU::extendedOpcode0x97() {
    res(&AF.hi, 2);
    return 8;
}// res 2, A


short CPU::extendedOpcode0x98() {
    res(&BC.hi, 3);
    return 8;
}// res 3, B

short CPU::extendedOpcode0x99() {
    res(&BC.low, 3);
    return 8;
}// res 3, C

short CPU::extendedOpcode0x9A() {
    res(&DE.hi, 3);
    return 8;
}// res 3, D

short CPU::extendedOpcode0x9B() {
    res(&DE.low, 3);
    return 8;
}// res 3, E

short CPU::extendedOpcode0x9C() {
    res(&HL.hi, 3);
    return 8;
}// res 3, H

short CPU::extendedOpcode0x9D() {
    res(&HL.low, 3);
    return 8;
}// res 3, L

short CPU::extendedOpcode0x9E() {
    byte data = memory->read(HL.value);
    utils->clearBit(&data, 3);
    memory->write(HL.value, data);
    return 16;
}// res 3, (HL)

short CPU::extendedOpcode0x9F() {
    res(&AF.hi, 3);
    return 8;
}// res 3, A


short CPU::extendedOpcode0xA0() {
    res(&BC.hi, 4);
    return 8;
}// res 4, B

short CPU::extendedOpcode0xA1() {
    res(&BC.low, 4);
    return 8;
}// res 4, C

short CPU::extendedOpcode0xA2() {
    res(&DE.hi, 4);
    return 8;
}// res 4, D

short CPU::extendedOpcode0xA3() {
    res(&DE.low, 4);
    return 8;
}// res 4, E

short CPU::extendedOpcode0xA4() {
    res(&HL.hi, 4);
    return 8;
}// res 4, H

short CPU::extendedOpcode0xA5() {
    res(&HL.low, 4);
    return 8;
}// res 4, L

short CPU::extendedOpcode0xA6() {
    byte data = memory->read(HL.value);
    utils->clearBit(&data, 4);
    memory->write(HL.value, data);
    return 16;
}// res 4, (HL)

short CPU::extendedOpcode0xA7() {
    res(&AF.hi, 4);
    return 8;
}// res 4, A


short CPU::extendedOpcode0xA8() {
    res(&BC.hi, 5);
    return 8;
}// res 5, B

short CPU::extendedOpcode0xA9() {
    res(&BC.low, 5);
    return 8;
}// res 5, C

short CPU::extendedOpcode0xAA() {
    res(&DE.hi, 5);
    return 8;
}// res 5, D

short CPU::extendedOpcode0xAB() {
    res(&DE.low, 5);
    return 8;
}// res 5, E

short CPU::extendedOpcode0xAC() {
    res(&HL.hi, 5);
    return 8;
}// res 5, H

short CPU::extendedOpcode0xAD() {
    res(&HL.low, 5);
    return 8;
}// res 5, L

short CPU::extendedOpcode0xAE() {
    byte data = memory->read(HL.value);
    utils->clearBit(&data, 5);
    memory->write(HL.value, data);
    return 16;
}// res 5, (HL)

short CPU::extendedOpcode0xAF() {
    res(&AF.hi, 5);
    return 8;
}// res 5, A


short CPU::extendedOpcode0xB0() {
    res(&BC.hi, 6);
    return 8;
}// res 6, B

short CPU::extendedOpcode0xB1() {
    res(&BC.low, 6);
    return 8;
}// res 6, C

short CPU::extendedOpcode0xB2() {
    res(&DE.hi, 6);
    return 8;
}// res 6, D

short CPU::extendedOpcode0xB3() {
    res(&DE.low, 6);
    return 8;
}// res 6, E

short CPU::extendedOpcode0xB4() {
    res(&HL.hi, 6);
    return 8;
}// res 6, H

short CPU::extendedOpcode0xB5() {
    res(&HL.low, 6);
    return 8;
}// res 6, L

short CPU::extendedOpcode0xB6() {
    byte data = memory->read(HL.value);
    utils->clearBit(&data, 6);
    memory->write(HL.value, data);
    return 16;
}// res 6, (HL)

short CPU::extendedOpcode0xB7() {
    res(&AF.hi, 6);
    return 8;
}// res 6, A


short CPU::extendedOpcode0xB8() {
    res(&BC.hi, 7);
    return 8;
}// res 7, B

short CPU::extendedOpcode0xB9() {
    res(&BC.low, 7);
    return 8;
}// res 7, C

short CPU::extendedOpcode0xBA() {
    res(&DE.hi, 7);
    return 8;
}// res 7, D

short CPU::extendedOpcode0xBB() {
    res(&DE.low, 7);
    return 8;
}// res 7, E

short CPU::extendedOpcode0xBC() {
    res(&HL.hi, 7);
    return 8;
}// res 7, H

short CPU::extendedOpcode0xBD() {
    res(&HL.low, 7);
    return 8;
}// res 7, L

short CPU::extendedOpcode0xBE() {
    byte data = memory->read(HL.value);
    utils->clearBit(&data, 7);
    memory->write(HL.value, data);
    return 16;
}// res 7, (HL)

short CPU::extendedOpcode0xBF() {
    res(&AF.hi, 7);
    return 8;
}// res 7, A

//////////////////////////////////////// SET

/*!
 Method that sets a specific bit from a register.
 */
void CPU::set(byte * reg, const short position) {
    utils->setBit(reg, position);
}

short CPU::extendedOpcode0xC0() {
    set(&BC.hi, 0);
    return 8;
}// set 0, B

short CPU::extendedOpcode0xC1() {
    set(&BC.low, 0);
    return 8;
}// set 0, C

short CPU::extendedOpcode0xC2() {
    set(&DE.hi, 0);
    return 8;
}// set 0, D

short CPU::extendedOpcode0xC3() {
    set(&DE.low, 0);
    return 8;
}// set 0, E

short CPU::extendedOpcode0xC4() {
    set(&HL.hi, 0);
    return 8;
}// set 0, H

short CPU::extendedOpcode0xC5() {
    set(&HL.low, 0);
    return 8;
}// set 0, L

short CPU::extendedOpcode0xC6() {
    byte data = memory->read(HL.value);
    utils->setBit(&data, 0);
    memory->write(HL.value, data);
    return 12;
}// set 0, (HL)

short CPU::extendedOpcode0xC7() {
    set(&AF.hi, 0);
    return 8;
}// set 0, A


short CPU::extendedOpcode0xC8() {
    set(&BC.hi, 1);
    return 8;
}// set 1, B

short CPU::extendedOpcode0xC9() {
    set(&BC.low, 1);
    return 8;
}// set 1, C

short CPU::extendedOpcode0xCA() {
    set(&DE.hi, 1);
    return 8;
}// set 1, D

short CPU::extendedOpcode0xCB() {
    set(&DE.low, 1);
    return 8;
}// set 1, E

short CPU::extendedOpcode0xCC() {
    set(&HL.hi, 1);
    return 8;
}// set 1, H

short CPU::extendedOpcode0xCD() {
    set(&HL.low, 1);
    return 8;
}// set 1, L

short CPU::extendedOpcode0xCE() {
    byte data = memory->read(HL.value);
    utils->setBit(&data, 1);
    memory->write(HL.value, data);
    return 12;
}// set 1, (HL)

short CPU::extendedOpcode0xCF() {
    set(&AF.hi, 1);
    return 8;
}// set 1, A


short CPU::extendedOpcode0xD0() {
    set(&BC.hi, 2);
    return 8;
}// set 2, B

short CPU::extendedOpcode0xD1() {
    set(&BC.low, 2);
    return 8;
}// set 2, C

short CPU::extendedOpcode0xD2() {
    set(&DE.hi, 2);
    return 8;
}// set 2, D

short CPU::extendedOpcode0xD3() {
    set(&DE.low, 2);
    return 8;
}// set 2, E

short CPU::extendedOpcode0xD4() {
    set(&HL.hi, 2);
    return 8;
}// set 2, H

short CPU::extendedOpcode0xD5() {
    set(&HL.low, 2);
    return 8;
}// set 2, L

short CPU::extendedOpcode0xD6() {
    byte data = memory->read(HL.value);
    utils->setBit(&data, 2);
    memory->write(HL.value, data);
    return 12;
}// set 2, (HL)

short CPU::extendedOpcode0xD7() {
    set(&AF.hi, 2);
    return 8;
}// set 2, A


short CPU::extendedOpcode0xD8() {
    set(&BC.hi, 3);
    return 8;
}// set 3, B

short CPU::extendedOpcode0xD9() {
    set(&BC.low, 3);
    return 8;
}// set 3, C

short CPU::extendedOpcode0xDA() {
    set(&DE.hi, 3);
    return 8;
}// set 3, D

short CPU::extendedOpcode0xDB() {
    set(&DE.low, 3);
    return 8;
}// set 3, E

short CPU::extendedOpcode0xDC() {
    set(&HL.hi, 3);
    return 8;
}// set 3, H

short CPU::extendedOpcode0xDD() {
    set(&HL.low, 3);
    return 8;
}// set 3, L

short CPU::extendedOpcode0xDE() {
    byte data = memory->read(HL.value);
    utils->setBit(&data, 3);
    memory->write(HL.value, data);
    return 12;
}// set 3, (HL)

short CPU::extendedOpcode0xDF() {
    set(&AF.hi, 3);
    return 8;
}// set 3, A


short CPU::extendedOpcode0xE0() {
    set(&BC.hi, 4);
    return 8;
}// set 4, B

short CPU::extendedOpcode0xE1() {
    set(&BC.low, 4);
    return 8;
}// set 4, C

short CPU::extendedOpcode0xE2() {
    set(&DE.hi, 4);
    return 8;
}// set 4, D

short CPU::extendedOpcode0xE3() {
    set(&DE.low, 4);
    return 8;
}// set 4, E

short CPU::extendedOpcode0xE4() {
    set(&HL.hi, 4);
    return 8;
}// set 4, H

short CPU::extendedOpcode0xE5() {
    set(&HL.low, 4);
    return 8;
}// set 4, L

short CPU::extendedOpcode0xE6() {
    byte data = memory->read(HL.value);
    utils->setBit(&data, 4);
    memory->write(HL.value, data);
    return 12;
}// set 4, (HL)

short CPU::extendedOpcode0xE7() {
    set(&AF.hi, 4);
    return 8;
}// set 4, A


short CPU::extendedOpcode0xE8() {
    set(&BC.hi, 5);
    return 8;
}// set 5, B

short CPU::extendedOpcode0xE9() {
    set(&BC.low, 5);
    return 8;
}// set 5, C

short CPU::extendedOpcode0xEA() {
    set(&DE.hi, 5);
    return 8;
}// set 5, D

short CPU::extendedOpcode0xEB() {
    set(&DE.low, 5);
    return 8;
}// set 5, E

short CPU::extendedOpcode0xEC() {
    set(&HL.hi, 5);
    return 8;
}// set 5, H

short CPU::extendedOpcode0xED() {
    set(&HL.low, 5);
    return 8;
}// set 5, L

short CPU::extendedOpcode0xEE() {
    byte data = memory->read(HL.value);
    utils->setBit(&data, 5);
    memory->write(HL.value, data);
    return 12;
}// set 5, (HL)

short CPU::extendedOpcode0xEF() {
    set(&AF.hi, 5);
    return 8;
}// set 5, A


short CPU::extendedOpcode0xF0() {
    set(&BC.hi, 6);
    return 8;
}// set 6, B

short CPU::extendedOpcode0xF1() {
    set(&BC.low, 6);
    return 8;
}// set 6, C

short CPU::extendedOpcode0xF2() {
    set(&DE.hi, 6);
    return 8;
}// set 6, D

short CPU::extendedOpcode0xF3() {
    set(&DE.low, 6);
    return 8;
}// set 6, E

short CPU::extendedOpcode0xF4() {
    set(&HL.hi, 6);
    return 8;
}// set 6, H

short CPU::extendedOpcode0xF5() {
    set(&HL.low, 6);
    return 8;
}// set 6, L

short CPU::extendedOpcode0xF6() {
    byte data = memory->read(HL.value);
    utils->setBit(&data, 6);
    memory->write(HL.value, data);
    return 12;
}// set 6, (HL)

short CPU::extendedOpcode0xF7() {
    set(&AF.hi, 6);
    return 8;
}// set 6, A


short CPU::extendedOpcode0xF8() {
    set(&BC.hi, 7);
    return 8;
}// set 7, B

short CPU::extendedOpcode0xF9() {
    set(&BC.low, 7);
    return 8;
}// set 7, C

short CPU::extendedOpcode0xFA() {
    set(&DE.hi, 7);
    return 8;
}// set 7, D

short CPU::extendedOpcode0xFB() {
    set(&DE.low, 7);
    return 8;
}// set 7, E

short CPU::extendedOpcode0xFC() {
    set(&HL.hi, 7);
    return 8;
}// set 7, H

short CPU::extendedOpcode0xFD() {
    set(&HL.low, 7);
    return 8;
}// set 7, L

short CPU::extendedOpcode0xFE() {
    byte data = memory->read(HL.value);
    utils->setBit(&data, 7);
    memory->write(HL.value, data);
    return 12;
}// set 7, (HL)

short CPU::extendedOpcode0xFF() {
    set(&AF.hi, 7);
    return 8;
}// set 7, A