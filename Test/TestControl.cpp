#include "gtest/gtest.h"
#include "InstructionSet.h"
#include "Types.h"

class CPU_CONTROL_TEST : public ::testing::Test {
protected:
    InstructionSet* set;
    byte flags;
    
    CPU_CONTROL_TEST() {
        set = new InstructionSet;
    }
    
    ~CPU_CONTROL_TEST() {
        delete set;
    }
};

TEST_F(CPU_CONTROL_TEST, ccf_no_raised_flags) {
    flags = 0x00;
    set->ccf(&flags);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F(CPU_CONTROL_TEST, ccf_raised_flags) {
    flags = 0x00;
    flags = 0x30;
    set->ccf(&flags);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_FALSE(isBitSet(flags, CARRY_FLAG));
}

TEST_F(CPU_CONTROL_TEST, scf_no_raised_flags) {
    flags = 0x00;
    set->scf(&flags);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}

TEST_F(CPU_CONTROL_TEST, scf_raised_flags) {
    flags = 0xF0;
    set->scf(&flags);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
}