#include "cpu.h"

#define CYCL cpu->cycl();

Word binarySubtract(CPU * cpu, Word addr) {
    // Perform binary subtraction: A - M - (1 - C)
    // Note: On 6502, carry flag acts as "NOT borrow" for subtraction
    return cpu->A - cpu->mem->read(addr) - (cpu->C() ? 0 : 1);
}

Word decimalSubtract(CPU * cpu, Word addr) {
    // TODO: implement proper BCD (Binary Coded Decimal) subtraction
    // For now, just perform binary subtraction even in decimal mode
    // This is a known limitation
    return cpu->A - cpu->mem->read(addr) - (cpu->C() ? 0 : 1);
}

void SBCFlags(CPU * cpu, Word binary, Word decimal) {
    Byte result = binary & 0xFF;
    Byte operand = cpu->mem->read(cpu->addr16);
    
    // Set overflow flag: overflow occurs when sign of result differs from sign of A
    // and A and operand have different signs
    cpu->setV(((cpu->A ^ result) & 0x80) && ((cpu->A ^ operand) & 0x80));
    cpu->setZ(result == 0);
    cpu->setN((result & 0x80) > 0);
    // Carry flag: set when NO borrow (i.e., result is non-negative)
    cpu->setC(!(binary & 0x100));  // Clear carry if borrow occurred
    
    if (cpu->D())
        cpu->setC(decimal >= 0);
}

// SBC - Subtract with Carry
void SBCIX(CPU * cpu) {
    Word addr = cpu->indirectX();
    cpu->addr16 = addr;
    Word binary = binarySubtract(cpu, addr);
    Word decimal = decimalSubtract(cpu, addr);
    SBCFlags(cpu, binary, decimal);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCZP(CPU * cpu) {
    Word addr = cpu->zeroPage();
    cpu->addr16 = addr;
    Word binary = binarySubtract(cpu, addr);
    Word decimal = decimalSubtract(cpu, addr);
    SBCFlags(cpu, binary, decimal);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCI(CPU * cpu) {
    Word addr = cpu->immediate();
    cpu->addr16 = addr;
    Word binary = binarySubtract(cpu, addr);
    Word decimal = decimalSubtract(cpu, addr);
    SBCFlags(cpu, binary, decimal);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCA(CPU * cpu) {
    Word addr = cpu->absolute();
    cpu->addr16 = addr;
    Word binary = binarySubtract(cpu, addr);
    Word decimal = decimalSubtract(cpu, addr);
    SBCFlags(cpu, binary, decimal);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCIY(CPU * cpu) {
    Word addr = cpu->indirectY();
    cpu->addr16 = addr;
    Word binary = binarySubtract(cpu, addr);
    Word decimal = decimalSubtract(cpu, addr);
    SBCFlags(cpu, binary, decimal);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCZPX(CPU * cpu) {
    Word addr = cpu->zeroPageX();
    cpu->addr16 = addr;
    Word binary = binarySubtract(cpu, addr);
    Word decimal = decimalSubtract(cpu, addr);
    SBCFlags(cpu, binary, decimal);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCAY(CPU * cpu) {
    Word addr = cpu->absoluteY();
    cpu->addr16 = addr;
    Word binary = binarySubtract(cpu, addr);
    Word decimal = decimalSubtract(cpu, addr);
    SBCFlags(cpu, binary, decimal);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCAX(CPU * cpu) {
    Word addr = cpu->absoluteX();
    cpu->addr16 = addr;
    Word binary = binarySubtract(cpu, addr);
    Word decimal = decimalSubtract(cpu, addr);
    SBCFlags(cpu, binary, decimal);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}
