#include "cpu.h"

#define CYCL cpu->cycl();

// NOP - No Operation (0xEA)
void NOP(CPU * cpu) {
    CYCL
}

// JMP - Jump (0x4C absolute, 0x6C indirect)
void JMPABS(CPU * cpu) {
    CYCL
    cpu->PC = cpu->mem->read16(cpu->PC);
    CYCL
}

void JMPIND(CPU * cpu) {
    CYCL
    Word addr = cpu->mem->read16(cpu->PC);
    CYCL
    CYCL
    cpu->PC = cpu->mem->read16(addr);
    CYCL
}

// JSR - Jump to Subroutine (0x20)
void JSR(CPU * cpu) {
    CYCL
    Word addr = cpu->mem->read16(cpu->PC);
    CYCL
    cpu->PC++;  // PC now points to next instruction - 1
    CYCL
    cpu->push(cpu->PC >> 8);
    CYCL
    cpu->push(cpu->PC & 0xFF);
    CYCL
    cpu->PC = addr;
}

// RTS - Return from Subroutine (0x60)
void RTS(CPU * cpu) {
    CYCL
    CYCL
    cpu->SP++;
    Byte lo = cpu->mem->read(0x100 + cpu->SP);
    CYCL
    cpu->SP++;
    Byte hi = cpu->mem->read(0x100 + cpu->SP);
    CYCL
    cpu->PC = (hi << 8) | lo;
    CYCL
    cpu->PC++;
}

// RTI - Return from Interrupt (0x40)
void RTI(CPU * cpu) {
    CYCL
    CYCL
    cpu->SP++;
    cpu->P = cpu->mem->read(0x100 + cpu->SP);
    CYCL
    cpu->SP++;
    Byte lo = cpu->mem->read(0x100 + cpu->SP);
    CYCL
    cpu->SP++;
    Byte hi = cpu->mem->read(0x100 + cpu->SP);
    CYCL
    cpu->PC = (hi << 8) | lo;
    CYCL
}

// BIT - Test Bits (0x24 zeropage, 0x2C absolute)
void BITZP(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->zeroPage());
    Byte result = cpu->A & value;
    cpu->setZ(result == 0);
    cpu->setN((value & 0x80) > 0);
    cpu->setV((value & 0x40) > 0);
}

void BITABS(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->absolute());
    Byte result = cpu->A & value;
    cpu->setZ(result == 0);
    cpu->setN((value & 0x80) > 0);
    cpu->setV((value & 0x40) > 0);
}
