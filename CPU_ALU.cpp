#include "CPU.h"

short CPU::opcode0x03() {
    instructionSet->increment16BitRegister(&BC);
    return 8;
}

short CPU::opcode0x13() {
    instructionSet->increment16BitRegister(&DE);
    return 8;
}

short CPU::opcode0x23() {
    instructionSet->increment16BitRegister(&HL);
    return 8;
}

short CPU::opcode0x33() {
    instructionSet->increment16BitRegister(&SP);
    return 8;
}

/////////////////////////////////// 8 bit increments z0h
void CPU::increment8BitRegister(byte *reg) {
    byte before = *reg;
    *reg = *reg +1;
    
    clearFlag(ADD_SUB_FLAG);
    checkZeroFlag(*reg);
    if ( (before & 0xF) == 0xF ) {
		raiseFlag(HALF_CARRY_FLAG);
	} else {
		clearFlag(HALF_CARRY_FLAG);
    }
}

short CPU::opcode0x3C() {
    increment8BitRegister(&AF.hi);
    return 4;
}// A increment 4

short CPU::opcode0x04() {
    increment8BitRegister(&BC.hi);
    return 4;
}// B increment

short CPU::opcode0x0C() {
    increment8BitRegister(&BC.low);
    return 4;
}// C increment

short CPU::opcode0x14() {
    increment8BitRegister(&DE.hi);
    return 4;
}// D increment

short CPU::opcode0x1C() {
    increment8BitRegister(&DE.low);
    return 4;
}// E increment

short CPU::opcode0x24() {
    increment8BitRegister(&HL.hi);
    return 4;
}// H increment

short CPU::opcode0x2C() {
    increment8BitRegister(&HL.low);
    return 4;
}// L increment

short CPU::opcode0x34() {
    byte result = memory->read(HL.value) + 1;
    memory->write(HL.value, result);
    
    clearFlag(ADD_SUB_FLAG);
    checkZeroFlag(result);
    checkHalfCarryFlag(result);
    return 12;
}// HL increment

///////////////////////////////////// 16 bits decrements
void CPU::decrement16BitRegister(Register *reg) {
    reg->value--;
}

short CPU::opcode0x0B() {
    decrement16BitRegister(&BC);
    return 8;
} // BC

short CPU::opcode0x1B() {
    decrement16BitRegister(&DE);
    return 8;
} // DE

short CPU::opcode0x2B() {
    decrement16BitRegister(&HL);
    return 8;
} // HL

short CPU::opcode0x3B() {
    decrementStackPointer();
    return 8;
} // SP

/////////////////////////////////// 8 bits decrements z1h FLAGS implemented
void CPU::decrement8BitRegister(byte *reg) {
    byte before = *reg;
    *reg = *reg -1;
    
    raiseFlag(ADD_SUB_FLAG);
    checkZeroFlag(*reg);
    if ( (before & 0x0F) == 0x00 ) {
        raiseFlag(HALF_CARRY_FLAG);
    } else {
        clearFlag(HALF_CARRY_FLAG);
    }
}

short CPU::opcode0x05() {
    decrement8BitRegister(&BC.hi);
    return 4;
}// DEC B 4

short CPU::opcode0x0D() {
    decrement8BitRegister(&BC.low);
    return 4;
}// DEC C

short CPU::opcode0x15() {
    decrement8BitRegister(&DE.hi);
    return 4;
}// DEC D

short CPU::opcode0x1D() {
    decrement8BitRegister(&DE.low);
    return 4;
}// DEC E

short CPU::opcode0x25() {
    decrement8BitRegister(&HL.hi);
    return 4;
}// DEC H

short CPU::opcode0x2D() {
    decrement8BitRegister(&HL.low);
    return 4;
}// DEC L

short CPU::opcode0x3D() {
    decrement8BitRegister(&AF.hi);
    return 4;
}// DEC A

short CPU::opcode0x35() {
    byte before = memory->read(HL.value);
    byte data = memory->read(HL.value) - 1;
    memory->write(HL.value, data);
    
    raiseFlag(ADD_SUB_FLAG);
    checkZeroFlag(data);
    checkHalfCarryFlag(before);
    
    return 12;
}// DEC HL 12 z1h

/////////////////////////////////////////////////////// 16 bits add operations 0hc
void CPU::add16BitHLRegister(const Register reg) {
	word before = HL.value;
	HL.value += reg.value;
    
    clearFlag(ADD_SUB_FLAG);
    
	if ((before + reg.value) > 0xFFFF) {
		raiseFlag(CARRY_FLAG);
    } else {
        clearFlag(CARRY_FLAG);
    }
    
	if (( (before & 0xFF00) & 0xF) + ((reg.value >> 8) & 0xF)) {
		raiseFlag(HALF_CARRY_FLAG);
    } else {
        clearFlag(HALF_CARRY_FLAG);
    }
}

short CPU::opcode0x09() {
    add16BitHLRegister(BC);
    return 8;
}// HL = HL + BC 8

short CPU::opcode0x19() {
    add16BitHLRegister(DE);
    return 8;
}// HL = HL + DE

short CPU::opcode0x29() {
    add16BitHLRegister(HL);
    return 8;
}// HL = HL + HL

short CPU::opcode0x39() {
    add16BitHLRegister(SP);
    return 8;
}// HL = HL + SP


///////////////////////////////////////////////////////// 8 bits XOR 4 cycles
void CPU::xor8bitRegister(Register *registerA, const byte data) {
    registerA->hi = registerA->hi ^ data;
    clearFlags();
    checkZeroFlag(registerA->hi);
}

short CPU::opcode0xAF() {
    xor8bitRegister(&AF, AF.hi);
    return 4;
}// XOR A, A

short CPU::opcode0xA8() {
    xor8bitRegister(&AF, BC.hi);
    return 4;
}// XOR A, B

short CPU::opcode0xA9() {
    xor8bitRegister(&AF, BC.low);
    return 4;
}// XOR A, C

short CPU::opcode0xAA() {
    xor8bitRegister(&AF, DE.hi);
    return 4;
}// XOR A, D

short CPU::opcode0xAB() {
    xor8bitRegister(&AF, DE.low);
    return 4;
}// XOR A, E

short CPU::opcode0xAC() {
    xor8bitRegister(&AF, HL.hi);
    return 4;
}// XOR A, H

short CPU::opcode0xAD() {
    xor8bitRegister(&AF, HL.low);
    return 4;
}// XOR A, L

short CPU::opcode0xAE() {
    byte data = memory->read(HL.value);
    xor8bitRegister(&AF, data);
    return 8;
}// XOR A, (HL) 8

short CPU::opcode0xEE() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    xor8bitRegister(&AF, data);
    return 8;
}// XOR A, n 8

///////////////////////////////////////////// 8 bits OR 4 cycles
void CPU::or8bitRegister(Register *registerA, const byte data) {
    registerA->hi = registerA->hi | data;
    clearFlags();
    checkZeroFlag(registerA->hi);
}

short CPU::opcode0xB7() {
    or8bitRegister(&AF, AF.hi);
    return 4;
}// OR A, A

short CPU::opcode0xB0() {
    or8bitRegister(&AF, BC.hi);
    return 4;
}// OR A, B

short CPU::opcode0xB1() {
    or8bitRegister(&AF, BC.low);
    return 4;
}// OR A, C

short CPU::opcode0xB2() {
    or8bitRegister(&AF, DE.hi);
    return 4;
}// OR A, D

short CPU::opcode0xB3() {
    or8bitRegister(&AF, DE.low);
    return 4;
}// OR A, E

short CPU::opcode0xB4() {
    or8bitRegister(&AF, HL.hi);
    return 4;
}// OR A, H

short CPU::opcode0xB5() {
    or8bitRegister(&AF, HL.low);
    return 4;
}// OR A, L

short CPU::opcode0xB6() {
    byte data = memory->read(HL.value);
    or8bitRegister(&AF, data);
    return 8;
}// OR A, (HL) 8 cycles

short CPU::opcode0xF6() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    or8bitRegister(&AF, data);
    return 8;
}// OR A, n 8 cycles

//////////////////////////////////// AND 8 bit register z010
void CPU::and8bitRegister(Register *registerA, const byte data) {
    registerA->hi = registerA->hi & data;
    clearFlags();
    checkZeroFlag(registerA->hi);
    raiseFlag(HALF_CARRY_FLAG);
}

short CPU::opcode0xA7() {
    and8bitRegister(&AF,AF.hi);
    return 4;
}// AND A, A

short CPU::opcode0xA0() {
    and8bitRegister(&AF,BC.hi);
    return 4;
}// AND A, B

short CPU::opcode0xA1() {
    and8bitRegister(&AF,BC.low);
    return 4;
}// AND A, C

short CPU::opcode0xA2() {
    and8bitRegister(&AF,DE.hi);
    return 4;
}// AND A, D

short CPU::opcode0xA3() {
    and8bitRegister(&AF,DE.low);
    return 4;
}// AND A, E

short CPU::opcode0xA4() {
    and8bitRegister(&AF, HL.hi);
    return 4;
}// AND A, H

short CPU::opcode0xA5() {
    and8bitRegister(&AF, HL.low);
    return 4;
}// AND A, L

short CPU::opcode0xA6() {
    byte data = memory->read(HL.value);
    and8bitRegister(&AF,data);
    return 8;
}// AND A, (HL) 8 cycles

short CPU::opcode0xE6(){
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    and8bitRegister(&AF, data);
    return 8;
}// AND A, n 8 cycles

/////////////////////////////////////////////// ADD 8 bits z0hc
void CPU::add8bitRegister(Register *registerA, const byte data) {

    byte before = registerA->hi;
    
	registerA->hi += data ;
    
	clearFlags();
    
    clearFlag(ADD_SUB_FLAG);
    
	if ( registerA->hi == 0 ) {
		clearFlag(ZERO_FLAG);
    }
    
	word htest = (before & 0xF) ;
	htest += (data & 0xF) ;
    
	if ( htest > 0xF ) {
		raiseFlag(HALF_CARRY_FLAG);
    }
        
	if ( (before + data) > 0xFF ) {
		raiseFlag(CARRY_FLAG);
    }
}

short CPU::opcode0x87() {
    add8bitRegister(&AF, AF.hi);
    return 4;
}// ADD A, A

short CPU::opcode0x80() {
    add8bitRegister(&AF, BC.hi);
    return 4;
}// ADD A, B

short CPU::opcode0x81() {
    add8bitRegister(&AF, BC.low);
    return 4;
}// ADD A, C

short CPU::opcode0x82() {
    add8bitRegister(&AF, DE.hi);
    return 4;
}// ADD A, D

short CPU::opcode0x83() {
    add8bitRegister(&AF, DE.low);
    return 4;
}// ADD A, E

short CPU::opcode0x84() {
    add8bitRegister(&AF, HL.hi);
    return 4;
}// ADD A, H

short CPU::opcode0x85() {
    add8bitRegister(&AF, HL.low);
    return 4;
}// ADD A, L

short CPU::opcode0x86() {
    byte data = memory->read(HL.value);
    add8bitRegister(&AF, data);
    return 8;
}// ADD A, (HL) 8

short CPU::opcode0xC6() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    add8bitRegister(&AF, data);
    return 8;
}// ADD A, n 8

///////////////////////////////////////////// ADC A, r - ADC A, nn  - ADC A, (HL)   z0hc
void CPU::adc8bitRegister(Register *registerA, const byte data) {
    byte carry = checkFlag(CARRY_FLAG) ? 1 : 0;
    byte result = registerA->hi + data + carry;
    clearFlags();
    raiseFlag(ADD_SUB_FLAG);
    checkZeroFlag(result);
    if (result > 0xFF) {
        raiseFlag(CARRY_FLAG);
    }
    if ( ((registerA->hi& 0x0F) + (data & 0x0F) + carry) > 0x0F ) {
        raiseFlag(HALF_CARRY_FLAG);
    }
    registerA->hi = static_cast<byte> (result);
}

short CPU::opcode0x8F() {
    adc8bitRegister(&AF, AF.hi);
    return 4;
}// ADC A, A

short CPU::opcode0x88() {
    adc8bitRegister(&AF, AF.hi);
    return 4;
}// ADC A, B

short CPU::opcode0x89() {
    adc8bitRegister(&AF, AF.hi);
    return 4;
}// ADC A, C

short CPU::opcode0x8A() {
    adc8bitRegister(&AF, AF.hi);
    return 4;
}// ADC A, D

short CPU::opcode0x8B() {
    adc8bitRegister(&AF, AF.hi);
    return 4;
}// ADC A, E

short CPU::opcode0x8C() {
    adc8bitRegister(&AF, AF.hi);
    return 4;
}// ADC A, H

short CPU::opcode0x8D() {
    adc8bitRegister(&AF, AF.hi);
    return 4;
}// ADC A, L

short CPU::opcode0x8E() {
    byte data = memory->read(HL.value);
    adc8bitRegister(&AF, data);
    return 8;
}// ADC A, (HL) 8

short CPU::opcode0xCE() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    adc8bitRegister(&AF, data);
    return 8;
}// ADC A, nn 8

////////////////////////////////////////////// 8-bit sub 4 cycles z1hc
void CPU::sub8bitRegister(Register *registerA, const byte data) {
    int result = registerA->hi - data;
    int carrybits = registerA->hi ^ data ^ result;
    registerA->hi = static_cast<byte> (result);
    
    clearFlags();
    raiseFlag(ADD_SUB_FLAG);
    checkZeroFlag(static_cast<byte> (result));
    if ( (carrybits & 0x100) != 0 ) {
        raiseFlag(CARRY_FLAG);
    }
    
    if ( (carrybits & 0x10) != 0 ) {
        raiseFlag(HALF_CARRY_FLAG);
    }
}

short CPU::opcode0x97() {
    sub8bitRegister(&AF, AF.hi);
    return 4;
}// SUB A, A

short CPU::opcode0x90() {
    sub8bitRegister(&AF, BC.hi);
    return 4;
}// SUB A, B

short CPU::opcode0x91() {
    sub8bitRegister(&AF, BC.low);
    return 4;
}// SUB A, C

short CPU::opcode0x92() {
    sub8bitRegister(&AF, DE.hi);
    return 4;
}// SUB A, D

short CPU::opcode0x93() {
    sub8bitRegister(&AF, DE.low);
    return 4;
}// SUB A, E

short CPU::opcode0x94() {
    sub8bitRegister(&AF, HL.hi);
    return 4;
}// SUB A, H

short CPU::opcode0x95() {
    sub8bitRegister(&AF, HL.low);
    return 4;
}// SUB A, L

short CPU::opcode0x96() {
    byte data = memory->read(HL.value);
    sub8bitRegister(&AF, data);
    return 8;
}// SUB A, (HL) 8

short CPU::opcode0xD6() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    sub8bitRegister(&AF, data);
    return 8;
}// SUB A, n 8

///////////////////////////////////////////// 8-bit sbc (with carry flag) 4 cycles z1hc
void CPU::sbc8bitRegister(Register *registerA, const byte data){
    int carry = checkFlag(CARRY_FLAG) ? 1 : 0;
    int result = registerA->hi - data - carry;
    clearFlags();
    raiseFlag(ADD_SUB_FLAG);
    
    checkZeroFlag(static_cast<byte> (result));
    if ( result < 0 ) {
        raiseFlag(CARRY_FLAG);
    }
    
    if ( ((registerA->hi & 0x0F) - (data & 0x0F) - carry) < 0 ) {
        raiseFlag(HALF_CARRY_FLAG);
    }
    registerA->hi = static_cast<byte> (result);
}

short CPU::opcode0x9F() {
    sbc8bitRegister(&AF, AF.hi);
    return 4;
}// SBC A, A

short CPU::opcode0x98() {
    sbc8bitRegister(&AF, BC.hi);
    return 4;
}// SBC A, B

short CPU::opcode0x99() {
    sbc8bitRegister(&AF, BC.low);
    return 4;
}// SBC A, C

short CPU::opcode0x9A() {
    sbc8bitRegister(&AF, DE.hi);
    return 4;
}// SBC A, D

short CPU::opcode0x9B() {
    sbc8bitRegister(&AF, DE.low);
    return 4;
}// SBC A, E

short CPU::opcode0x9C() {
    sbc8bitRegister(&AF, HL.hi);
    return 4;
}// SBC A, H

short CPU::opcode0x9D() {
    sbc8bitRegister(&AF, HL.low);
    return 4;
}// SBC A, L

short CPU::opcode0x9E() {
    byte data = memory->read(HL.value);
    sbc8bitRegister(&AF, data);
    return 8;
}// SBC A, (HL) 8

short CPU::opcode0xDE() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    sbc8bitRegister(&AF, data);
    return 8;
}// SBC A, n 8


//////////////////////////////////CP z1hc
void CPU::compare8bitRegister(const Register registerA, const byte data) {
    clearFlags();
    raiseFlag(ADD_SUB_FLAG);
    if ( registerA.hi < data ) {
        raiseFlag(CARRY_FLAG);
    }
    
    if ( registerA.hi == data ) {
        raiseFlag(ZERO_FLAG);
    }
    
    if ( (registerA.hi & 0xF) < (data & 0x0F) ) {
        raiseFlag(HALF_CARRY_FLAG);
    }
}

short CPU::opcode0xBF() {
    compare8bitRegister(AF, AF.hi);
    return 4;
}// CP A, A

short CPU::opcode0xB8() {
    compare8bitRegister(AF, BC.hi);
    return 4;
}// CP A, B

short CPU::opcode0xB9() {
    compare8bitRegister(AF, BC.low);
    return 4;
}// CP A, C

short CPU::opcode0xBA() {
    compare8bitRegister(AF, DE.hi);
    return 4;
}// CP A, D

short CPU::opcode0xBB() {
    compare8bitRegister(AF, DE.low);
    return 4;
}// CP A, E

short CPU::opcode0xBC() {
    compare8bitRegister(AF, HL.hi);
    return 4;
}// CP A, H

short CPU::opcode0xBD() {
    compare8bitRegister(AF, HL.low);
    return 4;
}// CP A, L

short CPU::opcode0xBE() {
    byte data = memory->read(HL.value);
    compare8bitRegister(AF, data);
    return 8;
}// CP A, (HL) 8 cycles

short CPU::opcode0xFE() {
    byte n = memory->read(PC.value);
    incrementProgramCounter();
    compare8bitRegister(AF, n);
    return 8;
}// CP A, nn 8 cycles

/////////////////////////////////////////////// CPL
short CPU::opcode0x2F() {
    AF.hi ^= 0xFF;
    raiseFlag(HALF_CARRY_FLAG);
    raiseFlag(ADD_SUB_FLAG);
    return 4;
}

/////////////////////////////////////////////// DDA
short CPU::opcode0x27() {
    
    // DAA
    int a = AF.hi;
    
    if ( !checkFlag(ADD_SUB_FLAG) ) {
        
        if ( checkFlag(HALF_CARRY_FLAG) || ((a & 0xF) > 9) ) {
            a += 0x06;
        }
        
        if ( checkFlag(CARRY_FLAG) || (a > 0x9F) ) {
            a += 0x60;
        }
        
    } else {
        
        if ( checkFlag(HALF_CARRY_FLAG)) {
            a = (a - 6) & 0xFF;
        }
        
        if ( checkFlag(CARRY_FLAG) ) {
            a -= 0x60;
        }
    }
    
    clearFlag(ZERO_FLAG);
    clearFlag(HALF_CARRY_FLAG);
    
    if ((a & 0x100) == 0x100)
        raiseFlag(CARRY_FLAG);
    
    a &= 0xFF;
    
    if ( a == 0 ) {
        raiseFlag(ZERO_FLAG);
    } else {
        clearFlag(ZERO_FLAG);
    }
    
    AF.hi = a;
    
    return 4;
}

/////////////////////////////// ADD SP, nn 00hc 16 cycles
short CPU::opcode0xE8() {
    byte data = memory->read(PC.value);
    int result = SP.value + data;
    incrementProgramCounter();
    
    clearFlags();
    
    if ( ((SP.value ^ data ^ (result & 0xFFFF)) & 0x100) == 0x100 ) {
        raiseFlag(CARRY_FLAG);
    }
    
    if ( ((SP.value ^ data ^ (result & 0xFFFF)) & 0x10) == 0x10 ) {
        raiseFlag(HALF_CARRY_FLAG);
    }
    
    SP.value = static_cast<word>(result);
    return 16;
}