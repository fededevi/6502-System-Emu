#include "cpu.h"

void STAZ(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->zeroPage(), cpu->A);
}

void STAZX(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->zeroPageX(), cpu->A);
}

void STAA(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->absolute(), cpu->A);
}

void STAAX(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->absoluteX(), cpu->A);
}

void STAAY(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->absoluteY(), cpu->A);
}

void STAIX(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->indirectX(), cpu->A);
}

void STAIY(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->indirectY(), cpu->A);
}

void STXZP(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->zeroPage(), cpu->X);
}

void STXZPY(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->zeroPageY(), cpu->X);
}

void STXA(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->absolute(), cpu->X);
}

void STYZP(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->zeroPage(), cpu->Y);
}

void STYZPX(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->zeroPageX(), cpu->Y);
}

void STYA(CPU * cpu) {
    cpu->cycl();
    cpu->mem->write(cpu->absolute(), cpu->Y);
}
