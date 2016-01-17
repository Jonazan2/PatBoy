#include "gtest/gtest.h"
#include "Utils.h"

TEST (UTILS_H, isBitSet) {
    byte b = 0x0F;
    EXPECT_FALSE(isBitSet(b,7));
    EXPECT_TRUE(isBitSet(b, 3));
    EXPECT_FALSE(isBitSet(b, 8));
    EXPECT_FALSE(isBitSet(b, -1));
}

TEST (UTILS_H, getBitValue) {
    EXPECT_EQ(0x01, getBitValue(0x01, 0));
    EXPECT_EQ(0x01, getBitValue(0xAF, 7));
    EXPECT_EQ(0x00, getBitValue(0xFF, 8));
    EXPECT_EQ(0x00, getBitValue(0xFF, -1));
}

TEST (UTILS_H, setBit) {
    byte value = 0x00;
    setBit(&value, 1);
    EXPECT_EQ(0x02, value);
    setBit(&value, 1);
    EXPECT_EQ(0x02, value);
    setBit(&value, 0);
    EXPECT_EQ(0x03, value);
    setBit(&value, -1);
    EXPECT_EQ(0x03, value);
    setBit(&value, 8);
    EXPECT_EQ(0x03, value);
}

TEST (UTILS_H, toogleBit) {
    byte value = 0x00;
    toggleBit(&value, 0);
    EXPECT_EQ(0x01, value);
    toggleBit(&value, 0);
    EXPECT_EQ(0x00, value);
    toggleBit(&value, 8);
    EXPECT_EQ(0x00, value);
    toggleBit(&value, -1);
    EXPECT_EQ(0x00, value);
}

TEST (UTILS_H, clearBit) {
    byte value = 0xFF;
    clearBit(&value, 7);
    EXPECT_EQ(0X7F, value);
    clearBit(&value, 0);
    EXPECT_EQ(0x7E, value);
    clearBit(&value, -1);
    EXPECT_EQ(0x7E, value);
    clearBit(&value, 8);
    EXPECT_EQ(0x7E, value);
}