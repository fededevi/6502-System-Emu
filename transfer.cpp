#include "cpu.h"

#define CYCL cpu->cycl();

// Forward declaration
void SetNZ(CPU * cpu, Byte reg);

// TAX - Transfer Accumulator to X (0xAA)
void TAX(CPU * cpu) {
    CYCL
    cpu->X = cpu->A;
    SetNZ(cpu, cpu->X);
}

// TAY - Transfer Accumulator to Y (0xA8)
void TAY(CPU * cpu) {
    CYCL
    cpu->Y = cpu->A;
    SetNZ(cpu, cpu->Y);
}

// TXA - Transfer X to Accumulator (0x8A)
void TXA(CPU * cpu) {
    CYCL
    cpu->A = cpu->X;
    SetNZ(cpu, cpu->A);
}

// TYA - Transfer Y to Accumulator (0x98)
void TYA(CPU * cpu) {
    CYCL
    cpu->A = cpu->Y;
    SetNZ(cpu, cpu->A);
}

// TSX - Transfer Stack Pointer to X (0xBA)
void TSX(CPU * cpu) {
    CYCL
    cpu->X = cpu->SP;
    SetNZ(cpu, cpu->X);
}

// TXS - Transfer X to Stack Pointer (0x9A)
void TXS(CPU * cpu) {
    CYCL
    cpu->SP = cpu->X;
    // Note: TXS does not affect any flags (unlike other transfer instructions)
}
