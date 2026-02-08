#include "cpu.h"

#define CYCL cpu->cycl();

void SetNZ(CPU * cpu, Byte reg);

// INX - Increment X Register (0xE8)
void INX(CPU * cpu) {
    CYCL
    cpu->X++;
    SetNZ(cpu, cpu->X);
}

// INY - Increment Y Register (0xC8)
void INY(CPU * cpu) {
    CYCL
    cpu->Y++;
    SetNZ(cpu, cpu->Y);
}

// DEX - Decrement X Register (0xCA)
void DEX(CPU * cpu) {
    CYCL
    cpu->X--;
    SetNZ(cpu, cpu->X);
}

// DEY - Decrement Y Register (0x88)
void DEY(CPU * cpu) {
    CYCL
    cpu->Y--;
    SetNZ(cpu, cpu->Y);
}

// INC - Increment Memory
void INCZP(CPU * cpu) {
    Byte addr = cpu->zeroPage();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    value++;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void INCZPX(CPU * cpu) {
    Byte addr = cpu->zeroPageX();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    value++;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void INCA(CPU * cpu) {
    Word addr = cpu->absolute();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    value++;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void INCAX(CPU * cpu) {
    Word addr = cpu->absoluteX();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    value++;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

// DEC - Decrement Memory
void DECZP(CPU * cpu) {
    Byte addr = cpu->zeroPage();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    value--;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void DECZPX(CPU * cpu) {
    Byte addr = cpu->zeroPageX();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    value--;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void DECA(CPU * cpu) {
    Word addr = cpu->absolute();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    value--;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}

void DECAX(CPU * cpu) {
    Word addr = cpu->absoluteX();
    CYCL
    Byte value = cpu->mem->read(addr);
    CYCL
    value--;
    cpu->mem->write(addr, value);
    CYCL
    SetNZ(cpu, value);
}
