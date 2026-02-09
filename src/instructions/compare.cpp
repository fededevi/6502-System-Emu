#include "cpu.h"

#define CYCL cpu->cycl();

void CompareFlags(CPU * cpu, Byte reg, Byte value) {
    Byte result = reg - value;
    cpu->setZ(reg == value);
    cpu->setN((result & 0x80) > 0);
    cpu->setC(reg >= value);
}

// CMP - Compare Accumulator
void CMPI(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->immediate());
    CompareFlags(cpu, cpu->A, value);
}

void CMPZP(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->zeroPage());
    CompareFlags(cpu, cpu->A, value);
}

void CMPZPX(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->zeroPageX());
    CompareFlags(cpu, cpu->A, value);
}

void CMPA(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->absolute());
    CompareFlags(cpu, cpu->A, value);
}

void CMPAX(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->absoluteX());
    CompareFlags(cpu, cpu->A, value);
}

void CMPAY(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->absoluteY());
    CompareFlags(cpu, cpu->A, value);
}

void CMPIX(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->indirectX());
    CompareFlags(cpu, cpu->A, value);
}

void CMPIY(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->indirectY());
    CompareFlags(cpu, cpu->A, value);
}

// CPX - Compare X Register
void CPXI(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->immediate());
    CompareFlags(cpu, cpu->X, value);
}

void CPXZP(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->zeroPage());
    CompareFlags(cpu, cpu->X, value);
}

void CPXA(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->absolute());
    CompareFlags(cpu, cpu->X, value);
}

// CPY - Compare Y Register
void CPYI(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->immediate());
    CompareFlags(cpu, cpu->Y, value);
}

void CPYZP(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->zeroPage());
    CompareFlags(cpu, cpu->Y, value);
}

void CPYA(CPU * cpu) {
    CYCL
    Byte value = cpu->mem->read(cpu->absolute());
    CompareFlags(cpu, cpu->Y, value);
}
