#include "cpu.h"

void LDAFlags(CPU * cpu) {
    cpu->Z = cpu->A == 0;
    cpu->N = (cpu->A & 0b10000000) > 0;
}

void LDAI(CPU * cpu) {
    cpu->A = cpu->immediate();
    LDAFlags(cpu);
}

void LDAZ(CPU * cpu) {
    cpu->A = cpu->zeroPage();
    LDAFlags(cpu);
}

void LDAZX(CPU * cpu) {
    cpu->A = cpu->zeroPageX();
    LDAFlags(cpu);
}

void LDAA(CPU * cpu) {
    cpu->A = cpu->absolute();
    LDAFlags(cpu);
}

void LDAAX(CPU * cpu) {
    cpu->A = cpu->absoluteX();
    LDAFlags(cpu);
}

void LDAAY(CPU * cpu) {
    cpu->A = cpu->absoluteY();
    LDAFlags(cpu);
}

void LDAIX(CPU * cpu) {
    cpu->A = cpu->indirectX();
    LDAFlags(cpu);
}

void LDAIY(CPU * cpu) {
    cpu->A = cpu->indirectY();
    LDAFlags(cpu);
}

void LDXFlags(CPU * cpu) {
    cpu->Z = cpu->X == 0;
    cpu->N = (cpu->X & 0b10000000) > 0;
}

void LDXI(CPU * cpu) {
    cpu->X = cpu->immediate();
    LDXFlags(cpu);
}

void LDXZP(CPU * cpu) {
    cpu->X = cpu->zeroPage();
    LDXFlags(cpu);
}

void LDXZPY(CPU * cpu) {
    cpu->X = cpu->zeroPageY();
    LDXFlags(cpu);
}

void LDXA(CPU * cpu) {
    cpu->X = cpu->absolute();
    LDXFlags(cpu);
}

void LDXAY(CPU * cpu) {
    cpu->X = cpu->absoluteY();
    LDXFlags(cpu);
}

void LDYFlags(CPU * cpu) {
    cpu->Z = cpu->Y == 0;
    cpu->N = (cpu->Y & 0b10000000) > 0;
}

void LDYI(CPU * cpu) {
    cpu->Y = cpu->immediate();
    LDXFlags(cpu);
}

void LDYZP(CPU * cpu) {
    cpu->Y = cpu->zeroPage();
    LDXFlags(cpu);
}

void LDYZPY(CPU * cpu) {
    cpu->Y = cpu->zeroPageX();
    LDXFlags(cpu);
}

void LDYA(CPU * cpu) {
    cpu->Y = cpu->absolute();
    LDXFlags(cpu);
}

void LDYAY(CPU * cpu) {
    cpu->Y = cpu->absoluteX();
    LDXFlags(cpu);
}
