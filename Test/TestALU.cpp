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

TEST (CPU_ALU, increment8BitRegister) {
    InstructionSet *set = new InstructionSet();
    byte data = 0x00;
    byte flags = 0x00;
    
    set->increment8BitRegister(&data, &flags);
    ASSERT_EQ(data, 0x01);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_FALSE(isBitSet(flags, HALF_CARRY_FLAG));
    
    data = 0x0F;
    flags = 0xF0;
    set->increment8BitRegister(&data, &flags);
    ASSERT_EQ(data, 0x10);
    ASSERT_FALSE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
    ASSERT_TRUE(isBitSet(flags, CARRY_FLAG));
    
    data = 0xFF;
    flags = 0x00;
    set->increment8BitRegister(&data, &flags);
    ASSERT_EQ(data, 0x00);
    ASSERT_TRUE(isBitSet(flags, ZERO_FLAG));
    ASSERT_FALSE(isBitSet(flags, ADD_SUB_FLAG));
    ASSERT_TRUE(isBitSet(flags, HALF_CARRY_FLAG));
}
