#include "../CPU.h"

void CPU::increment16BitRegister(Register *reg) {
	reg->value++;
}

short CPU::opcode0x03() {
    increment16BitRegister(&BC);
    return 8;
}

short CPU::opcode0x13() {
    increment16BitRegister(&DE);
    return 8;
}

short CPU::opcode0x23() {
    increment16BitRegister(&HL);
    return 8;
}

short CPU::opcode0x33() {
    increment16BitRegister(&SP);
    return 8;
}

void CPU::increment8BitRegister(byte *reg) {
	*reg = *reg + 1;

	clearFlag(ADD_SUB_FLAG);

	if (*reg == 0x00) {
		raiseFlag(ZERO_FLAG);
	} else {
		clearFlag(ZERO_FLAG);
	}

	if (*reg & 0x0F) {
		clearFlag(HALF_CARRY_FLAG);
	} else {
		raiseFlag(HALF_CARRY_FLAG);
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
    byte result = memory->read(HL.value);
    increment8BitRegister(&result);
    memory->write(HL.value, result);
    return 12;
}// HL increment

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
    decrement16BitRegister(&SP);
    return 8;
} // SP

void CPU::decrement8BitRegister(byte *reg) {
	*reg = *reg - 1;

	raiseFlag(ADD_SUB_FLAG);
	if (*reg == 0x00) {
		raiseFlag(ZERO_FLAG);
	} else {
		clearFlag(ZERO_FLAG);
	}

	if ((*reg & 0x0F) == 0x0F) {
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
    byte data = memory->read(HL.value);
    decrement8BitRegister(&data);
    memory->write(HL.value, data);
    return 12;
}// DEC HL 12 z1h

void CPU::add16BitRegister(Register *original, const Register adding) {

	clearFlag(ADD_SUB_FLAG);

	if (((original->value & 0x0FFF) + (adding.value & 0x0FFF)) > 0x0FFF) {
		raiseFlag(HALF_CARRY_FLAG);
	} else {
		clearFlag(HALF_CARRY_FLAG);
	}

	if ((original->value + adding.value) > 0xFFFF) {
		raiseFlag(CARRY_FLAG);
	} else {
		clearFlag(CARRY_FLAG);
	}

	original->value = original->value + adding.value;
}

short CPU::opcode0x09() {
    add16BitRegister(&HL, BC);
    return 8;
}// HL = HL + BC 8

short CPU::opcode0x19() {
    add16BitRegister(&HL, DE);
    return 8;
}// HL = HL + DE

short CPU::opcode0x29() {
    add16BitRegister(&HL, HL);
    return 8;
}// HL = HL + HL

short CPU::opcode0x39() {
    add16BitRegister(&HL, SP);
    return 8;
}// HL = HL + SP

void CPU::xor8BitRegister(byte *accumulator, const byte data) {
	*accumulator = *accumulator ^ data;

	clearFlags();
	if (*accumulator == 0x00) {
		raiseFlag(ZERO_FLAG);
	}
}

short CPU::opcode0xAF() {
    xor8BitRegister(&AF.hi, AF.hi);
    return 4;
}// XOR A, A

short CPU::opcode0xA8() {
    xor8BitRegister(&AF.hi, BC.hi);
    return 4;
}// XOR A, B

short CPU::opcode0xA9() {
    xor8BitRegister(&AF.hi, BC.low);
    return 4;
}// XOR A, C

short CPU::opcode0xAA() {
    xor8BitRegister(&AF.hi, DE.hi);
    return 4;
}// XOR A, D

short CPU::opcode0xAB() {
    xor8BitRegister(&AF.hi, DE.low);
    return 4;
}// XOR A, E

short CPU::opcode0xAC() {
    xor8BitRegister(&AF.hi, HL.hi);
    return 4;
}// XOR A, H

short CPU::opcode0xAD() {
    xor8BitRegister(&AF.hi, HL.low);
    return 4;
}// XOR A, L

short CPU::opcode0xAE() {
    byte data = memory->read(HL.value);
    xor8BitRegister(&AF.hi, data);
    return 8;
}// XOR A, (HL) 8

short CPU::opcode0xEE() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    xor8BitRegister(&AF.hi, data);
    return 8;
}// XOR A, n 8

void CPU::or8BitRegister(byte *accumulator, byte data) {
	*accumulator = *accumulator | data;

	clearFlags();
	if (*accumulator == 0x00) {
		raiseFlag(ZERO_FLAG);
	}
}

short CPU::opcode0xB7() {
    or8BitRegister(&AF.hi, AF.hi);
    return 4;
}// OR A, A

short CPU::opcode0xB0() {
    or8BitRegister(&AF.hi, BC.hi);
    return 4;
}// OR A, B

short CPU::opcode0xB1() {
    or8BitRegister(&AF.hi, BC.low);
    return 4;
}// OR A, C

short CPU::opcode0xB2() {
    or8BitRegister(&AF.hi, DE.hi);
    return 4;
}// OR A, D

short CPU::opcode0xB3() {
    or8BitRegister(&AF.hi, DE.low);
    return 4;
}// OR A, E

short CPU::opcode0xB4() {
    or8BitRegister(&AF.hi, HL.hi);
    return 4;
}// OR A, H

short CPU::opcode0xB5() {
    or8BitRegister(&AF.hi, HL.low);
    return 4;
}// OR A, L

short CPU::opcode0xB6() {
    byte data = memory->read(HL.value);
    or8BitRegister(&AF.hi, data);
    return 8;
}// OR A, (HL) 8 cycles

short CPU::opcode0xF6() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    or8BitRegister(&AF.hi, data);
    return 8;
}// OR A, n 8 cycles

void CPU::and8BitRegister(byte *accumulator, byte data) {
	*accumulator = *accumulator & data;

	clearFlags();
	raiseFlag(HALF_CARRY_FLAG);
	if (*accumulator == 0x00) {
		raiseFlag(ZERO_FLAG);
	}
}

short CPU::opcode0xA7() {
    and8BitRegister(&AF.hi, AF.hi);
    return 4;
}// AND A, A

short CPU::opcode0xA0() {
    and8BitRegister(&AF.hi, BC.hi);
    return 4;
}// AND A, B

short CPU::opcode0xA1() {
    and8BitRegister(&AF.hi, BC.low);
    return 4;
}// AND A, C

short CPU::opcode0xA2() {
    and8BitRegister(&AF.hi, DE.hi);
    return 4;
}// AND A, D

short CPU::opcode0xA3() {
    and8BitRegister(&AF.hi, DE.low);
    return 4;
}// AND A, E

short CPU::opcode0xA4() {
    and8BitRegister(&AF.hi, HL.hi);
    return 4;
}// AND A, H

short CPU::opcode0xA5() {
    and8BitRegister(&AF.hi, HL.low);
    return 4;
}// AND A, L

short CPU::opcode0xA6() {
    byte data = memory->read(HL.value);
    and8BitRegister(&AF.hi, data);
    return 8;
}// AND A, (HL) 8 cycles

short CPU::opcode0xE6(){
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    and8BitRegister(&AF.hi, data);
    return 8;
}// AND A, n 8 cycles

void CPU::add8BitRegister(byte *accumulator, byte adding) {
	if ((*accumulator + adding) & 0xFF) {
		clearFlag(ZERO_FLAG);
	} else {
		raiseFlag(ZERO_FLAG);
	}

	clearFlag(ADD_SUB_FLAG);

	if (((*accumulator & 0x0F) + (adding & 0x0F)) > 0x0F) {
		raiseFlag(HALF_CARRY_FLAG);
	} else {
		clearFlag(HALF_CARRY_FLAG);
	}

	if ((*accumulator + adding) > 0xFF) {
		raiseFlag(CARRY_FLAG);
	} else {
		clearFlag(CARRY_FLAG);
	}

	*accumulator = *accumulator + adding;
}

short CPU::opcode0x87() {
    add8BitRegister(&AF.hi, AF.hi);
    return 4;
}// ADD A, A

short CPU::opcode0x80() {
    add8BitRegister(&AF.hi, BC.hi);
    return 4;
}// ADD A, B

short CPU::opcode0x81() {
    add8BitRegister(&AF.hi, BC.low);
    return 4;
}// ADD A, C

short CPU::opcode0x82() {
    add8BitRegister(&AF.hi, DE.hi);
    return 4;
}// ADD A, D

short CPU::opcode0x83() {
    add8BitRegister(&AF.hi, DE.low);
    return 4;
}// ADD A, E

short CPU::opcode0x84() {
    add8BitRegister(&AF.hi, HL.hi);
    return 4;
}// ADD A, H

short CPU::opcode0x85() {
    add8BitRegister(&AF.hi, HL.low);
    return 4;
}// ADD A, L

short CPU::opcode0x86() {
    byte data = memory->read(HL.value);
    add8BitRegister(&AF.hi, data);
    return 8;
}// ADD A, (HL) 8

short CPU::opcode0xC6() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    add8BitRegister(&AF.hi, data);
    return 8;
}// ADD A, n 8


void CPU::adc8BitRegister(byte *accumulator, byte adding) {

	byte carryValue = 0x00;
	if (checkFlag(CARRY_FLAG)) {
		carryValue = 0x01;
	}

	word result = *accumulator + carryValue + adding;

	clearFlag(ADD_SUB_FLAG);
	if ((result & 0xFF) == 0x00) {
		raiseFlag(ZERO_FLAG);
	} else {
		clearFlag(ZERO_FLAG);
	}

	if ((carryValue + (adding & 0x0F) + (*accumulator & 0x0F)) > 0x0F) {
		raiseFlag(HALF_CARRY_FLAG);
	} else {
		clearFlag(HALF_CARRY_FLAG);
	}

	if (result > 0xFF) {
		raiseFlag(CARRY_FLAG);
	} else {
		clearFlag(CARRY_FLAG);
	}

	*accumulator = result & 0xFF;
}

short CPU::opcode0x8F() {
    adc8BitRegister(&AF.hi, AF.hi);
    return 4;
}// ADC A, A

short CPU::opcode0x88() {
    adc8BitRegister(&AF.hi, BC.hi);
    return 4;
}// ADC A, B

short CPU::opcode0x89() {
    adc8BitRegister(&AF.hi, BC.low);
    return 4;
}// ADC A, C

short CPU::opcode0x8A() {
    adc8BitRegister(&AF.hi, DE.hi);
    return 4;
}// ADC A, D

short CPU::opcode0x8B() {
    adc8BitRegister(&AF.hi, DE.low);
    return 4;
}// ADC A, E

short CPU::opcode0x8C() {
    adc8BitRegister(&AF.hi, HL.hi);
    return 4;
}// ADC A, H

short CPU::opcode0x8D() {
    adc8BitRegister(&AF.hi, HL.low);
    return 4;
}// ADC A, L

short CPU::opcode0x8E() {
    byte data = memory->read(HL.value);
    adc8BitRegister(&AF.hi, data);
    return 8;
}// ADC A, (HL) 8

short CPU::opcode0xCE() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    adc8BitRegister(&AF.hi, data);
    return 8;
}// ADC A, nn 8

void CPU::sub8BitRegister(byte *accumulator, byte substract) {

	raiseFlag(ADD_SUB_FLAG);
	if ((*accumulator - substract) == 0x00) {
		raiseFlag(ZERO_FLAG);
	} else {
		clearFlag(ZERO_FLAG);
	}

	if ((*accumulator & 0x0F) < (substract & 0x0F)) {
		raiseFlag(HALF_CARRY_FLAG);
	} else {
		clearFlag(HALF_CARRY_FLAG);
	}

	if (*accumulator < substract) {
		raiseFlag(CARRY_FLAG);
	} else {
		clearFlag(CARRY_FLAG);
	}

	*accumulator = *accumulator - substract;
}
short CPU::opcode0x97() {
    sub8BitRegister(&AF.hi, AF.hi);
    return 4;
}// SUB A, A

short CPU::opcode0x90() {
    sub8BitRegister(&AF.hi, BC.hi);
    return 4;
}// SUB A, B

short CPU::opcode0x91() {
    sub8BitRegister(&AF.hi, BC.low);
    return 4;
}// SUB A, C

short CPU::opcode0x92() {
    sub8BitRegister(&AF.hi, DE.hi);
    return 4;
}// SUB A, D

short CPU::opcode0x93() {
    sub8BitRegister(&AF.hi, DE.low);
    return 4;
}// SUB A, E

short CPU::opcode0x94() {
    sub8BitRegister(&AF.hi, HL.hi);
    return 4;
}// SUB A, H

short CPU::opcode0x95() {
    sub8BitRegister(&AF.hi, HL.low);
    return 4;
}// SUB A, L

short CPU::opcode0x96() {
    byte data = memory->read(HL.value);
    sub8BitRegister(&AF.hi, data);
    return 8;
}// SUB A, (HL) 8

short CPU::opcode0xD6() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    sub8BitRegister(&AF.hi, data);
    return 8;
}// SUB A, n 8

void CPU::sbc8BitRegister(byte *accumulator, byte substract) {
	int carry = checkFlag(Flag::CARRY_FLAG) ? 1 : 0;
	int result = *accumulator - substract - carry;

	clearFlags();
	raiseFlag(Flag::ADD_SUB_FLAG);
	if (static_cast<byte>(result) == 0x00) {
		raiseFlag(Flag::ZERO_FLAG);
	}

	if (result < 0) {
		raiseFlag(Flag::CARRY_FLAG);
	}

	if (((*accumulator & 0x0F) - (substract & 0x0F) - carry) < 0) {
		raiseFlag(Flag::HALF_CARRY_FLAG);
	}

	*accumulator = static_cast<byte>(result);
}

short CPU::opcode0x9F() {
    sbc8BitRegister(&AF.hi, AF.hi);
    return 4;
}// SBC A, A

short CPU::opcode0x98() {
    sbc8BitRegister(&AF.hi, BC.hi);
    return 4;
}// SBC A, B

short CPU::opcode0x99() {
    sbc8BitRegister(&AF.hi, BC.low);
    return 4;
}// SBC A, C

short CPU::opcode0x9A() {
    sbc8BitRegister(&AF.hi, DE.hi);
    return 4;
}// SBC A, D

short CPU::opcode0x9B() {
    sbc8BitRegister(&AF.hi, DE.low);
    return 4;
}// SBC A, E

short CPU::opcode0x9C() {
    sbc8BitRegister(&AF.hi, HL.hi);
    return 4;
}// SBC A, H

short CPU::opcode0x9D() {
    sbc8BitRegister(&AF.hi, HL.low);
    return 4;
}// SBC A, L

short CPU::opcode0x9E() {
    byte data = memory->read(HL.value);
    sbc8BitRegister(&AF.hi, data);
    return 8;
}// SBC A, (HL) 8

short CPU::opcode0xDE() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    sbc8BitRegister(&AF.hi, data);
    return 8;
}// SBC A, n 8

void CPU::compare8BitRegister(byte accumulator, byte data) {
	clearFlags();

	raiseFlag(ADD_SUB_FLAG);
	if (accumulator == data) {
		raiseFlag(ZERO_FLAG);
	}

	if ((accumulator & 0x0F) < (data & 0x0F)) {
		raiseFlag(HALF_CARRY_FLAG);
	}

	if (accumulator < data) {
		raiseFlag(CARRY_FLAG);
	}
}

short CPU::opcode0xBF() {
    compare8BitRegister(AF.hi, AF.hi);
    return 4;
}// CP A, A

short CPU::opcode0xB8() {
    compare8BitRegister(AF.hi, BC.hi);
    return 4;
}// CP A, B

short CPU::opcode0xB9() {
    compare8BitRegister(AF.hi, BC.low);
    return 4;
}// CP A, C

short CPU::opcode0xBA() {
    compare8BitRegister(AF.hi, DE.hi);
    return 4;
}// CP A, D

short CPU::opcode0xBB() {
    compare8BitRegister(AF.hi, DE.low);
    return 4;
}// CP A, E

short CPU::opcode0xBC() {
    compare8BitRegister(AF.hi, HL.hi);
    return 4;
}// CP A, H

short CPU::opcode0xBD() {
    compare8BitRegister(AF.hi, HL.low);
    return 4;
}// CP A, L

short CPU::opcode0xBE() {
    byte data = memory->read(HL.value);
    compare8BitRegister(AF.hi, data);
    return 8;
}// CP A, (HL) 8 cycles

short CPU::opcode0xFE() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    compare8BitRegister(AF.hi, data);
    return 8;
}// CP A, nn 8 cycles

short CPU::opcode0x2F() {
	AF.hi = ~AF.hi;
	raiseFlag(ADD_SUB_FLAG);
	raiseFlag(HALF_CARRY_FLAG);
    return 4;
}

short CPU::opcode0x27() {
    int a = AF.hi;

	if (checkFlag(ADD_SUB_FLAG)) {
		if (checkFlag(HALF_CARRY_FLAG)) {
			a = (a - 6) & 0xFF;
		}
		if (checkFlag(CARRY_FLAG)) {
			a -= 0x60;
		}
	} else {

		if (checkFlag(HALF_CARRY_FLAG) || ((a & 0xF) > 9)) {
			a += 0x06;
		}
		if ((checkFlag(CARRY_FLAG)) || (a > 0x9F)) {
			a += 0x60;
		}
	}

	clearFlag(HALF_CARRY_FLAG);
	clearFlag(ZERO_FLAG);

	if ((a & 0x100) == 0x100) {
		raiseFlag(CARRY_FLAG);
	}

	a &= 0xFF;

	if (a == 0) {
		raiseFlag(ZERO_FLAG);
	}

	AF.hi = a;
    return 4;
}

short CPU::opcode0xE8() {
    char data = memory->read(PC.value);
    incrementProgramCounter();

	clearFlags();

	if (((SP.value & 0x0F) + (data & 0x0F)) > 0x0F) {
		raiseFlag(HALF_CARRY_FLAG);
	}

	if (((SP.value & 0xFF) + (data & 0xFF)) > 0xFF) {
		raiseFlag(CARRY_FLAG);
	}

	SP.value += static_cast<word>(data);

	return 16;
}