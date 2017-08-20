#include "CPU.h"

///////////////////////////////////16 bit load commands
void CPU::load16BitRegister(Register *reg) {
    reg->low = memory->read(PC.value);
    incrementProgramCounter();
    reg->hi = memory->read(PC.value);
    incrementProgramCounter();
}

short CPU::opcode0x01() {
    load16BitRegister(&BC);
    return 12;
}//LD BC, nn

short CPU::opcode0x11() {
    load16BitRegister(&DE);
    return 12;
}// Load DE, nn

short CPU::opcode0x21() {
    load16BitRegister(&HL);
    return 12;
}// Load HL, nn

short CPU::opcode0x31() {
    load16BitRegister(&SP);
    return 12;
}// Load SP, nn

short CPU::opcode0xF9() {
    SP.value = HL.value;
    return 8;
} // Load SP, HL


short CPU::opcode0x08() {
    // LD (nn),SP
    Register aux;
    aux.low = memory->read(PC.value);
    incrementProgramCounter();
    aux.hi = memory->read(PC.value);
    incrementProgramCounter();
    
    memory->write(aux.value, SP.low);
    memory->write(aux.value+1, SP.hi);
    
    return 20;
}

//////////////////////////////////////////// 8 bits load
void CPU::loadRegister(byte *reg) {
    *reg = memory->read(PC.value);
    incrementProgramCounter();
}

short CPU::opcode0x06() {
    loadRegister(&BC.hi);
    return 8;
}// B load

short CPU::opcode0x0E() {
    loadRegister(&BC.low);
    return 8;
}// C load

short CPU::opcode0x16() {
    loadRegister(&DE.hi);
    return 8;
}// D load

short CPU::opcode0x1E() {
    loadRegister(&DE.low);
    return 8;
}// E load

short CPU::opcode0x26() {
    loadRegister(&HL.hi);
    return 8;
}// H load

short CPU::opcode0x2E() {
    loadRegister(&HL.low);
    return 8;
}// L load

short CPU::opcode0x3E() {
    loadRegister(&AF.hi);
    return 8;
}

/////////////////////////////////////////////// Loads to accumulator
short CPU::opcode0x02(){
    memory->write(BC.value, AF.hi);
    return 8;
}// LD (BC), A 8

short CPU::opcode0x12(){
    memory->write(DE.value, AF.hi);
    return 8;
}// LD (DE), A

short CPU::opcode0x77() {
    memory->write(HL.value, AF.hi);
    return 8;
}// LD (HL),A

short CPU::opcode0xEA() {
    Register address;
    address.low = memory->read(PC.value);
    incrementProgramCounter();
    address.hi = memory->read(PC.value);
    incrementProgramCounter();
    memory->write(address.value, AF.hi);
    return 16;
} // LD (nn), A

short CPU::opcode0xE2() {
    memory->write(BC.low+0xFF00, AF.hi);
    return 8;
}// LD (0xFF00+C),A

/////////////////////////////////////////////// Put A in memory and DEC or INC

short CPU::opcode0x22() {
    memory->write(HL.value, AF.hi);
    instructionSet->increment16BitRegister(&HL);
    return 8;
} // LD (HLI),A (22) INC

short CPU::opcode0x2A() {
    AF.hi = memory->read(HL.value);
    instructionSet->increment16BitRegister(&HL);
    return 8;
} // LDi A, (HL)

short CPU::opcode0x32() {
    memory->write(HL.value, AF.hi);
    instructionSet->decrement16BitRegister(&HL);
    return 8;
}

short CPU::opcode0x3A() {
    AF.hi = memory->read(HL.value);
    instructionSet->decrement16BitRegister(&HL);
    return 8;
} // LDi A, (HL)

//////////////////////////////////////////////////// LD r,r 4
void CPU::load8BitRegister(byte *reg, const byte data) {
    *reg = data;
}

// LD A,r -------------
short CPU::opcode0x7F() {
    load8BitRegister(&AF.hi, AF.hi);
    return 4;
}// LD A, A

short CPU::opcode0x78() {
    load8BitRegister(&AF.hi, BC.hi);
    return 4;
}// LD A, B

short CPU::opcode0x79() {
    load8BitRegister(&AF.hi, BC.low);
    return 4;
}// LD A, C

short CPU::opcode0x7A() {
    load8BitRegister(&AF.hi, DE.hi);
    return 4;
}// LD A, D

short CPU::opcode0x7B() {
    load8BitRegister(&AF.hi, DE.low);
    return 4;
}// LD A, E

short CPU::opcode0x7C() {
    load8BitRegister(&AF.hi, HL.hi);
    return 4;
}// LD A, H

short CPU::opcode0x7D() {
    load8BitRegister(&AF.hi, HL.low);
    return 4;
}// LD A, L

// LD B,r -------------

short CPU::opcode0x40() {
    load8BitRegister(&BC.hi, BC.hi);
    return 4;
}// LD B, B

short CPU::opcode0x41(){
    load8BitRegister(&BC.hi, BC.low);
    return 4;
}// LD B, C

short CPU::opcode0x42(){
    load8BitRegister(&BC.hi, DE.hi);
    return 4;
}// LD B, D

short CPU::opcode0x43(){
    load8BitRegister(&BC.hi, DE.low);
    return 4;
}// LD B, E

short CPU::opcode0x44(){
    load8BitRegister(&BC.hi, HL.hi);
    return 4;
}// LD B, H

short CPU::opcode0x45(){
    load8BitRegister(&BC.hi, HL.low);
    return 4;
}// LD B, L

// LD C,r -------------
short CPU::opcode0x48() {
    load8BitRegister(&BC.low, BC.hi);
    return 4;
}// LD C, B

short CPU::opcode0x49() {
    load8BitRegister(&BC.low, BC.low);
    return 4;
}// LD C, C

short CPU::opcode0x4A() {
    load8BitRegister(&BC.low, DE.hi);
    return 4;
}// LD C, D

short CPU::opcode0x4B() {
    load8BitRegister(&BC.low, DE.low);
    return 4;
}// LD C, E

short CPU::opcode0x4C() {
    load8BitRegister(&BC.low, HL.hi);
    return 4;
}// LD C, H

short CPU::opcode0x4D() {
    load8BitRegister(&BC.low, HL.low);
    return 4;
}// LD C, L

// LD D,r -------------
short CPU::opcode0x50() {
    load8BitRegister(&DE.hi, BC.hi);
    return 4;
}// LD D, B

short CPU::opcode0x51() {
    load8BitRegister(&DE.hi, BC.low);
    return 4;
}// LD D, C

short CPU::opcode0x52() {
    load8BitRegister(&DE.hi, DE.hi);
    return 4;
}// LD D, D

short CPU::opcode0x53() {
    load8BitRegister(&DE.hi, DE.low);
    return 4;
}// LD D, E

short CPU::opcode0x54() {
    load8BitRegister(&DE.hi, HL.hi);
    return 4;
}// LD D, H

short CPU::opcode0x55() {
    load8BitRegister(&DE.hi, HL.low);
    return 4;
}// LD D, L

// LD E,r -------------
short CPU::opcode0x58() {
    load8BitRegister(&DE.low, BC.hi);
    return 4;
}// LD E, B

short CPU::opcode0x59() {
    load8BitRegister(&DE.low, BC.low);
    return 4;
}// LD E, C

short CPU::opcode0x5A() {
    load8BitRegister(&DE.low, DE.hi);
    return 4;
}// LD E, D

short CPU::opcode0x5B() {
    load8BitRegister(&DE.low, DE.low);
    return 4;
}// LD E, E

short CPU::opcode0x5C() {
    load8BitRegister(&DE.low, HL.hi);
    return 4;
}// LD E, H

short CPU::opcode0x5D() {
    load8BitRegister(&DE.low, HL.low);
    return 4;
}// LD E, L

// LD H, r -------------
short CPU::opcode0x60() {
    load8BitRegister(&HL.hi, BC.hi);
    return 4;
}// LD H, B

short CPU::opcode0x61() {
    load8BitRegister(&HL.hi, BC.low);
    return 4;
}// LD H, C

short CPU::opcode0x62() {
    load8BitRegister(&HL.hi, DE.hi);
    return 4;
}// LD H, D

short CPU::opcode0x63() {
    load8BitRegister(&HL.hi, DE.low);
    return 4;
}// LD H, E

short CPU::opcode0x64() {
    load8BitRegister(&HL.hi, HL.hi);
    return 4;
}// LD H, H

short CPU::opcode0x65() {
    load8BitRegister(&HL.hi, HL.low);
    return 4;
}// LD H, L

// LD L, r -------------
short CPU::opcode0x68() {
    load8BitRegister(&HL.low, BC.hi);
    return 4;
}// LD L, B

short CPU::opcode0x69() {
    load8BitRegister(&HL.low, BC.low);
    return 4;
}// LD L, C

short CPU::opcode0x6A() {
    load8BitRegister(&HL.low, DE.hi);
    return 4;
}// LD L, D

short CPU::opcode0x6B() {
    load8BitRegister(&HL.low, DE.low);
    return 4;
}// LD L, E

short CPU::opcode0x6C() {
    load8BitRegister(&HL.low, HL.hi);
    return 4;
}// LD L, H

short CPU::opcode0x6D() {
    load8BitRegister(&HL.low, HL.low);
    return 4;
}// LD L, L

///////////////////////////////////////////// LD (HL), r
short CPU::opcode0x70() {
    memory->write(HL.value, BC.hi);
    return 8;
}// LD (HL), B

short CPU::opcode0x71() {
    memory->write(HL.value, BC.low);
    return 8;
}// LD (HL), C

short CPU::opcode0x72() {
    memory->write(HL.value, DE.hi);
    return 8;
}// LD (HL), D

short CPU::opcode0x73() {
    memory->write(HL.value, DE.low);
    return 8;
}// LD (HL), E

short CPU::opcode0x74() {
    memory->write(HL.value, HL.hi);
    return 8;
}; // LD (HL), H

short CPU::opcode0x75() {
    memory->write(HL.value, HL.low);
    return 8;
}// LD (HL), L

short CPU::opcode0x36() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    memory->write(HL.value, data);
    return 12;
}// LD (HL), n 12

/////////////////////////////////////// LD r, (HL) 8
short CPU::opcode0x7E() {
    load8BitRegister(&AF.hi, memory->read(HL.value));
    return 8;
}// LD A, (HL)

short CPU::opcode0x46() {
    load8BitRegister(&BC.hi, memory->read(HL.value));
    return 8;
}// LD B, (HL)

short CPU::opcode0x4E() {
    load8BitRegister(&BC.low, memory->read(HL.value));
    return 8;
}// LD C, (HL)

short CPU::opcode0x56() {
    load8BitRegister(&DE.hi, memory->read(HL.value));
    return 8;
}// LD D, (HL)

short CPU::opcode0x5E() {
    load8BitRegister(&DE.low, memory->read(HL.value));
    return 8;
}// LD E, (HL)

short CPU::opcode0x66() {
    load8BitRegister(&HL.hi, memory->read(HL.value));
    return 8;
}// LD H, (HL)

short CPU::opcode0x6E() {
    load8BitRegister(&HL.low, memory->read(HL.value));
    return 8;
}// LD L, (HL)

short CPU::opcode0x0A() {
    AF.hi = memory->read(BC.value);
    return 8;
}// LD A, (BC) 8

short CPU::opcode0x1A() {
    AF.hi = memory->read(DE.value);
    return 8;
}// LD A, (DE) 8

short CPU::opcode0xFA() {
    Register aux;
    aux.low = memory->read(PC.value);
    incrementProgramCounter();
    aux.hi = memory->read(PC.value);
    incrementProgramCounter();
    
    AF.hi = memory->read(aux.value);
    return 16;
}// LD A, (nn) 16

short CPU::opcode0xF0() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    AF.hi = memory->read(0xFF00 + data);
    return 12;
}// LD A, (FF00+n) 12

short CPU::opcode0xE0() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    memory->write(0xFF00 + data, AF.hi);
    return 12;
} // LD (FF00+n), A 12

short CPU::opcode0xF2() {
    AF.hi = memory->read(0xFF00 + BC.low);
    return 8;
}// LD A, (FF00+C), 8

////////////////////////////////////// STACK OPERATIONS

void CPU::incrementStackPointer() {
    SP.value++;
}

void CPU::decrementStackPointer() {
    SP.value--;
}

void CPU::stackPush(const Register reg) {
    decrementStackPointer();
    memory->writeDirectly(SP.value, reg.hi);
    decrementStackPointer();
    memory->writeDirectly(SP.value, reg.low);
}

void CPU::stackPop(Register *reg) {
    reg->low = memory->readDirectly(SP.value);
    incrementStackPointer();
    reg->hi = memory->readDirectly(SP.value);
    incrementStackPointer();
}

short CPU::opcode0xC5() {
    stackPush(BC);
    return 16;
}// PUSH BC

short CPU::opcode0xD5() {
    stackPush(DE);
    return 16;
}// PUSH DE

short CPU::opcode0xE5() {
    stackPush(HL);
    return 16;
}// PUSH HL

short CPU::opcode0xF5() {
    stackPush(AF);
    return 16;
}// PUSH AF

short CPU::opcode0xC1() {
    stackPop(&BC);
    return 12;
} // POP BC

short CPU::opcode0xD1() {
    stackPop(&DE);
    return 12;
}// POP DE

short CPU::opcode0xE1() {
    stackPop(&HL);
    return 12;
}// POP HL

short CPU::opcode0xF1() {
	stackPop(&AF);
	// We must mask out the bits (0-3) of F in order to ignore their value
	AF.low = AF.low & 0xF0;
    return 12;
}// POP AF

void  CPU::loadIntoRegister(byte *reg, const byte registerA) {
    *reg = registerA;
}

short CPU::opcode0x47() {
    loadIntoRegister(&BC.hi, AF.hi);
    return 4;
}// LD B,A

short CPU::opcode0x4F() {
    loadIntoRegister(&BC.low, AF.hi);
    return 4;
}// LD C,A

short CPU::opcode0x57() {
    loadIntoRegister(&DE.hi, AF.hi);
    return 4;
}// LD D,A

short CPU::opcode0x5F() {
    loadIntoRegister(&DE.low, AF.hi);
    return 4;
}// LD E,A

short CPU::opcode0x67() {
    loadIntoRegister(&HL.hi, AF.hi);
    return 4;
}// LD H,A

short CPU::opcode0x6F() {
    loadIntoRegister(&HL.low, AF.hi);
    return 4;
}// LD L,A

////////////////////////////////// ld   HL,SP+dd
short CPU::opcode0xF8() {
    char n = memory->read(PC.value) ;
    word value = (SP.value + n);
	
	clearFlags();
	if (((SP.value ^ n ^ value) & 0x100) == 0x100) {
		raiseFlag(CARRY_FLAG);
	}
	if (((SP.value ^ n ^ value) & 0x10) == 0x10) {
		raiseFlag(HALF_CARRY_FLAG);
	}

	HL.value = value;
	incrementProgramCounter();
	return 12;
}