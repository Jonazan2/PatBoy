#ifndef InstructionSet_h
#define InstructionSet_h

#include "Types.h"
#include "Utils.h"

class InstructionSet {

public:
    /* ALU operations */
    void increment16BitRegister(Register *);
    void increment8BitRegister(byte *, byte*);
    void decrement16BitRegister(Register *);
    void decrement8BitRegister(byte *, byte *);
    void add16BitRegister(Register *, const Register, byte *);
    void add8BitRegister(byte *, const byte, byte *);
    void adc8BitRegister(byte *, const byte, byte *);
    void sub8BitRegister(byte *, const byte, byte *);
    void sbc8BitRegister(byte *, const byte, byte *);
    void xor8BitRegister(byte *, const byte, byte *);
    void or8BitRegister(byte *, const byte, byte *);
    void and8BitRegister(byte *, const byte, byte *);
    void compare8BitRegister(const byte, const byte, byte *);
    void cpl(byte *, byte *);
    void daa(byte *, byte *);
    void addStackPointer(Register *, const byte, byte *);
    

private:
    void raiseFlag(Flag, byte *);
    bool checkFlag(Flag, const byte);
    void toggleFlag(Flag, byte *);
    void clearFlag(Flag, byte *);
    void clearFlags(byte *);
};

#endif /* InstructionSet_h */
