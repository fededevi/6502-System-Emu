#include "cpu.h"

#define CYCL cpu->cycl();

void LDFlags(CPU * cpu, Byte reg) {
    cpu->setZ( reg == 0);
    cpu->setN((reg & 0b10000000) > 0);
}

void LDAI(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->immediate());
    LDFlags(cpu, cpu->A);
}

void LDAZ(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->zeroPage());
    LDFlags(cpu, cpu->A);
}

void LDAZX(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->zeroPageX());
    LDFlags(cpu, cpu->A);
}

void LDAA(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->absolute());
    LDFlags(cpu, cpu->A);
}

void LDAAX(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->absoluteX());
    LDFlags(cpu, cpu->A);
}

void LDAAY(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->absoluteY());
    LDFlags(cpu, cpu->A);
}

void LDAIX(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->indirectX());
    LDFlags(cpu, cpu->A);
}

void LDAIY(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->indirectY());
    LDFlags(cpu, cpu->A);
}

void LDXI(CPU * cpu) {
    CYCL
    cpu->X = cpu->mem->read(cpu->immediate());
    LDFlags(cpu, cpu->X);
}

void LDXZP(CPU * cpu) {
    CYCL
    cpu->X = cpu->mem->read(cpu->zeroPage());
    LDFlags(cpu, cpu->X);
}

void LDXZPY(CPU * cpu) {
    CYCL
    cpu->X = cpu->mem->read(cpu->zeroPageY());
    LDFlags(cpu, cpu->X);
}

void LDXA(CPU * cpu) {
    CYCL
    cpu->X = cpu->mem->read(cpu->absolute());
    LDFlags(cpu, cpu->X);
}

void LDXAY(CPU * cpu) {
    CYCL
    cpu->X = cpu->mem->read(cpu->absoluteY());
    LDFlags(cpu, cpu->X);
}

void LDYI(CPU * cpu) {
    CYCL
    cpu->Y = cpu->mem->read(cpu->immediate());
    LDFlags(cpu, cpu->Y);
}

void LDYZP(CPU * cpu) {
    CYCL
    cpu->Y = cpu->mem->read(cpu->zeroPage());
    LDFlags(cpu, cpu->Y);
}

void LDYZPY(CPU * cpu) {
    CYCL
    cpu->Y = cpu->mem->read(cpu->zeroPageX());
    LDFlags(cpu, cpu->Y);
}

void LDYA(CPU * cpu) {
    CYCL
    cpu->Y = cpu->mem->read(cpu->absolute());
    LDFlags(cpu, cpu->Y);
}

void LDYAY(CPU * cpu) {
    CYCL
    cpu->Y = cpu->mem->read(cpu->absoluteX());
    LDFlags(cpu, cpu->Y);
}
