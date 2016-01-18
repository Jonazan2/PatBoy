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
    void add8BitRegister(byte *, const byte);
    void adc8BitRegister(byte *, const byte);
    void sub8BitRegister(byte *, const byte);
    void sbc8BitRegister(byte *, const byte);
    void xor8BitRegister(byte *, const byte);
    void or8BitRegister(byte *, const byte);
    void and8BitRegister(byte *, const byte);
    void compare8BitRegister(byte *, const byte);
    void cpl(byte *);
    void dda(byte *);
    

private:
    void raiseFlag(Flag, byte *);
    bool checkFlag(Flag, const byte);
    void toggleFlag(Flag, byte *);
    void clearFlag(Flag, byte *);
    void clearFlags(byte *);
};

#endif /* InstructionSet_h */
