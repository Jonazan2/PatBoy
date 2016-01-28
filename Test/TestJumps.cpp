#include "gtest/gtest.h"
#include "InstructionSet.h"
#include "Types.h"

class CPU_JUMP_TEST : public ::testing::Test {
protected:
    InstructionSet* set;
    Register reg;
    byte flags;
    
    CPU_JUMP_TEST() {
        set = new InstructionSet;
    }
    
    virtual void SetUp() {
        reg.value = 0x00;
        flags = 0x00;
    }
    
    ~CPU_JUMP_TEST() {
        delete set;
    }
};

TEST_F (CPU_JUMP_TEST, jump) {
    set->jump(&reg, 0x2222);
    ASSERT_EQ(reg.value, 0x2222);
}