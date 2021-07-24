#include "cpu.h"

#define CYCL cpu->cycl();

void LDAFlags(CPU * cpu) {
    cpu->Z = cpu->A == 0;
    cpu->N = (cpu->A & 0b10000000) > 0;
}

void LDAI(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->immediate());
    LDAFlags(cpu);
}

void LDAZ(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->zeroPage());
    LDAFlags(cpu);
}

void LDAZX(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->zeroPageX());
    LDAFlags(cpu);
}

void LDAA(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->absolute());
    LDAFlags(cpu);
}

void LDAAX(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->absoluteX());
    LDAFlags(cpu);
}

void LDAAY(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->absoluteY());
    LDAFlags(cpu);
}

void LDAIX(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->indirectX());
    LDAFlags(cpu);
}

void LDAIY(CPU * cpu) {
    CYCL
    cpu->A = cpu->mem->read(cpu->indirectY());
    LDAFlags(cpu);
}

void LDXFlags(CPU * cpu) {
    cpu->Z = cpu->X == 0;
    cpu->N = (cpu->X & 0b10000000) > 0;
}

void LDXI(CPU * cpu) {
    CYCL
    cpu->X = cpu->mem->read(cpu->immediate());
    LDXFlags(cpu);
}

void LDXZP(CPU * cpu) {
    CYCL
    cpu->X = cpu->mem->read(cpu->zeroPage());
    LDXFlags(cpu);
}

void LDXZPY(CPU * cpu) {
    CYCL
    cpu->X = cpu->mem->read(cpu->zeroPageY());
    LDXFlags(cpu);
}

void LDXA(CPU * cpu) {
    CYCL
    cpu->X = cpu->mem->read(cpu->absolute());
    LDXFlags(cpu);
}

void LDXAY(CPU * cpu) {
    CYCL
    cpu->X = cpu->absoluteY();
    LDXFlags(cpu);
}

void LDYFlags(CPU * cpu) {
    cpu->Z = cpu->Y == 0;
    cpu->N = (cpu->Y & 0b10000000) > 0;
}

void LDYI(CPU * cpu) {
    CYCL
    cpu->Y = cpu->mem->read(cpu->immediate());
    LDXFlags(cpu);
}

void LDYZP(CPU * cpu) {
    CYCL
    cpu->Y = cpu->mem->read(cpu->zeroPage());
    LDXFlags(cpu);
}

void LDYZPY(CPU * cpu) {
    CYCL
    cpu->Y = cpu->mem->read(cpu->zeroPageX());
    LDXFlags(cpu);
}

void LDYA(CPU * cpu) {
    CYCL
    cpu->Y = cpu->mem->read(cpu->absolute());
    LDXFlags(cpu);
}

void LDYAY(CPU * cpu) {
    CYCL
    cpu->Y = cpu->mem->read(cpu->absoluteX());
    LDXFlags(cpu);
}
