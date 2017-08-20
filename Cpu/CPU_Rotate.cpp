#include "CPU.h"

short CPU::opcode0x07() {
	byte bit = AF.hi & 0x80;
	AF.hi <<= 1;
    
	clearFlags();
	if (bit == 0x01) {
		raiseFlag(CARRY_FLAG);;
	}
	if (AF.hi == 0x0) {
		raiseFlag(ZERO_FLAG);
    }

    return 4;
}

short CPU::opcode0x0F() {
	byte bitZero = AF.hi & 0x01;
	AF.hi >>= 1;

	clearFlags();
	if (bitZero == 0x01) {
		raiseFlag(Flag::CARRY_FLAG);
	}
	if (AF.hi == 0x00) {
		raiseFlag(Flag::ZERO_FLAG);
	}

    return 4;
}

short CPU::opcode0x17() {	
	byte lastBit = AF.hi & 0x80;
	AF.hi <<= 1;

	if (checkFlag(Flag::CARRY_FLAG)) {
		setBit(&AF.hi, 0);
	} else {
		clearBit(&AF.hi, 0);
	}

	clearFlags();
	if (lastBit == 0x01) {
		raiseFlag(Flag::CARRY_FLAG);
	}
	if (AF.hi == 0) {
		raiseFlag(ZERO_FLAG);
	}
    return 4;
}// RLA

short CPU::opcode0x1F() {
	byte firstBit = AF.hi & 0x01;
	AF.hi >>= 1;

	if (checkFlag(Flag::CARRY_FLAG)) {
		setBit(&AF.hi, 7);
	} else {
		clearBit(&AF.hi, 7);
	}

	clearFlags();
	if (firstBit == 0x01) {
		raiseFlag(Flag::CARRY_FLAG);
	}
	return 4;
}// RR A

void CPU::rlc8bitRegister(byte * reg) {
	byte bit= *reg & 0x80;
	*reg <<= 1;

	clearFlags();
	if (bit == 0x01) {
		raiseFlag(Flag::CARRY_FLAG);
	}
	if (*reg == 0x00) {
		raiseFlag(ZERO_FLAG);
	}
}

short CPU::extendedOpcode0x00() {
    rlc8bitRegister(&BC.hi);
    return 8;
}// RLC B

short CPU::extendedOpcode0x01() {
    rlc8bitRegister(&BC.low);
    return 8;
}// RLC C

short CPU::extendedOpcode0x02() {
    rlc8bitRegister(&DE.hi);
    return 8;
}// RLC D

short CPU::extendedOpcode0x03() {
    rlc8bitRegister(&DE.low);
    return 8;
}// RLC E

short CPU::extendedOpcode0x04() {
    rlc8bitRegister(&HL.hi);
    return 8;
}// RLC H

short CPU::extendedOpcode0x05() {
    rlc8bitRegister(&HL.low);
    return 8;
}// RLC L

short CPU::extendedOpcode0x06() {
	byte reg = memory->read(HL.value) ;
	rlc8bitRegister(&reg);
	memory->write(HL.value, reg);

	return 16;
}// RLC (HL)

short CPU::extendedOpcode0x07() {
    rlc8bitRegister(&AF.hi);
    return 8;
}// RLC A


void CPU::rrc8bitRegister(byte *reg) {
	byte zeroBit = *reg & 0x01;
	*reg = *reg >> 1;

	clearFlags();
	if (zeroBit == 0x01) {
		raiseFlag(CARRY_FLAG);
	}
	if (*reg == 0) {
		raiseFlag(ZERO_FLAG);
    }
}

short CPU::extendedOpcode0x08() {
    rrc8bitRegister(&BC.hi);
    return 8;
}// RRC B

short CPU::extendedOpcode0x09() {
    rrc8bitRegister(&BC.low);
    return 8;
}// RRC C

short CPU::extendedOpcode0x0A() {
    rrc8bitRegister(&DE.hi);
    return 8;
}// RRC D

short CPU::extendedOpcode0x0B() {
    rrc8bitRegister(&DE.low);
    return 8;
}// RRC E

short CPU::extendedOpcode0x0C() {
    rrc8bitRegister(&HL.hi);
    return 8;
}// RRC H

short CPU::extendedOpcode0x0D() {
    rrc8bitRegister(&HL.low);
    return 8;
}// RRC L

short CPU::extendedOpcode0x0E() {
	byte reg = memory->read(HL.value) ;
	rrc8bitRegister(&reg);
	memory->write(HL.value, reg);
    return 16;
}// RRC (HL)

short CPU::extendedOpcode0x0F() {
	byte bit = AF.hi & 0x01;
	AF.hi >>= 1;

	clearFlags();
	if (bit == 0x01) {
		raiseFlag(Flag::CARRY_FLAG);
	}

	if (AF.hi == 0x00) {
		raiseFlag(Flag::ZERO_FLAG);
	}

    return 8;
}// RRC A

void CPU::rl8bitRegister(byte *reg) {
	byte lastBit = *reg & 0x80;
	*reg <<= 1;

	if (checkFlag(Flag::CARRY_FLAG)) {
		setBit(&AF.hi, 7);
	} else {
		clearBit(&AF.hi, 7);
	}

	clearFlags();
	if (lastBit == 0x01) {
		raiseFlag(Flag::CARRY_FLAG);
	}
	if (AF.hi == 0) {
		raiseFlag(ZERO_FLAG);
	}
}

short CPU::extendedOpcode0x10() {
    rl8bitRegister(&BC.hi);
    return 8;
}// rl B

short CPU::extendedOpcode0x11() {
    rl8bitRegister(&BC.low);
    return 8;
}// rl C

short CPU::extendedOpcode0x12() {
    rl8bitRegister(&DE.hi);
    return 8;
}// rl D

short CPU::extendedOpcode0x13() {
    rl8bitRegister(&DE.low);
    return 8;
}// rl E

short CPU::extendedOpcode0x14() {
    rl8bitRegister(&HL.hi);
    return 8;
}// rl H

short CPU::extendedOpcode0x15() {
    rl8bitRegister(&HL.low);
    return 8;
}// rl L

short CPU::extendedOpcode0x16() {
	byte reg = memory->read(HL.value);
	rl8bitRegister(&reg);
	memory->write(HL.value, reg);
    return 16;
}// rl (HL)

short CPU::extendedOpcode0x17() {

	byte bit = AF.hi & 0x80;

	byte carry = checkFlag(Flag::CARRY_FLAG) ? 0x01 : 0x00;
	AF.hi <<= carry;

	clearFlags();
	if (bit == 0x01) {
		raiseFlag(CARRY_FLAG);;
	}

	if (AF.hi == 0x0) {
		raiseFlag(ZERO_FLAG);
	}

	return 8;
}

void CPU::rr8bitRegister(byte *reg) {
	byte zeroBit = *reg & 0x01;
	*reg = *reg >> 1;

	if (checkFlag(Flag::CARRY_FLAG)) {
		setBit(reg, 7);
	} else {
		clearBit(reg, 7);
	}

	clearFlags();
	if (zeroBit == 0x01) {
		raiseFlag(Flag::CARRY_FLAG);
	}
	if (*reg == 0x00) {
		raiseFlag(Flag::ZERO_FLAG);
	}
}

short CPU::extendedOpcode0x18() {
    rr8bitRegister(&BC.hi);
    return 8;
}// rr B

short CPU::extendedOpcode0x19() {
    rr8bitRegister(&BC.low);
    return 8;
}// rr C

short CPU::extendedOpcode0x1A() {
    rr8bitRegister(&DE.hi);
    return 8;
}// rr D

short CPU::extendedOpcode0x1B() {
    rr8bitRegister(&DE.low);
    return 8;
}// rr E

short CPU::extendedOpcode0x1C() {
    rr8bitRegister(&HL.hi);
    return 8;
}// rr H

short CPU::extendedOpcode0x1D() {
    rr8bitRegister(&HL.low);
    return 8;
}// rr L

short CPU::extendedOpcode0x1E() {
	byte reg = memory->read(HL.value);
	rr8bitRegister(&reg);
    memory->write(HL.value, reg);
    return 16;
}// rr (HL)

short CPU::extendedOpcode0x1F() {
    rr8bitRegister(&AF.hi);
    return 8;
}// rr A

void CPU::sla8bitRegister(byte *reg) {
	byte lastBit = *reg & 0x80;
	*reg <<= 1;

	clearFlags();
	if (lastBit == 0x01) {
		raiseFlag(Flag::CARRY_FLAG);
	}
    if ( *reg == 0 ) {
        raiseFlag(ZERO_FLAG);
    }
}

short CPU::extendedOpcode0x20() {
    sla8bitRegister(&BC.hi);
    return 8;
}// sla B

short CPU::extendedOpcode0x21() {
    sla8bitRegister(&BC.low);
    return 8;
}// sla C

short CPU::extendedOpcode0x22() {
    sla8bitRegister(&DE.hi);
    return 8;
}// sla D

short CPU::extendedOpcode0x23() {
    sla8bitRegister(&DE.low);
    return 8;
}// sla E

short CPU::extendedOpcode0x24() {
    sla8bitRegister(&HL.hi);
    return 8;
}// sla H

short CPU::extendedOpcode0x25() {
    sla8bitRegister(&HL.low);
    return 8;
}// sla L

short CPU::extendedOpcode0x26() {
    byte reg = memory->read(HL.value);
	sla8bitRegister(&reg);
    memory->write(HL.value, reg);
    return 16;
}// sla (HL)

short CPU::extendedOpcode0x27() {
    sla8bitRegister(&AF.hi);
    return 8;
}// sla A


void CPU::sra8bitRegister(byte * reg) {
	byte firstBit = *reg & 0x01;
	byte lastBit = *reg & 0x80;
	
	*reg >>= 1;

	if (lastBit == 0x01) {
		setBit(reg, 7);
	} else {
		clearBit(reg, 7);
	}

	clearFlags();
	if (firstBit == 0x01) {
		raiseFlag(Flag::CARRY_FLAG);
	}
	if (*reg == 0x00) {
		raiseFlag(Flag::ZERO_FLAG);
	}
}

short CPU::extendedOpcode0x28() {
    sra8bitRegister(&BC.hi);
    return 8;
}// sra B

short CPU::extendedOpcode0x29() {
    sra8bitRegister(&BC.low);
    return 8;
}// sra C

short CPU::extendedOpcode0x2A() {
    sra8bitRegister(&DE.hi);
    return 8;
}// sra D

short CPU::extendedOpcode0x2B() {
    sra8bitRegister(&DE.low);
    return 8;
}// sra E

short CPU::extendedOpcode0x2C() {
    sra8bitRegister(&HL.hi);
    return 8;
}// sra H

short CPU::extendedOpcode0x2D(){
    sra8bitRegister(&HL.low);
    return 8;
}// sra L

short CPU::extendedOpcode0x2E() {
    byte data = memory->read(HL.value);
	sra8bitRegister(&data);
	memory->write(HL.value, data);
    return 16;
}// sra (HL)

short CPU::extendedOpcode0x2F() {
    sra8bitRegister(&AF.hi);
    return 8;
}// sra A

void CPU::swap8bitRegister(byte * reg) {
	clearFlags();
    
	*reg = (((*reg & 0xF0) >> 4) | ((*reg & 0x0F) << 4));
    
	if ( *reg == 0 ) {
        raiseFlag(ZERO_FLAG);
    }
}

short CPU::extendedOpcode0x30() {
    swap8bitRegister(&BC.hi);
    return 8;
}// swap B

short CPU::extendedOpcode0x31() {
    swap8bitRegister(&BC.low);
    return 8;
}// swap C

short CPU::extendedOpcode0x32() {
    swap8bitRegister(&DE.hi);
    return 8;
}// swap D

short CPU::extendedOpcode0x33() {
    swap8bitRegister(&DE.low);
    return 8;
}// swap E

short CPU::extendedOpcode0x34() {
    swap8bitRegister(&HL.hi);
    return 8;
}// swap H

short CPU::extendedOpcode0x35() {
    swap8bitRegister(&HL.low);
    return 8;
}// swap L

short CPU::extendedOpcode0x36() {
	clearFlags();
    byte data = memory->read(HL.value);
	data = (((data & 0xF0) >> 4) | ((data & 0x0F) << 4));
    
	if ( data == 0 ) {
        raiseFlag(ZERO_FLAG);
    }
    
    memory->write(HL.value, data);
    return 16;
}// swap (HL)

short CPU::extendedOpcode0x37() {
    swap8bitRegister(&AF.hi);
    return 8;
}// swap A

void CPU::srl8bitRegister(byte *reg) {
	byte firstBit = *reg & 0x01;
	*reg >>= 1;

	clearBit(reg, 7);

	clearFlags();
	if (firstBit == 0x01) {
        raiseFlag(CARRY_FLAG);
    }
	if (*reg == 0) {
        raiseFlag(ZERO_FLAG);
    }
}

short CPU::extendedOpcode0x38() {
    srl8bitRegister(&BC.hi);
    return 8;
}// SRL B

short CPU::extendedOpcode0x39() {
    srl8bitRegister(&BC.low);
    return 8;
}// SRL C

short CPU::extendedOpcode0x3A() {
    srl8bitRegister(&DE.hi);
    return 8;
}// SRL D

short CPU::extendedOpcode0x3B() {
    srl8bitRegister(&DE.low);
    return 8;
}// SRL E

short CPU::extendedOpcode0x3C() {
    srl8bitRegister(&HL.hi);
    return 8;
}// SRL H

short CPU::extendedOpcode0x3D() {
    srl8bitRegister(&HL.low);
    return 8;
}// SRL L

short CPU::extendedOpcode0x3E() {
    byte data = memory->read(HL.value);
	srl8bitRegister(&data);
	memory->write(HL.value, data);
    return 16;
}// SRL (HL)

short CPU::extendedOpcode0x3F() {
    srl8bitRegister(&AF.hi);
    return 8;
}// SRL A
