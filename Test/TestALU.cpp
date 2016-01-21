#include "gtest/gtest.h"
#include "InstructionSet.h"
#include "Types.h"

class CPU_ALU_TEST : public ::testing::Test {
    protected:
        InstructionSet* set;
        Register reg;
        byte flags;
    
        CPU_ALU_TEST() {
            set = new InstructionSet;
        }
    
        virtual void SetUp() {
            reg.value = 0x00;
            flags = 0x00;
        }
    
        ~CPU_ALU_TEST() {
            delete set;
        }
};

TEST_F (CPU_ALU_TEST, increment16BitRegister_happy) {
    set->increment16BitRegister(&reg);
    ASSERT_EQ(reg.value, 0x01);
}

TEST_F (CPU_ALU_TEST, increment16BitRegister_half_carry) {
    reg.value = 0xFF;
    set->increment16BitRegister(&reg);
    ASSERT_EQ(reg.value, 0x100);
}

TEST_F (CPU_ALU_TEST, increment16BitRegister_overflow) {
    reg.value = 0xFFFF;
    set->increment16BitRegister(&reg);
    ASSERT_EQ(reg.value, 0x00);
}

TEST_F (CPU_ALU_TEST, increment8BitRegister_happy) {
    set->increment8BitRegister(&reg.hi, &flags);
    ASSERT_EQ(reg.hi, 0x01);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
}


TEST_F (CPU_ALU_TEST, increment8BitRegister_half_carry) {
    reg.hi = 0x0F;
    flags = 0xF0;
    set->increment8BitRegister(&reg.hi, &flags);
    ASSERT_EQ(reg.hi, 0x10);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, increment8BitRegister_overflow_zero) {
    reg.hi = 0xFF;
    flags = 0x00;
    set->increment8BitRegister(&reg.hi, &flags);
    ASSERT_EQ(reg.hi, 0x00);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
}


TEST_F (CPU_ALU_TEST, decrement16BitRegister_happy) {
    reg.value = 0xFFFF;
    set->decrement16BitRegister(&reg);
    ASSERT_EQ(reg.value, 0xFFFE);
}

TEST_F (CPU_ALU_TEST, decrement16BitRegister_half_carry) {
    reg.value = 0x0100;
    set->decrement16BitRegister(&reg);
    ASSERT_EQ(reg.value, 0x00FF);
}

TEST_F (CPU_ALU_TEST, decrement16BitRegister_overflow) {
    reg.value = 0x0000;
    set->decrement16BitRegister(&reg);
    ASSERT_EQ(reg.value, 0xFFFF);
}


TEST_F (CPU_ALU_TEST, decrement8BitRegister_happy) {
    reg.low = 0xFF;
    set->decrement8BitRegister(&reg.low, &flags);
    ASSERT_EQ(reg.low, 0xFE);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, decrement8BitRegister_half_carry) {
    reg.low = 0x10;
    flags = 0x00;
    set->decrement8BitRegister(&reg.low, &flags);
    ASSERT_EQ(reg.low, 0x0F);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, decrement8BitRegister_zero) {
    reg.hi = 0x01;
    flags = 0xF0;
    set->decrement8BitRegister(&reg.hi, &flags);
    ASSERT_EQ(reg.hi, 0x00);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, decrement8BitRegister_overflow) {
    reg.hi = 0x00;
    flags = 0xF0;
    set->decrement8BitRegister(&reg.hi, &flags);
    ASSERT_EQ(reg.hi, 0xFF);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, add16BitRegister_happy) {
    Register adding;
    adding.value = 0x00A0;
    set->add16BitRegister(&reg, adding, &flags);
    ASSERT_EQ(reg.value, 0x00A0);
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, add16BitRegister_half_carry) {
    reg.value = 0x0999;
    Register adding;
    adding.value = 0x0999;
    set->add16BitRegister(&reg, adding, &flags);
    ASSERT_EQ(reg.value, 0x1332);
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, add16BitRegister_overflow) {
    reg.value = 0xFFFF;
    Register adding;
    adding.value = 0xFFFF;
    set->add16BitRegister(&reg, adding, &flags);
    ASSERT_EQ(reg.value, 0xFFFE);
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, add8BitRegister_happy) {
    reg.hi = 0x05;
    set->add8BitRegister(&reg.hi, 0x09, &flags);
    ASSERT_EQ(reg.hi, 0x0E);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, add8BitRegister_half_carry) {
    reg.hi = 0x08;
    set->add8BitRegister(&reg.hi, 0x09, &flags);
    ASSERT_EQ(reg.hi, 0x11);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, add8BitRegister_zero_flag) {
    reg.hi = 0x80;
    set->add8BitRegister(&reg.hi, 0x80, &flags);
    ASSERT_EQ(reg.hi, 0x00);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, add8BitRegister_overflow) {
    reg.hi = 0xFF;
    set->add8BitRegister(&reg.hi, 0xFF, &flags);
    ASSERT_EQ(reg.hi, 0xFE);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, adc8BitRegister_happy) {
    set->adc8BitRegister(&reg.hi, 0xFE, &flags);
    ASSERT_EQ(reg.hi, 0xFE);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, adc8BitRegister_zero_flag) {
    reg.hi = 0x80;
    flags = 0x10;
    set->adc8BitRegister(&reg.hi, 0x7F, &flags);
    ASSERT_EQ(reg.hi, 0x00);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, adc8BitRegister_half_carry) {
    reg.hi = 0x0F;
    flags = 0x10;
    set->adc8BitRegister(&reg.hi, 0x0E, &flags);
    ASSERT_EQ(reg.hi, 0x1E);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, adc8BitRegister_overflow) {
    reg.hi = 0x86;
    flags = 0x10;
    set->adc8BitRegister(&reg.hi, 0x89, &flags);
    ASSERT_EQ(reg.hi, 0x10);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, sub8BitRegister_happy) {
    reg.low = 0x09;
    set->sub8BitRegister(&reg.low, 0x07, &flags);
    ASSERT_EQ(reg.low, 0x02);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, sub8BitRegister_zero_flag) {
    reg.low = 0x09;
    set->sub8BitRegister(&reg.low, 0x09, &flags);
    ASSERT_EQ(reg.low, 0x00);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, sub8BitRegister_half_carry) {
    reg.low = 0x10;
    set->sub8BitRegister(&reg.low, 0x02, &flags);
    ASSERT_EQ(reg.low, 0x0E);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, sub8BitRegister_overflow) {
    reg.low = 0x10;
    set->sub8BitRegister(&reg.low, 0x12, &flags);
    ASSERT_EQ(reg.low, 0xFE);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, sbc8BitRegister_happy) {
    reg.low = 0x09;
    flags = 0x10;
    set->sbc8BitRegister(&reg.low, 0x03, &flags);
    ASSERT_EQ(reg.low, 0x05);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, sbc8BitRegister_zero_flag) {
    reg.low = 0x10;
    flags = 0x00;
    set->sbc8BitRegister(&reg.low, 0x10, &flags);
    ASSERT_EQ(reg.low, 0x00);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, sbc8BitRegister_half_carry) {
    reg.low = 0x10;
    flags = 0x10;
    set->sbc8BitRegister(&reg.low, 0x07, &flags);
    ASSERT_EQ(reg.low, 0x08);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, sbc8BitRegister_overflow) {
    reg.low = 0x10;
    flags = 0x10;
    set->sbc8BitRegister(&reg.low, 0x10, &flags);
    ASSERT_EQ(reg.low, 0xFF);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}


TEST_F (CPU_ALU_TEST, xor8BitRegister_happy) {
    reg.low = 0x08;
    flags = 0xF0;
    set->xor8BitRegister(&reg.low, 0x09, &flags);
    ASSERT_EQ(reg.low, 0x01);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, xor8BitRegister_zero_flag) {
    reg.low = 0x09;
    flags = 0xF0;
    set->xor8BitRegister(&reg.low, 0x09, &flags);
    ASSERT_EQ(reg.low, 0x00);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, or8BitRegister_happy) {
    reg.hi = 0x12;
    flags = 0xF0;
    set->xor8BitRegister(&reg.hi, 0x08, &flags);
    ASSERT_EQ(reg.hi, 0x1A);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, or8BitRegister_zero_flag) {
    reg.hi = 0x00;
    flags = 0xF0;
    set->xor8BitRegister(&reg.hi, 0x00, &flags);
    ASSERT_EQ(reg.hi, 0x00);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, and8BitRegister_happy) {
    reg.hi = 0xFC;
    flags = 0x00;
    set->and8BitRegister(&reg.hi, 0x15, &flags);
    ASSERT_EQ(reg.hi, 0x14);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, and8BitRegister_zero_flag) {
    reg.hi = 0x11;
    flags = 0xF0;
    set->and8BitRegister(&reg.hi, 0x08, &flags);
    ASSERT_EQ(reg.hi, 0x00);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, compare8BitRegister_happy_not_equal) {
    reg.hi = 0x09;
    flags = 0xF0;
    set->compare8BitRegister(reg.hi, 0x08, &flags);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, compare8BitRegister_happy_equal) {
    reg.hi = 0x09;
    flags = 0xF0;
    set->compare8BitRegister(reg.hi, 0x09, &flags);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, compare8BitRegister_half_carry) {
    reg.hi = 0x11;
    flags = 0xF0;
    set->compare8BitRegister(reg.hi, 0x09, &flags);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, compare8BitRegister_carry_flag) {
    reg.hi = 0x11;
    flags = 0xF0;
    set->compare8BitRegister(reg.hi, 0x12, &flags);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, cpl_happy) {
    reg.hi = 0x01;
    set->cpl(&reg.hi, &flags);
    ASSERT_EQ(reg.hi, 0xFE);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, cpl_happy_keeping_flags) {
    reg.hi = 0xFF;
    flags = 0xF0;
    set->cpl(&reg.hi, &flags);
    ASSERT_EQ(reg.hi, 0x00);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_TRUE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, addStackPointer_happy) {
    reg.value = 0x000E;
    set->addStackPointer(&reg, 0x01, &flags);
    ASSERT_EQ(reg.value, 0x000F);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, addStackPointer_half_carry) {
    reg.value = 0x000E;
    flags = 0x70;
    set->addStackPointer(&reg, 0x07, &flags);
    ASSERT_EQ(reg.value, 0x0015);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, addStackPointer_carry) {
    reg.value = 0x00EE;
    flags = 0x10;
    set->addStackPointer(&reg, 0x17, &flags);
    ASSERT_EQ(reg.value, 0x0105);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F (CPU_ALU_TEST, addDAAAlignment_happy) {
    reg.hi = 0x15;
    set->add8BitRegister(&reg.hi, 0x27, &flags);
    ASSERT_EQ(reg.hi, 0x003C);
    set->daa(&reg.hi, &flags);
    ASSERT_EQ(reg.hi, 0x42);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}