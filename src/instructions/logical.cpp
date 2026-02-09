#include "cpu.h"

#define CYCL cpu->cycl();

void SetNZ(CPU * cpu, Byte reg);

// AND - Logical AND with Accumulator
void ANDI(CPU * cpu) {
    CYCL
    cpu->A &= cpu->mem->read(cpu->immediate());
    SetNZ(cpu, cpu->A);
}

void ANDZP(CPU * cpu) {
    CYCL
    cpu->A &= cpu->mem->read(cpu->zeroPage());
    SetNZ(cpu, cpu->A);
}

void ANDZPX(CPU * cpu) {
    CYCL
    cpu->A &= cpu->mem->read(cpu->zeroPageX());
    SetNZ(cpu, cpu->A);
}

void ANDA(CPU * cpu) {
    CYCL
    cpu->A &= cpu->mem->read(cpu->absolute());
    SetNZ(cpu, cpu->A);
}

void ANDAX(CPU * cpu) {
    CYCL
    cpu->A &= cpu->mem->read(cpu->absoluteX());
    SetNZ(cpu, cpu->A);
}

void ANDAY(CPU * cpu) {
    CYCL
    cpu->A &= cpu->mem->read(cpu->absoluteY());
    SetNZ(cpu, cpu->A);
}

void ANDIX(CPU * cpu) {
    CYCL
    cpu->A &= cpu->mem->read(cpu->indirectX());
    SetNZ(cpu, cpu->A);
}

void ANDIY(CPU * cpu) {
    CYCL
    cpu->A &= cpu->mem->read(cpu->indirectY());
    SetNZ(cpu, cpu->A);
}

// ORA - Logical OR with Accumulator
void ORAI(CPU * cpu) {
    CYCL
    cpu->A |= cpu->mem->read(cpu->immediate());
    SetNZ(cpu, cpu->A);
}

void ORAZP(CPU * cpu) {
    CYCL
    cpu->A |= cpu->mem->read(cpu->zeroPage());
    SetNZ(cpu, cpu->A);
}

void ORAZPX(CPU * cpu) {
    CYCL
    cpu->A |= cpu->mem->read(cpu->zeroPageX());
    SetNZ(cpu, cpu->A);
}

void ORAA(CPU * cpu) {
    CYCL
    cpu->A |= cpu->mem->read(cpu->absolute());
    SetNZ(cpu, cpu->A);
}

void ORAAX(CPU * cpu) {
    CYCL
    cpu->A |= cpu->mem->read(cpu->absoluteX());
    SetNZ(cpu, cpu->A);
}

void ORAAY(CPU * cpu) {
    CYCL
    cpu->A |= cpu->mem->read(cpu->absoluteY());
    SetNZ(cpu, cpu->A);
}

void ORAIX(CPU * cpu) {
    CYCL
    cpu->A |= cpu->mem->read(cpu->indirectX());
    SetNZ(cpu, cpu->A);
}

void ORAIY(CPU * cpu) {
    CYCL
    cpu->A |= cpu->mem->read(cpu->indirectY());
    SetNZ(cpu, cpu->A);
}

// EOR - Logical Exclusive OR with Accumulator
void EORI(CPU * cpu) {
    CYCL
    cpu->A ^= cpu->mem->read(cpu->immediate());
    SetNZ(cpu, cpu->A);
}

void EORZP(CPU * cpu) {
    CYCL
    cpu->A ^= cpu->mem->read(cpu->zeroPage());
    SetNZ(cpu, cpu->A);
}

void EORZPX(CPU * cpu) {
    CYCL
    cpu->A ^= cpu->mem->read(cpu->zeroPageX());
    SetNZ(cpu, cpu->A);
}

void EORA(CPU * cpu) {
    CYCL
    cpu->A ^= cpu->mem->read(cpu->absolute());
    SetNZ(cpu, cpu->A);
}

void EORAX(CPU * cpu) {
    CYCL
    cpu->A ^= cpu->mem->read(cpu->absoluteX());
    SetNZ(cpu, cpu->A);
}

void EORAY(CPU * cpu) {
    CYCL
    cpu->A ^= cpu->mem->read(cpu->absoluteY());
    SetNZ(cpu, cpu->A);
}

void EORIX(CPU * cpu) {
    CYCL
    cpu->A ^= cpu->mem->read(cpu->indirectX());
    SetNZ(cpu, cpu->A);
}

void EORIY(CPU * cpu) {
    CYCL
    cpu->A ^= cpu->mem->read(cpu->indirectY());
    SetNZ(cpu, cpu->A);
}
