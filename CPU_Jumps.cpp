#include "CPU.h"

////////////////////////////////////////////////////// RESET OPERATIONS
void CPU::rst(const word address) {
    stackPush(PC);
    PC.value = address;
}

short CPU::opcode0xC7() {
    rst(0x00);
    return 32;
}// call 0x00

short CPU::opcode0xCF() {
    rst(0x08);
    return 32;
}// call 0x08

short CPU::opcode0xD7() {
    rst(0x10);
    return 32;
}// call 0x10

short CPU::opcode0xDF() {
    rst(0x18);
    return 32;
}// call 0x18

short CPU::opcode0xE7() {
    rst(0x20);
    return 32;
}// call 0x20

short CPU::opcode0xEF() {
    rst(0x28);
    return 32;
}// call 0x28

short CPU::opcode0xF7() {
    rst(0x30);
    return 32;
}// call 0x30

short CPU::opcode0xFF() {
    rst(0x38);
    return 32;
}// call 0x38

/////////////////////////////////////////////// JUMPS
short CPU::opcode0x18(){
    PC.value = PC.value + 1 + static_cast<char>(memory->read(PC.value));
    return 8;
} // JP PC, nn

short CPU::opcode0x20(){
    if ( !checkFlag(ZERO_FLAG) ) {
        PC.value = PC.value + static_cast<char>(memory->read(PC.value));
    }
    incrementProgramCounter();
    return 8;
} // JR NZ, n

short CPU::opcode0x28(){
    if ( checkFlag(ZERO_FLAG) ) {
        PC.value = PC.value + static_cast<char>(memory->read(PC.value));
    }
    incrementProgramCounter();
    return 8;
} // JR Z,n

short CPU::opcode0x30(){
    if ( !checkFlag(CARRY_FLAG) ) {
        PC.value = PC.value + static_cast<char>(memory->read(PC.value));
    }
    incrementProgramCounter();
    return 8;
} // JR NC,n

short CPU::opcode0x38(){
    if ( checkFlag(CARRY_FLAG) ) {
        PC.value = PC.value + static_cast<char>(memory->read(PC.value));
    }
    incrementProgramCounter();
    return 8;
} // JR C,n

//////////////////////////////////////////////// JUMPS WITHOUT DATA
short CPU::opcode0xE9() {
    PC.value = HL.value;
    return 4;
}// JP HL  4

short CPU::opcode0xC3() {
    byte low = memory->read(PC.value);
    byte hi = memory->read(PC.value + 1);
    PC.hi = hi;
    PC.low = low;
    return 12;
}// JP nn 16

short CPU::opcode0xC2() {
    if ( !checkFlag(ZERO_FLAG) ) {
        byte low = memory->read(PC.value);
        incrementProgramCounter();
        byte hi = memory->read(PC.value);
        incrementProgramCounter();
        PC.hi = hi;
        PC.low = low;
    } else {
        incrementProgramCounter();
        incrementProgramCounter();
    }
    
    return 12;
}// JP NZ, nn

short CPU::opcode0xCA() {
    if ( checkFlag(ZERO_FLAG) ) {
        byte low = memory->read(PC.value);
        incrementProgramCounter();
        byte hi = memory->read(PC.value);
        incrementProgramCounter();
        PC.hi = hi;
        PC.low = low;
    } else {
        incrementProgramCounter();
        incrementProgramCounter();
    }
    return 12;
}// JP Z,nn

short CPU::opcode0xD2() {
    if ( !checkFlag(CARRY_FLAG) ) {
        byte low = memory->read(PC.value);
        incrementProgramCounter();
        byte hi = memory->read(PC.value);
        incrementProgramCounter();
        PC.hi = hi;
        PC.low = low;
    } else {
        incrementProgramCounter();
        incrementProgramCounter();
    }
    return 12;
}// JP NC, nn

short CPU::opcode0xDA() {
    if ( checkFlag(CARRY_FLAG) ) {
        byte low = memory->read(PC.value);
        incrementProgramCounter();
        byte hi = memory->read(PC.value);
        incrementProgramCounter();
        PC.hi = hi;
        PC.low = low;
    } else {
        incrementProgramCounter();
        incrementProgramCounter();
    }
    return 12;
}// JP C, nn


///////////////////////////////// CALLS
short CPU::opcode0xCD() {
    
    byte low = memory->read(PC.value);
    incrementProgramCounter();
    byte hi = memory->read(PC.value);
    incrementProgramCounter();
    
    stackPush(PC);
    
    PC.hi = hi;
    PC.low = low;
    
    return 12;
}//  call nn

short CPU::opcode0xC4() {
    if ( !checkFlag(ZERO_FLAG) ) {
        byte low = memory->read(PC.value);
        incrementProgramCounter();
        byte hi = memory->read(PC.value);
        incrementProgramCounter();
        stackPush(PC);
        PC.hi = hi;
        PC.low = low;
    } else {
        incrementProgramCounter();
        incrementProgramCounter();
    }
    
    return 12;
}//  call NZ, nn

short CPU::opcode0xCC() {
    if ( checkFlag(ZERO_FLAG) ) {
        byte low = memory->read(PC.value);
        incrementProgramCounter();
        byte hi = memory->read(PC.value);
        incrementProgramCounter();
        stackPush(PC);
        PC.hi = hi;
        PC.low = low;
    } else {
        incrementProgramCounter();
        incrementProgramCounter();
    }
    
    return 12;
}//  call Z, nn

short CPU::opcode0xD4() {
    if ( !checkFlag(CARRY_FLAG) ) {
        byte low = memory->read(PC.value);
        incrementProgramCounter();
        byte hi = memory->read(PC.value);
        incrementProgramCounter();
        stackPush(PC);
        PC.hi = hi;
        PC.low = low;
    } else {
        incrementProgramCounter();
        incrementProgramCounter();
    }
    
    return 12;
}//  call NC, nn

short CPU::opcode0xDC() {
    if ( checkFlag(CARRY_FLAG) ) {
        byte low = memory->read(PC.value);
        incrementProgramCounter();
        byte hi = memory->read(PC.value);
        incrementProgramCounter();
        stackPush(PC);
        PC.hi = hi;
        PC.low = low;
    } else {
        incrementProgramCounter();
        incrementProgramCounter();
    }
    
    return 12;
}//  call C, nn


///////////////////////////////////////////////// returns
short CPU::opcode0xC9() {
    stackPop(&PC);
    return 8;
}// ret

short CPU::opcode0xC0() {
    if ( !checkFlag(ZERO_FLAG) ) {
        stackPop(&PC);
    }
    return 8;
}// ret NZ

short CPU::opcode0xC8() {
    if ( checkFlag(ZERO_FLAG) ) {
        stackPop(&PC);
    }
    return 8;
}// ret Z

short CPU::opcode0xD0() {
    if ( !checkFlag(CARRY_FLAG) ) {
        stackPop(&PC);
    }
    return 8;
}// ret NC

short CPU::opcode0xD8() {
    if ( checkFlag(CARRY_FLAG) ) {
        stackPop(&PC);
    }
    return 8;
}// ret C

short CPU::opcode0xD9() {
    // RETI
    stackPop(&PC);
    ime = true;
    return 8;
}