#include "InstructionSet.h"

/* ALU INSTRUCTIONS */

void InstructionSet::increment16BitRegister(Register *reg) {
    reg->value++;
}

void InstructionSet::increment8BitRegister(byte *reg, byte *flags) {
    *reg = *reg + 1;
    
    clearFlag(ADD_SUB_FLAG, flags);
    
    if (*reg == 0x00) {
        raiseFlag(ZERO_FLAG, flags);
    } else {
        clearFlag(ZERO_FLAG, flags);
    }
    
    if (*reg & 0x0F) {
        clearFlag(HALF_CARRY_FLAG, flags);
    } else {
        raiseFlag(HALF_CARRY_FLAG, flags);
    }
}

void InstructionSet::decrement16BitRegister(Register *reg) {
    reg->value--;
}

void InstructionSet::decrement8BitRegister(byte *reg, byte *flags) {
    *reg = *reg -1;
    
    raiseFlag(ADD_SUB_FLAG, flags);
    if (*reg == 0x00) {
        raiseFlag(ZERO_FLAG, flags);
    } else {
        clearFlag(ZERO_FLAG, flags);
    }
    
    if ((*reg & 0x0F) == 0x0F) {
        raiseFlag(HALF_CARRY_FLAG, flags);
    } else {
        clearFlag(HALF_CARRY_FLAG, flags);
    }
}

void InstructionSet::add16BitRegister(Register *original, const Register adding, byte *flags) {
    
    clearFlag(ADD_SUB_FLAG, flags);
    
    if (((original->value & 0x0FFF) + (adding.value & 0x0FFF)) > 0x0FFF) {
        raiseFlag(HALF_CARRY_FLAG, flags);
    } else {
        clearFlag(HALF_CARRY_FLAG, flags);
    }
    
    if ((original->value + adding.value) > 0xFFFF) {
        raiseFlag(CARRY_FLAG, flags);
    } else {
        clearFlag(CARRY_FLAG, flags);
    }
    
    original->value = original->value + adding.value;
}

/* COMMON INSTRUCTIONS */
void InstructionSet::raiseFlag(Flag flag, byte *reg) {
    switch ( flag ) {
        case ZERO_FLAG:
            setBit(reg, ZERO_FLAG);
            break;
        case ADD_SUB_FLAG:
            setBit(reg, ADD_SUB_FLAG);
            break;
        case HALF_CARRY_FLAG:
            setBit(reg, HALF_CARRY_FLAG);
            break;
        case CARRY_FLAG:
            setBit(reg, CARRY_FLAG);
            break;
            break;
    }
}

void InstructionSet::toggleFlag(Flag flag, byte *reg) {
    switch ( flag ) {
        case ZERO_FLAG:
            toggleBit(reg, ZERO_FLAG);
            break;
        case ADD_SUB_FLAG:
            toggleBit(reg, ADD_SUB_FLAG);
            break;
        case HALF_CARRY_FLAG:
            toggleBit(reg, HALF_CARRY_FLAG);
            break;
        case CARRY_FLAG:
            toggleBit(reg, CARRY_FLAG);
            break;
    }
}

void InstructionSet::clearFlag(Flag flag, byte *reg) {
    switch ( flag ) {
        case ZERO_FLAG:
            clearBit(reg, ZERO_FLAG);
            break;
        case ADD_SUB_FLAG:
            clearBit(reg, ADD_SUB_FLAG);
            break;
        case HALF_CARRY_FLAG:
            clearBit(reg, HALF_CARRY_FLAG);
            break;
        case CARRY_FLAG:
            clearBit(reg, CARRY_FLAG);
            break;
    }
}

bool InstructionSet::checkFlag(Flag flag, const byte reg) {
    bool result = false;
    switch ( flag ) {
        case ZERO_FLAG:
            result = isBitSet(reg, ZERO_FLAG);
            break;
        case ADD_SUB_FLAG:
            result = isBitSet(reg, ADD_SUB_FLAG);
            break;
        case HALF_CARRY_FLAG:
            result = isBitSet(reg, HALF_CARRY_FLAG);
            break;
        case CARRY_FLAG:
            result = isBitSet(reg, CARRY_FLAG);
            break;
    }
    return result;
}

void InstructionSet::clearFlags(byte *reg) {
    reg = 0;
}