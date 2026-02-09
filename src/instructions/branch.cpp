#include "cpu.h"

#define CYCL cpu->cycl();

void Branch(CPU * cpu, bool condition) {
    CYCL
    signed char offset = cpu->mem->read(cpu->PC++);
    if (condition) {
        CYCL
        Word newPC = cpu->PC + offset;
        if ((newPC & 0xFF00) != (cpu->PC & 0xFF00)) {
            CYCL  // Page boundary crossed
        }
        cpu->PC = newPC;
    }
}

// BCC - Branch if Carry Clear (0x90)
void BCC(CPU * cpu) {
    Branch(cpu, !cpu->C());
}

// BCS - Branch if Carry Set (0xB0)
void BCS(CPU * cpu) {
    Branch(cpu, cpu->C());
}

// BEQ - Branch if Equal (Zero Set) (0xF0)
void BEQ(CPU * cpu) {
    Branch(cpu, cpu->Z());
}

// BNE - Branch if Not Equal (Zero Clear) (0xD0)
void BNE(CPU * cpu) {
    Branch(cpu, !cpu->Z());
}

// BMI - Branch if Minus (Negative Set) (0x30)
void BMI(CPU * cpu) {
    Branch(cpu, cpu->N());
}

// BPL - Branch if Plus (Negative Clear) (0x10)
void BPL(CPU * cpu) {
    Branch(cpu, !cpu->N());
}

// BVC - Branch if Overflow Clear (0x50)
void BVC(CPU * cpu) {
    Branch(cpu, !cpu->V());
}

// BVS - Branch if Overflow Set (0x70)
void BVS(CPU * cpu) {
    Branch(cpu, cpu->V());
}
