#include "cpu.h"

#define CYCL cpu->cycl();

void STAZ(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->zeroPage(), cpu->A);
}

void STAZX(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->zeroPageX(), cpu->A);
}

void STAA(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->absolute(), cpu->A);
}

void STAAX(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->absoluteX(), cpu->A);
}

void STAAY(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->absoluteY(), cpu->A);
}

void STAIX(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->indirectX(), cpu->A);
}

void STAIY(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->indirectY(), cpu->A);
}

void STXZP(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->zeroPage(), cpu->X);
}

void STXZPY(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->zeroPageY(), cpu->X);
}

void STXA(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->absolute(), cpu->X);
}

void STYZP(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->zeroPage(), cpu->Y);
}

void STYZPX(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->zeroPageX(), cpu->Y);
}

void STYA(CPU * cpu) {
    CYCL
    cpu->mem->write(cpu->absolute(), cpu->Y);
}
