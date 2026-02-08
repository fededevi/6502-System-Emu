

#include "cpu.h"

Word binarySum(CPU * cpu, Word addr) {
    return cpu->A + cpu->C() + cpu->mem->read(addr);
}

Word decimalSum(CPU * cpu, Word addr) {
    //TODO:implement
    return cpu->A + cpu->C() + cpu->mem->read(addr);
}

void ADFlags(CPU * cpu, Word binary, Word decimal) {
    Byte result = binary & 0xFF;
    cpu->setV((binary & 0x80) != (cpu->A & 0x80));
    cpu->setZ(result == 0);
    cpu->setN((result & 0x80) > 0);
    cpu->setC(binary > 0XFF);

    if (cpu->D())
        cpu->setC(decimal > 99);
}

void ADCIX( CPU * cpu) {
    Word addr = cpu->indirectX();
    Word binary = binarySum(cpu, addr);
    Word decimal = decimalSum(cpu, addr);
    ADFlags(cpu, binary, decimal );
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCZ( CPU * cpu) {
    Word addr = cpu->zeroPage();
    Word binary = binarySum(cpu, addr);
    Word decimal = decimalSum(cpu, addr);
    ADFlags(cpu, binary, decimal );
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCI( CPU * cpu) {
    Word addr = cpu->immediate();
    Word binary = binarySum(cpu, addr);
    Word decimal = decimalSum(cpu, addr);
    ADFlags(cpu, binary, decimal );
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCA( CPU * cpu) {
    Word addr = cpu->absolute();
    Word binary = binarySum(cpu, addr);
    Word decimal = decimalSum(cpu, addr);
    ADFlags(cpu, binary, decimal );
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCIY( CPU * cpu) {
    Word addr = cpu->indirectY();
    Word binary = binarySum(cpu, addr);
    Word decimal = decimalSum(cpu, addr);
    ADFlags(cpu, binary, decimal );
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCZX( CPU * cpu) {
    Word addr = cpu->zeroPageX();
    Word binary = binarySum(cpu, addr);
    Word decimal = decimalSum(cpu, addr);
    ADFlags(cpu, binary, decimal );
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCAY( CPU * cpu) {
    Word addr = cpu->absoluteY();
    Word binary = binarySum(cpu, addr);
    Word decimal = decimalSum(cpu, addr);
    ADFlags(cpu, binary, decimal );
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCAX( CPU * cpu) {
    Word addr = cpu->absoluteX();
    Word binary = binarySum(cpu, addr);
    Word decimal = decimalSum(cpu, addr);
    ADFlags(cpu, binary, decimal );
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}
