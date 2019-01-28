#include "../CPU.h"


//////////////////////////////// CPU CONTROL COMMANDS
short CPU::opcode0x76() {
    
	halt = true;
	
	/* Check halt bug */
	byte interruptEnabledFlag = memory->read(IE_REGISTER);
	byte interruptFlag = memory->read(IF_REGISTER);

	if (!ime && (interruptFlag & interruptEnabledFlag & 0x1F)) {
		halt_bug = true;
	}

    return 0;
}

short CPU::opcode0xF3() {
    ime = false;
    return 4;
}

short CPU::opcode0xFB() {
    ime = true;
    return 4;
}

short CPU::opcode0x3F() {
	clearFlag(ADD_SUB_FLAG);
	clearFlag(HALF_CARRY_FLAG);
	toggleFlag(CARRY_FLAG);
    return 4;
}// ccf -00c cy = cy xor 1

short CPU::opcode0x37() {
	clearFlag(ADD_SUB_FLAG);
	clearFlag(HALF_CARRY_FLAG);
	raiseFlag(CARRY_FLAG);
	return 4;
}// scf -001 cy = 1

short CPU::opcode0x00() {
    //NOP
    return 4;
}

/////////////////////////////////////////////// STOP
short CPU::opcode0x10() {
    incrementProgramCounter();
    return 0;
}

short CPU::invalidOpcode() {
    return 0;
}
