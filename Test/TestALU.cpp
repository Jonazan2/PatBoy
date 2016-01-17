#include "gtest/gtest.h"
#include "InstructionSet.h"
#include "Types.h"

TEST (CPU_ALU, increment16BitRegister) {
    InstructionSet *set = new InstructionSet();
    Register reg;
    reg.value = 0x00;
    
    set->increment16BitRegister(&reg);
    ASSERT_EQ(reg.value, 0x01);
    
    reg.value = 0xFF;
    set->increment16BitRegister(&reg);
    ASSERT_EQ(reg.value, 0x100);
    
    reg.value = 0xFFFF;
    set->increment16BitRegister(&reg);
    ASSERT_EQ(reg.value, 0x0000);
}
