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

short CPU::opcode0x3C() {
    instructionSet->increment8BitRegister(&AF.hi, &AF.low);
    return 4;
}// A increment 4

short CPU::opcode0x04() {
    instructionSet->increment8BitRegister(&BC.hi, &AF.low);
    return 4;
}// B increment

short CPU::opcode0x0C() {
    instructionSet->increment8BitRegister(&BC.low, &AF.low);
    return 4;
}// C increment

short CPU::opcode0x14() {
    instructionSet->increment8BitRegister(&DE.hi, &AF.low);
    return 4;
}// D increment

short CPU::opcode0x1C() {
    instructionSet->increment8BitRegister(&DE.low, &AF.low);
    return 4;
}// E increment

short CPU::opcode0x24() {
    instructionSet->increment8BitRegister(&HL.hi, &AF.low);
    return 4;
}// H increment

short CPU::opcode0x2C() {
    instructionSet->increment8BitRegister(&HL.low, &AF.low);
    return 4;
}// L increment

short CPU::opcode0x34() {
    byte result = memory->read(HL.value);
    instructionSet->increment8BitRegister(&result, &AF.low);
    memory->write(HL.value, result);
    return 12;
}// HL increment

short CPU::opcode0x0B() {
    instructionSet->decrement16BitRegister(&BC);
    return 8;
} // BC

short CPU::opcode0x1B() {
    instructionSet->decrement16BitRegister(&DE);
    return 8;
} // DE

short CPU::opcode0x2B() {
    instructionSet->decrement16BitRegister(&HL);
    return 8;
} // HL

short CPU::opcode0x3B() {
    instructionSet->decrement16BitRegister(&SP);
    return 8;
} // SP

short CPU::opcode0x05() {
    instructionSet->decrement8BitRegister(&BC.hi, &AF.low);
    return 4;
}// DEC B 4

short CPU::opcode0x0D() {
    instructionSet->decrement8BitRegister(&BC.low, &AF.low);
    return 4;
}// DEC C

short CPU::opcode0x15() {
    instructionSet->decrement8BitRegister(&DE.hi, &AF.low);
    return 4;
}// DEC D

short CPU::opcode0x1D() {
    instructionSet->decrement8BitRegister(&DE.low, &AF.low);
    return 4;
}// DEC E

short CPU::opcode0x25() {
    instructionSet->decrement8BitRegister(&HL.hi, &AF.low);
    return 4;
}// DEC H

short CPU::opcode0x2D() {
    instructionSet->decrement8BitRegister(&HL.low, &AF.low);
    return 4;
}// DEC L

short CPU::opcode0x3D() {
    instructionSet->decrement8BitRegister(&AF.hi, &AF.low);
    return 4;
}// DEC A

short CPU::opcode0x35() {
    byte data = memory->read(HL.value);
    instructionSet->decrement8BitRegister(&data, &AF.low);
    memory->write(HL.value, data);
    return 12;
}// DEC HL 12 z1h

short CPU::opcode0x09() {
    instructionSet->add16BitRegister(&HL, BC, &AF.low);
    return 8;
}// HL = HL + BC 8

short CPU::opcode0x19() {
    instructionSet->add16BitRegister(&HL, DE, &AF.low);
    return 8;
}// HL = HL + DE

short CPU::opcode0x29() {
    instructionSet->add16BitRegister(&HL, HL, &AF.low);
    return 8;
}// HL = HL + HL

short CPU::opcode0x39() {
    instructionSet->add16BitRegister(&HL, SP, &AF.low);
    return 8;
}// HL = HL + SP

short CPU::opcode0xAF() {
    instructionSet->xor8BitRegister(&AF.hi, AF.hi, &AF.low);
    return 4;
}// XOR A, A

short CPU::opcode0xA8() {
    instructionSet->xor8BitRegister(&AF.hi, BC.hi, &AF.low);
    return 4;
}// XOR A, B

short CPU::opcode0xA9() {
    instructionSet->xor8BitRegister(&AF.hi, BC.low, &AF.low);
    return 4;
}// XOR A, C

short CPU::opcode0xAA() {
    instructionSet->xor8BitRegister(&AF.hi, DE.hi, &AF.low);
    return 4;
}// XOR A, D

short CPU::opcode0xAB() {
    instructionSet->xor8BitRegister(&AF.hi, DE.low, &AF.low);
    return 4;
}// XOR A, E

short CPU::opcode0xAC() {
    instructionSet->xor8BitRegister(&AF.hi, HL.hi, &AF.low);
    return 4;
}// XOR A, H

short CPU::opcode0xAD() {
    instructionSet->xor8BitRegister(&AF.hi, HL.low, &AF.low);
    return 4;
}// XOR A, L

short CPU::opcode0xAE() {
    byte data = memory->read(HL.value);
    instructionSet->xor8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// XOR A, (HL) 8

short CPU::opcode0xEE() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    instructionSet->xor8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// XOR A, n 8

short CPU::opcode0xB7() {
    instructionSet->or8BitRegister(&AF.hi, AF.hi, &AF.low);
    return 4;
}// OR A, A

short CPU::opcode0xB0() {
    instructionSet->or8BitRegister(&AF.hi, BC.hi, &AF.low);
    return 4;
}// OR A, B

short CPU::opcode0xB1() {
    instructionSet->or8BitRegister(&AF.hi, BC.low, &AF.low);
    return 4;
}// OR A, C

short CPU::opcode0xB2() {
    instructionSet->or8BitRegister(&AF.hi, DE.hi, &AF.low);
    return 4;
}// OR A, D

short CPU::opcode0xB3() {
    instructionSet->or8BitRegister(&AF.hi, DE.low, &AF.low);
    return 4;
}// OR A, E

short CPU::opcode0xB4() {
    instructionSet->or8BitRegister(&AF.hi, HL.hi, &AF.low);
    return 4;
}// OR A, H

short CPU::opcode0xB5() {
    instructionSet->or8BitRegister(&AF.hi, HL.low, &AF.low);
    return 4;
}// OR A, L

short CPU::opcode0xB6() {
    byte data = memory->read(HL.value);
    instructionSet->or8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// OR A, (HL) 8 cycles

short CPU::opcode0xF6() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    instructionSet->or8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// OR A, n 8 cycles

short CPU::opcode0xA7() {
    instructionSet->and8BitRegister(&AF.hi, AF.hi, &AF.low);
    return 4;
}// AND A, A

short CPU::opcode0xA0() {
    instructionSet->and8BitRegister(&AF.hi, BC.hi, &AF.low);
    return 4;
}// AND A, B

short CPU::opcode0xA1() {
    instructionSet->and8BitRegister(&AF.hi, BC.low, &AF.low);
    return 4;
}// AND A, C

short CPU::opcode0xA2() {
    instructionSet->and8BitRegister(&AF.hi, DE.hi, &AF.low);
    return 4;
}// AND A, D

short CPU::opcode0xA3() {
    instructionSet->and8BitRegister(&AF.hi, DE.low, &AF.low);
    return 4;
}// AND A, E

short CPU::opcode0xA4() {
    instructionSet->and8BitRegister(&AF.hi, HL.hi, &AF.low);
    return 4;
}// AND A, H

short CPU::opcode0xA5() {
    instructionSet->and8BitRegister(&AF.hi, HL.low, &AF.low);
    return 4;
}// AND A, L

short CPU::opcode0xA6() {
    byte data = memory->read(HL.value);
    instructionSet->and8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// AND A, (HL) 8 cycles

short CPU::opcode0xE6(){
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    instructionSet->and8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// AND A, n 8 cycles

short CPU::opcode0x87() {
    instructionSet->add8BitRegister(&AF.hi, AF.hi, &AF.low);
    return 4;
}// ADD A, A

short CPU::opcode0x80() {
    instructionSet->add8BitRegister(&AF.hi, BC.hi, &AF.low);
    return 4;
}// ADD A, B

short CPU::opcode0x81() {
    instructionSet->add8BitRegister(&AF.hi, BC.low, &AF.low);
    return 4;
}// ADD A, C

short CPU::opcode0x82() {
    instructionSet->add8BitRegister(&AF.hi, DE.hi, &AF.low);
    return 4;
}// ADD A, D

short CPU::opcode0x83() {
    instructionSet->add8BitRegister(&AF.hi, DE.low, &AF.low);
    return 4;
}// ADD A, E

short CPU::opcode0x84() {
    instructionSet->add8BitRegister(&AF.hi, HL.hi, &AF.low);
    return 4;
}// ADD A, H

short CPU::opcode0x85() {
    instructionSet->add8BitRegister(&AF.hi, HL.low, &AF.low);
    return 4;
}// ADD A, L

short CPU::opcode0x86() {
    byte data = memory->read(HL.value);
    instructionSet->add8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// ADD A, (HL) 8

short CPU::opcode0xC6() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    instructionSet->add8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// ADD A, n 8

short CPU::opcode0x8F() {
    instructionSet->adc8BitRegister(&AF.hi, AF.hi, &AF.low);
    return 4;
}// ADC A, A

short CPU::opcode0x88() {
    instructionSet->adc8BitRegister(&AF.hi, BC.hi, &AF.low);
    return 4;
}// ADC A, B

short CPU::opcode0x89() {
    instructionSet->adc8BitRegister(&AF.hi, BC.low, &AF.low);
    return 4;
}// ADC A, C

short CPU::opcode0x8A() {
    instructionSet->adc8BitRegister(&AF.hi, DE.hi, &AF.low);
    return 4;
}// ADC A, D

short CPU::opcode0x8B() {
    instructionSet->adc8BitRegister(&AF.hi, DE.low, &AF.low);
    return 4;
}// ADC A, E

short CPU::opcode0x8C() {
    instructionSet->adc8BitRegister(&AF.hi, HL.hi, &AF.low);
    return 4;
}// ADC A, H

short CPU::opcode0x8D() {
    instructionSet->adc8BitRegister(&AF.hi, HL.low, &AF.low);
    return 4;
}// ADC A, L

short CPU::opcode0x8E() {
    byte data = memory->read(HL.value);
    instructionSet->adc8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// ADC A, (HL) 8

short CPU::opcode0xCE() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    instructionSet->adc8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// ADC A, nn 8

short CPU::opcode0x97() {
    instructionSet->sub8BitRegister(&AF.hi, AF.hi, &AF.low);
    return 4;
}// SUB A, A

short CPU::opcode0x90() {
    instructionSet->sub8BitRegister(&AF.hi, BC.hi, &AF.low);
    return 4;
}// SUB A, B

short CPU::opcode0x91() {
    instructionSet->sub8BitRegister(&AF.hi, BC.low, &AF.low);
    return 4;
}// SUB A, C

short CPU::opcode0x92() {
    instructionSet->sub8BitRegister(&AF.hi, DE.hi, &AF.low);
    return 4;
}// SUB A, D

short CPU::opcode0x93() {
    instructionSet->sub8BitRegister(&AF.hi, DE.low, &AF.low);
    return 4;
}// SUB A, E

short CPU::opcode0x94() {
    instructionSet->sub8BitRegister(&AF.hi, HL.hi, &AF.low);
    return 4;
}// SUB A, H

short CPU::opcode0x95() {
    instructionSet->sub8BitRegister(&AF.hi, HL.low, &AF.low);
    return 4;
}// SUB A, L

short CPU::opcode0x96() {
    byte data = memory->read(HL.value);
    instructionSet->sub8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// SUB A, (HL) 8

short CPU::opcode0xD6() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    instructionSet->sub8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// SUB A, n 8

short CPU::opcode0x9F() {
    instructionSet->sbc8BitRegister(&AF.hi, AF.hi, &AF.low);
    return 4;
}// SBC A, A

short CPU::opcode0x98() {
    instructionSet->sbc8BitRegister(&AF.hi, BC.hi, &AF.low);
    return 4;
}// SBC A, B

short CPU::opcode0x99() {
    instructionSet->sbc8BitRegister(&AF.hi, BC.low, &AF.low);
    return 4;
}// SBC A, C

short CPU::opcode0x9A() {
    instructionSet->sbc8BitRegister(&AF.hi, DE.hi, &AF.low);
    return 4;
}// SBC A, D

short CPU::opcode0x9B() {
    instructionSet->sbc8BitRegister(&AF.hi, DE.low, &AF.low);
    return 4;
}// SBC A, E

short CPU::opcode0x9C() {
    instructionSet->sbc8BitRegister(&AF.hi, HL.hi, &AF.low);
    return 4;
}// SBC A, H

short CPU::opcode0x9D() {
    instructionSet->sbc8BitRegister(&AF.hi, HL.low, &AF.low);
    return 4;
}// SBC A, L

short CPU::opcode0x9E() {
    byte data = memory->read(HL.value);
    instructionSet->sbc8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// SBC A, (HL) 8

short CPU::opcode0xDE() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    instructionSet->sbc8BitRegister(&AF.hi, data, &AF.low);
    return 8;
}// SBC A, n 8

short CPU::opcode0xBF() {
    instructionSet->compare8BitRegister(AF.hi, AF.hi, &AF.low);
    return 4;
}// CP A, A

short CPU::opcode0xB8() {
    instructionSet->compare8BitRegister(AF.hi, BC.hi, &AF.low);
    return 4;
}// CP A, B

short CPU::opcode0xB9() {
    instructionSet->compare8BitRegister(AF.hi, BC.low, &AF.low);
    return 4;
}// CP A, C

short CPU::opcode0xBA() {
    instructionSet->compare8BitRegister(AF.hi, DE.hi, &AF.low);
    return 4;
}// CP A, D

short CPU::opcode0xBB() {
    instructionSet->compare8BitRegister(AF.hi, DE.low, &AF.low);
    return 4;
}// CP A, E

short CPU::opcode0xBC() {
    instructionSet->compare8BitRegister(AF.hi, HL.hi, &AF.low);
    return 4;
}// CP A, H

short CPU::opcode0xBD() {
    instructionSet->compare8BitRegister(AF.hi, HL.low, &AF.low);
    return 4;
}// CP A, L

short CPU::opcode0xBE() {
    byte data = memory->read(HL.value);
    instructionSet->compare8BitRegister(AF.hi, data, &AF.low);
    return 8;
}// CP A, (HL) 8 cycles

short CPU::opcode0xFE() {
    byte data = memory->read(PC.value);
    incrementProgramCounter();
    instructionSet->compare8BitRegister(AF.hi, data, &AF.low);
    return 8;
}// CP A, nn 8 cycles

short CPU::opcode0x2F() {
    instructionSet->cpl(&AF.hi, &AF.low);
    return 4;
}

short CPU::opcode0x27() {
    instructionSet->daa(&AF.hi, &AF.low);
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