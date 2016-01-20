#ifndef Cartridge_CPU_h
#define Cartridge_CPU_h

#include "Types.h"
#include "Memory.h"
#include "InstructionSet.h"
#include "Utils.h"

class Memory;

/**************************************************************************//**
 * \brief Class that implements the CPU system of the Nintendo Game Boy
 *
 * This class represent the CPU of the Game Boy. It has an array of functions
 * that contains each opcode function and an array with the same funcionality but
 * for the extended opcode operation. It handles the interrupts and timers.
 *
 * See also \ref Memmory \ref Utils
******************************************************************************/
class CPU {

public:
    CPU(Memory *);
    unsigned int update();
    void requestInterrupt(Interrupts);
    void updateTimers(int);
    void updateInterrupts();
    void resetDivRegister();
    void resetTimaRegister();
    void setCurrentClockSpeed(const int);
    int getCurrrentClockSpeed() const;
    Register getAF();
    
    ~CPU();
    
private:
    typedef short (CPU::*Opcode) (void);
    
    Register AF;
    Register BC;
    Register DE;
    Register HL;
    Register SP;
    Register PC;
    
    InstructionSet *instructionSet;
    
    bool halt;
    bool ime;
    
    Memory *memory;
    Opcode opcodes[256];
    Opcode extendedOpcodes[256];
    
    unsigned short divRegister;
    unsigned int timaCounter;
    int	currentClockSpeed;
    
    bool interruptPendingEnabled;
    bool interruptPendingDisabled;
    void serviceInterrupt(Interrupts);
    
    void chargeOpcodes();
    void chargeExtendedOpcodes();
    byte getCurrentOpcode();
    short executeOpcode(byte);
    void incrementProgramCounter();
    void decrementProgramCounter();
    void incrementStackPointer();
    void decrementStackPointer();
    void stackPush(const Register);
    void stackPop(Register *);
    
    void printRegisterState();
    void printOpcodeName(const byte, const bool);
    
    // Flag related operations
    void raiseFlag(Flag); // set flag = 1, if it is 1 doesn't change
    bool checkFlag(Flag); // return true if flag = 1
    void toggleFlag(Flag); // if flag = 1 then changes it to 0. if flag = 0, then changes it to 1.
    void clearFlag(Flag); // The flag will be 0 after this operation
    void clearFlags(); // Put flags to 0
    void checkZeroFlag(const byte);
    void checkZeroFlag(const word);
    void checkHalfCarryFlag(const byte);
    void checkHalfCarryFlag(const word, const word);
    void checkCarryFlag(const byte);
    void checkCarryFlag(const word);
    
    // reset
    void reset();
    
    short invalidOpcode();
    
    // 16 bits loads
    void  load16BitRegister(Register *);
    short opcode0x01(); // Load BC, nn
    short opcode0x11(); // Load DE, nn
    short opcode0x21(); // Load HL, nn
    short opcode0x31(); // Load SP, nn
    short opcode0xF9(); // Load SP, HL
    short opcode0x08(); // LD (nn), SP
    
    // 16 bits increments
    short opcode0x03(); // INC BC
    short opcode0x13(); // INC DE
    short opcode0x23(); // INC HL
    short opcode0x33(); // INC SP
    
    // 16 bits decrements
    short opcode0x0B();
    short opcode0x1B();
    short opcode0x2B();
    short opcode0x3B();
    
    // 8 bits loads -> flags not affected LD r,n
    void  loadRegister(byte *);
    short opcode0x06(); // LD B,n load
    short opcode0x0E(); // LD C,n load
    short opcode0x16(); // LD D,n load
    short opcode0x1E(); // LD E,n load
    short opcode0x26(); // LD H,n load
    short opcode0x2E(); // LD L,n load
    short opcode0x3E(); // LD A,n
    
    // LD r,r 4
    void load8BitRegister(byte *, const byte);
    
    // LD A,r
    short opcode0x7F(); // LD A, A
    short opcode0x78(); // LD A, B
    short opcode0x79(); // LD A, C
    short opcode0x7A(); // LD A, D
    short opcode0x7B(); // LD A, E
    short opcode0x7C(); // LD A, H
    short opcode0x7D(); // LD A, L
    
    // LD B,r
    short opcode0x40(); // LD B, B
    short opcode0x41(); // LD B, C
    short opcode0x42(); // LD B, D
    short opcode0x43(); // LD B, E
    short opcode0x44(); // LD B, H
    short opcode0x45(); // LD B, L
    
    // LD C,r
    short opcode0x48(); // LD C, B
    short opcode0x49(); // LD C, C
    short opcode0x4A(); // LD C, D
    short opcode0x4B(); // LD C, E
    short opcode0x4C(); // LD C, H
    short opcode0x4D(); // LD C, L
    
    // LD D,r
    short opcode0x50(); // LD D, B
    short opcode0x51(); // LD D, C
    short opcode0x52(); // LD D, D
    short opcode0x53(); // LD D, E
    short opcode0x54(); // LD D, H
    short opcode0x55(); // LD D, L
    
    // LD E,r
    short opcode0x58(); // LD E, B
    short opcode0x59(); // LD E, C
    short opcode0x5A(); // LD E, D
    short opcode0x5B(); // LD E, E
    short opcode0x5C(); // LD E, H
    short opcode0x5D(); // LD E, L
    
    // LD H, r
    short opcode0x60(); // LD H, B
    short opcode0x61(); // LD H, C
    short opcode0x62(); // LD H, D
    short opcode0x63(); // LD H, E
    short opcode0x64(); // LD H, H
    short opcode0x65(); // LD H, L
    
    // LD L, r
    short opcode0x68(); // LD L, B
    short opcode0x69(); // LD L, C
    short opcode0x6A(); // LD L, D
    short opcode0x6B(); // LD L, E
    short opcode0x6C(); // LD L, H
    short opcode0x6D(); // LD L, L
    
    // LD (HL), r 8
    short opcode0x70(); // LD (HL), B
    short opcode0x71(); // LD (HL), C
    short opcode0x72(); // LD (HL), D
    short opcode0x73(); // LD (HL), E
    short opcode0x74(); // LD (HL), H
    short opcode0x75(); // LD (HL), L
    short opcode0x36(); // LD (HL), n 12
    
    // LD r, (HL) 8
    short opcode0x7E(); // LD A, (HL)
    short opcode0x46(); // LD B, (HL)
    short opcode0x4E(); // LD C, (HL)
    short opcode0x56(); // LD D, (HL)
    short opcode0x5E(); // LD E, (HL)
    short opcode0x66(); // LD H, (HL)
    short opcode0x6E(); // LD L, (HL)
    
    short opcode0x0A(); // LD A, (BC) 8
    short opcode0x1A(); // LD A, (DE) 8
    short opcode0xFA(); // LD A, (nn) 16
    
    short opcode0x22(); // LDi (HL), A 8
    short opcode0x2A(); // LDi A, (HL) 8
    short opcode0x32(); // LDd (HL), A 8
    short opcode0x3A(); // LDd A, (HL) 8
    
    // load to acummulator
    short opcode0x02(); // LD (BC), A 8
    short opcode0x12(); // LD (DE), A
    short opcode0x77(); // LD (HL), A
    short opcode0xEA(); // LD (nn), A 16
    short opcode0xE2(); // LD (0xFF00+C),A
    short opcode0xF0(); // LD A, (FF00+n) 12
    short opcode0xE0(); // LD (FF00+n), A 12
    short opcode0xF2(); // LD A, (FF00+C), 8
    
    // 8 bits add z0hc 4 cycles
    short opcode0x87(); // ADD A, A
    short opcode0x80(); // ADD A, B
    short opcode0x81(); // ADD A, C
    short opcode0x82(); // ADD A, D
    short opcode0x83(); // ADD A, E
    short opcode0x84(); // ADD A, H
    short opcode0x85(); // ADD A, L
    short opcode0x86(); // ADD A, (HL) 8
    short opcode0xC6(); // ADD A, n 8
    
    // 8 bits adc (with carry flag) z0hc 4 cycles
    short opcode0x8F(); // ADC A, A
    short opcode0x88(); // ADC A, B
    short opcode0x89(); // ADC A, C
    short opcode0x8A(); // ADC A, D
    short opcode0x8B(); // ADC A, E
    short opcode0x8C(); // ADC A, H
    short opcode0x8D(); // ADC A, L
    short opcode0x8E(); // ADC A, (HL) 8
    short opcode0xCE(); // ADC A, nn 8
    
    // 8-bit sub 4 cycles
    short opcode0x97(); // SUB A, A
    short opcode0x90(); // SUB A, B
    short opcode0x91(); // SUB A, C
    short opcode0x92(); // SUB A, D
    short opcode0x93(); // SUB A, E
    short opcode0x94(); // SUB A, H
    short opcode0x95(); // SUB A, L
    short opcode0x96(); // SUB A, (HL) 8
    short opcode0xD6(); // SUB A, n 8
    
    // 8-bit sbc (with carry flag) 4 cycles
    short opcode0x9F(); // SBC A, A
    short opcode0x98(); // SBC A, B
    short opcode0x99(); // SBC A, C
    short opcode0x9A(); // SBC A, D
    short opcode0x9B(); // SBC A, E
    short opcode0x9C(); // SBC A, H
    short opcode0x9D(); // SBC A, L
    short opcode0x9E(); // SBC A, (HL) 8
    short opcode0xDE(); // SBC A, n 8
    
    // 8 bit compare z1hc 4 cycles
    void compare8bitRegister(const Register, const byte);
    short opcode0xBF(); // CP A, A
    short opcode0xB8(); // CP A, B
    short opcode0xB9(); // CP A, C
    short opcode0xBA(); // CP A, D
    short opcode0xBB(); // CP A, E
    short opcode0xBC(); // CP A, H
    short opcode0xBD(); // CP A, L
    short opcode0xBE(); // CP A, (HL) 8 cycles
    short opcode0xFE(); // CP A, nn 8 cycles
    
    //8 bit increments -> flags affected
    short opcode0x3C(); // A increment 4
    short opcode0x04(); // B increment
    short opcode0x0C(); // C increment
    short opcode0x14(); // D increment
    short opcode0x1C(); // E increment
    short opcode0x24(); // H increment
    short opcode0x2C(); // L increment
    short opcode0x34(); // HL increment 12
    
    //8 bit decrements
    short opcode0x05(); // DEC B 4
    short opcode0x0D(); // DEC C
    short opcode0x15(); // DEC D
    short opcode0x1D(); // DEC E
    short opcode0x25(); // DEC H
    short opcode0x2D(); // DEC L
    short opcode0x3D(); // DEC A
    short opcode0x35(); // DEC HL 12
    
    // 8 bits XOR 4 cycles
    void xor8bitRegister(Register *, const byte);
    short opcode0xAF(); // XOR A, A
    short opcode0xA8(); // XOR A, B
    short opcode0xA9(); // XOR A, C
    short opcode0xAA(); // XOR A, D
    short opcode0xAB(); // XOR A, E
    short opcode0xAC(); // XOR A, H
    short opcode0xAD(); // XOR A, L
    short opcode0xAE(); // XOR A, (HL) 8
    short opcode0xEE(); // XOR A, n 8
    
    // 8 bits OR 4 cycles
    void or8bitRegister(Register *, const byte);
    short opcode0xB7(); // OR A, A
    short opcode0xB0(); // OR A, B
    short opcode0xB1(); // OR A, C
    short opcode0xB2(); // OR A, D
    short opcode0xB3(); // OR A, E
    short opcode0xB4(); // OR A, H
    short opcode0xB5(); // OR A, L
    short opcode0xB6(); // OR A, (HL) 8 cycles
    short opcode0xF6(); // OR A, n 8 cycles
    
    // 8 bit and
    void and8bitRegister(Register *, const byte data);
    short opcode0xA7(); // AND A, A
    short opcode0xA0(); // AND A, B
    short opcode0xA1(); // AND A, C
    short opcode0xA2(); // AND A, D
    short opcode0xA3(); // AND A, E
    short opcode0xA4(); // AND A, H
    short opcode0xA5(); // AND A, L
    short opcode0xA6(); // AND A, (HL) 8 cycles
    short opcode0xE6(); // AND A, n 8 cycles
    
    // ADD SP, nn (signed) - ld   HL,SP+dd
    short opcode0xE8();
    short opcode0xF8();
    
    // stack instructions
    short opcode0xC5(); // PUSH BC
    short opcode0xD5(); // PUSH DE
    short opcode0xE5(); // PUSH HL
    short opcode0xF5(); // PUSH AF
    short opcode0xC1(); // POP BC
    short opcode0xD1(); // POP DE
    short opcode0xE1(); // POP HL
    short opcode0xF1(); // POP AF -> F = F & 0xF0
    
    //16 bit add
    short opcode0x09(); // HL = HL + BC 8
    short opcode0x19(); // HL = HL + DE
    short opcode0x29(); // HL = HL + HL
    short opcode0x39(); // HL = HL + SP
    
    //RST instructions
    void rst(const word); // 16
    short opcode0xC7(); // 0x00
    short opcode0xCF(); // 0x08
    short opcode0xD7(); // 0x10
    short opcode0xDF(); // 0x18
    short opcode0xE7(); // 0x20
    short opcode0xEF(); // 0x28
    short opcode0xF7(); // 0x30
    short opcode0xFF(); // 0x38
    
    //jumps with inmediate data
    short opcode0x18();
    short opcode0x20();
    short opcode0x28();
    short opcode0x30();
    short opcode0x38();
    
    // jumps
    short opcode0xE9(); // JP HL  4
    short opcode0xC3(); // JP nn 16
    short opcode0xC2(); // JP NZ, nn
    short opcode0xCA(); // JP Z,nn
    short opcode0xD2(); // JP NC,nn
    short opcode0xDA(); // JP C,nn

    // calls
    short opcode0xCD(); //  call nn
    short opcode0xC4(); //  call NZ, nn
    short opcode0xCC(); //  call Z, nn
    short opcode0xD4(); //  call NC, nn
    short opcode0xDC(); //  call C, nn
    
    // returns
    short opcode0xC9(); // ret
    short opcode0xC0(); // ret NZ
    short opcode0xC8(); // ret Z
    short opcode0xD0(); // ret NC
    short opcode0xD8(); // ret C
    short opcode0xD9(); // reti
    
    // load into register 4
    void  loadIntoRegister(byte *, const byte);
    short opcode0x47(); // LD B,A
    short opcode0x4F(); // LD C,A
    short opcode0x57(); // LD D,A
    short opcode0x5F(); // LD E,A
    short opcode0x67(); // LD H,A
    short opcode0x6F(); // LD L,A
    
    // DAA
    short opcode0x27();
    
    // CPL
    short opcode0x2F();
    
    //GMB CPU-Controlcommands
    short opcode0x76(); // halt
    short opcode0xF3(); // di
    short opcode0xFB(); // ei
    short opcode0x3F(); // ccf -00c cy = cy xor 1
    short opcode0x37(); // scf -001 cy = 1
    short opcode0x00(); // nop
    short opcode0x10(); // stop
    
    
    // GMB rotate and shift commands
    void rlc8bitRegister(byte *);
    void rrc8bitRegister(byte *);
    void rl8bitRegister(byte *);
    void rr8bitRegister(byte *);
    void sla8bitRegister(byte *);
    void swap8bitRegister(byte *);
    void sra8bitRegister(byte *);
    void srl8bitRegister(byte *);
    
    short opcode0x07(); // RLC A
    short opcode0x0F(); // RRC A
    short opcode0x17(); // RL A
    short opcode0x1F(); // RR A
    
    short extendedOpcode0x00(); // RLC B
    short extendedOpcode0x01(); // RLC C
    short extendedOpcode0x02(); // RLC D
    short extendedOpcode0x03(); // RLC E
    short extendedOpcode0x04(); // RLC H
    short extendedOpcode0x05(); // RLC L
    short extendedOpcode0x06(); // RLC (HL)
    short extendedOpcode0x07(); // RLC A
    
    short extendedOpcode0x08(); // RRC B
    short extendedOpcode0x09(); // RRC C
    short extendedOpcode0x0A(); // RRC D
    short extendedOpcode0x0B(); // RRC E
    short extendedOpcode0x0C(); // RRC H
    short extendedOpcode0x0D(); // RRC L
    short extendedOpcode0x0E(); // RRC (HL)
    short extendedOpcode0x0F(); // RRC A
    
    short extendedOpcode0x10(); // rl B
    short extendedOpcode0x11(); // rl C
    short extendedOpcode0x12(); // rl D
    short extendedOpcode0x13(); // rl E
    short extendedOpcode0x14(); // rl H
    short extendedOpcode0x15(); // rl L
    short extendedOpcode0x16(); // rl (HL)
    short extendedOpcode0x17(); // rl A
    
    short extendedOpcode0x18(); // rr B
    short extendedOpcode0x19(); // rr C
    short extendedOpcode0x1A(); // rr D
    short extendedOpcode0x1B(); // rr E
    short extendedOpcode0x1C(); // rr H
    short extendedOpcode0x1D(); // rr L
    short extendedOpcode0x1E(); // rr (HL)
    short extendedOpcode0x1F(); // rr A
    
    short extendedOpcode0x20(); // sla B
    short extendedOpcode0x21(); // sla C
    short extendedOpcode0x22(); // sla D
    short extendedOpcode0x23(); // sla E
    short extendedOpcode0x24(); // sla H
    short extendedOpcode0x25(); // sla L
    short extendedOpcode0x26(); // sla (HL)
    short extendedOpcode0x27(); // sla A
    
    short extendedOpcode0x28(); // sra B
    short extendedOpcode0x29(); // sra C
    short extendedOpcode0x2A(); // sra D
    short extendedOpcode0x2B(); // sra E
    short extendedOpcode0x2C(); // sra H
    short extendedOpcode0x2D(); // sra L
    short extendedOpcode0x2E(); // sra (HL)
    short extendedOpcode0x2F(); // sra A
    
    short extendedOpcode0x30(); // swap B
    short extendedOpcode0x31(); // swap C
    short extendedOpcode0x32(); // swap D
    short extendedOpcode0x33(); // swap E
    short extendedOpcode0x34(); // swap H
    short extendedOpcode0x35(); // swap L
    short extendedOpcode0x36(); // swap (HL)
    short extendedOpcode0x37(); // swap A
    
    short extendedOpcode0x38(); // SRL B
    short extendedOpcode0x39(); // SRL C
    short extendedOpcode0x3A(); // SRL D
    short extendedOpcode0x3B(); // SRL E
    short extendedOpcode0x3C(); // SRL H
    short extendedOpcode0x3D(); // SRL L
    short extendedOpcode0x3E(); // SRL (HL)
    short extendedOpcode0x3F(); // SRL A
    
    // Singlebit operations
    void bit(const byte, const short);
    void set(byte *,const short);
    void res(byte *, const short);
    
    short extendedOpcode0x40(); // bit 0, B
    short extendedOpcode0x41(); // bit 0, C
    short extendedOpcode0x42(); // bit 0, D
    short extendedOpcode0x43(); // bit 0, E
    short extendedOpcode0x44(); // bit 0, H
    short extendedOpcode0x45(); // bit 0, L
    short extendedOpcode0x46(); // bit 0, (HL)
    short extendedOpcode0x47(); // bit 0, A
    
    short extendedOpcode0x48(); // bit 1, B
    short extendedOpcode0x49(); // bit 1, C
    short extendedOpcode0x4A(); // bit 1, D
    short extendedOpcode0x4B(); // bit 1, E
    short extendedOpcode0x4C(); // bit 1, H
    short extendedOpcode0x4D(); // bit 1, L
    short extendedOpcode0x4E(); // bit 1, (HL)
    short extendedOpcode0x4F(); // bit 1, A
    
    short extendedOpcode0x50(); // bit 2, B
    short extendedOpcode0x51(); // bit 2, C
    short extendedOpcode0x52(); // bit 2, D
    short extendedOpcode0x53(); // bit 2, E
    short extendedOpcode0x54(); // bit 2, H
    short extendedOpcode0x55(); // bit 2, L
    short extendedOpcode0x56(); // bit 2, (HL)
    short extendedOpcode0x57(); // bit 2, A
    
    short extendedOpcode0x58(); // bit 3, B
    short extendedOpcode0x59(); // bit 3, C
    short extendedOpcode0x5A(); // bit 3, D
    short extendedOpcode0x5B(); // bit 3, E
    short extendedOpcode0x5C(); // bit 3, H
    short extendedOpcode0x5D(); // bit 3, L
    short extendedOpcode0x5E(); // bit 3, (HL)
    short extendedOpcode0x5F(); // bit 3, A
    
    short extendedOpcode0x60(); // bit 4, B
    short extendedOpcode0x61(); // bit 4, C
    short extendedOpcode0x62(); // bit 4, D
    short extendedOpcode0x63(); // bit 4, E
    short extendedOpcode0x64(); // bit 4, H
    short extendedOpcode0x65(); // bit 4, L
    short extendedOpcode0x66(); // bit 4, (HL)
    short extendedOpcode0x67(); // bit 4, A
    
    short extendedOpcode0x68(); // bit 5, B
    short extendedOpcode0x69(); // bit 5, C
    short extendedOpcode0x6A(); // bit 5, D
    short extendedOpcode0x6B(); // bit 5, E
    short extendedOpcode0x6C(); // bit 5, H
    short extendedOpcode0x6D(); // bit 5, L
    short extendedOpcode0x6E(); // bit 5, (HL)
    short extendedOpcode0x6F(); // bit 5, A
    
    short extendedOpcode0x70(); // bit 6, B
    short extendedOpcode0x71(); // bit 6, C
    short extendedOpcode0x72(); // bit 6, D
    short extendedOpcode0x73(); // bit 6, E
    short extendedOpcode0x74(); // bit 6, H
    short extendedOpcode0x75(); // bit 6, L
    short extendedOpcode0x76(); // bit 6, (HL)
    short extendedOpcode0x77(); // bit 6, A
    
    short extendedOpcode0x78(); // bit 7, B
    short extendedOpcode0x79(); // bit 7, C
    short extendedOpcode0x7A(); // bit 7, D
    short extendedOpcode0x7B(); // bit 7, E
    short extendedOpcode0x7C(); // bit 7, H
    short extendedOpcode0x7D(); // bit 7, L
    short extendedOpcode0x7E(); // bit 7, (HL)
    short extendedOpcode0x7F(); // bit 7, A
    
    short extendedOpcode0x80(); // res 0, B
    short extendedOpcode0x81(); // res 0, C
    short extendedOpcode0x82(); // res 0, D
    short extendedOpcode0x83(); // res 0, E
    short extendedOpcode0x84(); // res 0, H
    short extendedOpcode0x85(); // res 0, L
    short extendedOpcode0x86(); // res 0, (HL)
    short extendedOpcode0x87(); // res 0, A
    
    short extendedOpcode0x88(); // res 1, B
    short extendedOpcode0x89(); // res 1, C
    short extendedOpcode0x8A(); // res 1, D
    short extendedOpcode0x8B(); // res 1, E
    short extendedOpcode0x8C(); // res 1, H
    short extendedOpcode0x8D(); // res 1, L
    short extendedOpcode0x8E(); // res 1, (HL)
    short extendedOpcode0x8F(); // res 1, A

    short extendedOpcode0x90(); // res 2, B
    short extendedOpcode0x91(); // res 2, C
    short extendedOpcode0x92(); // res 2, D
    short extendedOpcode0x93(); // res 2, E
    short extendedOpcode0x94(); // res 2, H
    short extendedOpcode0x95(); // res 2, L
    short extendedOpcode0x96(); // res 2, (HL)
    short extendedOpcode0x97(); // res 2, A
    
    short extendedOpcode0x98(); // res 3, B
    short extendedOpcode0x99(); // res 3, C
    short extendedOpcode0x9A(); // res 3, D
    short extendedOpcode0x9B(); // res 3, E
    short extendedOpcode0x9C(); // res 3, H
    short extendedOpcode0x9D(); // res 3, L
    short extendedOpcode0x9E(); // res 3, (HL)
    short extendedOpcode0x9F(); // res 3, A
    
    short extendedOpcode0xA0(); // res 4, B
    short extendedOpcode0xA1(); // res 4, C
    short extendedOpcode0xA2(); // res 4, D
    short extendedOpcode0xA3(); // res 4, E
    short extendedOpcode0xA4(); // res 4, H
    short extendedOpcode0xA5(); // res 4, L
    short extendedOpcode0xA6(); // res 4, (HL)
    short extendedOpcode0xA7(); // res 4, A
    
    short extendedOpcode0xA8(); // res 5, B
    short extendedOpcode0xA9(); // res 5, C
    short extendedOpcode0xAA(); // res 5, D
    short extendedOpcode0xAB(); // res 5, E
    short extendedOpcode0xAC(); // res 5, H
    short extendedOpcode0xAD(); // res 5, L
    short extendedOpcode0xAE(); // res 5, (HL)
    short extendedOpcode0xAF(); // res 5, A
    
    short extendedOpcode0xB0(); // res 6, B
    short extendedOpcode0xB1(); // res 6, C
    short extendedOpcode0xB2(); // res 6, D
    short extendedOpcode0xB3(); // res 6, E
    short extendedOpcode0xB4(); // res 6, H
    short extendedOpcode0xB5(); // res 6, L
    short extendedOpcode0xB6(); // res 6, (HL)
    short extendedOpcode0xB7(); // res 6, A
    
    short extendedOpcode0xB8(); // res 7, B
    short extendedOpcode0xB9(); // res 7, C
    short extendedOpcode0xBA(); // res 7, D
    short extendedOpcode0xBB(); // res 7, E
    short extendedOpcode0xBC(); // res 7, H
    short extendedOpcode0xBD(); // res 7, L
    short extendedOpcode0xBE(); // res 7, (HL)
    short extendedOpcode0xBF(); // res 7, A
    
    short extendedOpcode0xC0(); // set 0, B
    short extendedOpcode0xC1(); // set 0, C
    short extendedOpcode0xC2(); // set 0, D
    short extendedOpcode0xC3(); // set 0, E
    short extendedOpcode0xC4(); // set 0, H
    short extendedOpcode0xC5(); // set 0, L
    short extendedOpcode0xC6(); // set 0, (HL)
    short extendedOpcode0xC7(); // set 0, A
    
    short extendedOpcode0xC8(); // set 1, B
    short extendedOpcode0xC9(); // set 1, C
    short extendedOpcode0xCA(); // set 1, D
    short extendedOpcode0xCB(); // set 1, E
    short extendedOpcode0xCC(); // set 1, H
    short extendedOpcode0xCD(); // set 1, L
    short extendedOpcode0xCE(); // set 1, (HL)
    short extendedOpcode0xCF(); // set 1, A
    
    short extendedOpcode0xD0(); // set 2, B
    short extendedOpcode0xD1(); // set 2, C
    short extendedOpcode0xD2(); // set 2, D
    short extendedOpcode0xD3(); // set 2, E
    short extendedOpcode0xD4(); // set 2, H
    short extendedOpcode0xD5(); // set 2, L
    short extendedOpcode0xD6(); // set 2, (HL)
    short extendedOpcode0xD7(); // set 2, A
    
    short extendedOpcode0xD8(); // set 3, B
    short extendedOpcode0xD9(); // set 3, C
    short extendedOpcode0xDA(); // set 3, D
    short extendedOpcode0xDB(); // set 3, E
    short extendedOpcode0xDC(); // set 3, H
    short extendedOpcode0xDD(); // set 3, L
    short extendedOpcode0xDE(); // set 3, (HL)
    short extendedOpcode0xDF(); // set 3, A
    
    short extendedOpcode0xE0(); // set 4, B
    short extendedOpcode0xE1(); // set 4, C
    short extendedOpcode0xE2(); // set 4, D
    short extendedOpcode0xE3(); // set 4, E
    short extendedOpcode0xE4(); // set 4, H
    short extendedOpcode0xE5(); // set 4, L
    short extendedOpcode0xE6(); // set 4, (HL)
    short extendedOpcode0xE7(); // set 4, A
    
    short extendedOpcode0xE8(); // set 5, B
    short extendedOpcode0xE9(); // set 5, C
    short extendedOpcode0xEA(); // set 5, D
    short extendedOpcode0xEB(); // set 5, E
    short extendedOpcode0xEC(); // set 5, H
    short extendedOpcode0xED(); // set 5, L
    short extendedOpcode0xEE(); // set 5, (HL)
    short extendedOpcode0xEF(); // set 5, A
    
    short extendedOpcode0xF0(); // set 6, B
    short extendedOpcode0xF1(); // set 6, C
    short extendedOpcode0xF2(); // set 6, D
    short extendedOpcode0xF3(); // set 6, E
    short extendedOpcode0xF4(); // set 6, H
    short extendedOpcode0xF5(); // set 6, L
    short extendedOpcode0xF6(); // set 6, (HL)
    short extendedOpcode0xF7(); // set 6, A
    
    short extendedOpcode0xF8(); // set 7, B
    short extendedOpcode0xF9(); // set 7, C
    short extendedOpcode0xFA(); // set 7, D
    short extendedOpcode0xFB(); // set 7, E
    short extendedOpcode0xFC(); // set 7, H
    short extendedOpcode0xFD(); // set 7, L
    short extendedOpcode0xFE(); // set 7, (HL)
    short extendedOpcode0xFF(); // set 7, A
};



#endif