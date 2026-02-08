#include "cpu.h"

#define CYCL cpu->cycl();

void SetNZ(CPU * cpu, Byte reg);

// ASL - Arithmetic Shift Left
void ASLA(CPU * cpu) {
    CYCL
    cpu->setC((cpu->A & 0x80) > 0);
    cpu->A <<= 1;
    SetNZ(cpu, cpu->A);
}

void ASLZP(CPU * cpu) {
    Byte addr = cpu->zeroPage();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    cpu->setC((value & 0x80) > 0);
    value <<= 1;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void ASLZPX(CPU * cpu) {
    Byte addr = cpu->zeroPageX();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    cpu->setC((value & 0x80) > 0);
    value <<= 1;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void ASLABS(CPU * cpu) {
    Word addr = cpu->absolute();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    cpu->setC((value & 0x80) > 0);
    value <<= 1;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void ASLABSX(CPU * cpu) {
    Word addr = cpu->absoluteX();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    cpu->setC((value & 0x80) > 0);
    value <<= 1;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

// LSR - Logical Shift Right
void LSRA(CPU * cpu) {
    CYCL
    cpu->setC((cpu->A & 0x01) > 0);
    cpu->A >>= 1;
    SetNZ(cpu, cpu->A);
}

void LSRZP(CPU * cpu) {
    Byte addr = cpu->zeroPage();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    cpu->setC((value & 0x01) > 0);
    value >>= 1;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void LSRZPX(CPU * cpu) {
    Byte addr = cpu->zeroPageX();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    cpu->setC((value & 0x01) > 0);
    value >>= 1;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void LSRABS(CPU * cpu) {
    Word addr = cpu->absolute();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    cpu->setC((value & 0x01) > 0);
    value >>= 1;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void LSRABSX(CPU * cpu) {
    Word addr = cpu->absoluteX();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    cpu->setC((value & 0x01) > 0);
    value >>= 1;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

// ROL - Rotate Left
void ROLA(CPU * cpu) {
    CYCL
    bool oldCarry = cpu->C();
    cpu->setC((cpu->A & 0x80) > 0);
    cpu->A = (cpu->A << 1) | (oldCarry ? 1 : 0);
    SetNZ(cpu, cpu->A);
}

void ROLZP(CPU * cpu) {
    Byte addr = cpu->zeroPage();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    bool oldCarry = cpu->C();
    cpu->setC((value & 0x80) > 0);
    value = (value << 1) | (oldCarry ? 1 : 0);
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void ROLZPX(CPU * cpu) {
    Byte addr = cpu->zeroPageX();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    bool oldCarry = cpu->C();
    cpu->setC((value & 0x80) > 0);
    value = (value << 1) | (oldCarry ? 1 : 0);
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void ROLABS(CPU * cpu) {
    Word addr = cpu->absolute();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    bool oldCarry = cpu->C();
    cpu->setC((value & 0x80) > 0);
    value = (value << 1) | (oldCarry ? 1 : 0);
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void ROLABSX(CPU * cpu) {
    Word addr = cpu->absoluteX();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    bool oldCarry = cpu->C();
    cpu->setC((value & 0x80) > 0);
    value = (value << 1) | (oldCarry ? 1 : 0);
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

// ROR - Rotate Right
void RORA(CPU * cpu) {
    CYCL
    bool oldCarry = cpu->C();
    cpu->setC((cpu->A & 0x01) > 0);
    cpu->A = (cpu->A >> 1) | (oldCarry ? 0x80 : 0);
    SetNZ(cpu, cpu->A);
}

void RORZP(CPU * cpu) {
    Byte addr = cpu->zeroPage();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    bool oldCarry = cpu->C();
    cpu->setC((value & 0x01) > 0);
    value = (value >> 1) | (oldCarry ? 0x80 : 0);
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void RORZPX(CPU * cpu) {
    Byte addr = cpu->zeroPageX();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    bool oldCarry = cpu->C();
    cpu->setC((value & 0x01) > 0);
    value = (value >> 1) | (oldCarry ? 0x80 : 0);
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void RORABS(CPU * cpu) {
    Word addr = cpu->absolute();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    bool oldCarry = cpu->C();
    cpu->setC((value & 0x01) > 0);
    value = (value >> 1) | (oldCarry ? 0x80 : 0);
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void RORABSX(CPU * cpu) {
    Word addr = cpu->absoluteX();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    bool oldCarry = cpu->C();
    cpu->setC((value & 0x01) > 0);
    value = (value >> 1) | (oldCarry ? 0x80 : 0);
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}
