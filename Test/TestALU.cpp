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