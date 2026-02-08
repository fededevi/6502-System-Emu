#include "cpu.h"

#define CYCL cpu->cycl();

Word binarySubtract(CPU * cpu, Word addr) {
    // Perform binary subtraction: A - M - (1 - C)
    // Note: On 6502, carry flag acts as "NOT borrow" for subtraction
    return cpu->A - cpu->mem->read(addr) - (cpu->C() ? 0 : 1);
}

Word decimalSubtract(CPU * cpu, Word addr) {
    // BCD (Binary Coded Decimal) subtraction
    // Each nibble represents a decimal digit (0-9)
    Byte a = cpu->A;
    Byte b = cpu->mem->read(addr);
    Byte borrow = cpu->C() ? 0 : 1;  // On 6502, carry flag acts as "NOT borrow"
    
    // Subtract low nibbles (ones place)
    int lowNibble = (a & 0x0F) - (b & 0x0F) - borrow;
    int lowBorrow = 0;
    if (lowNibble < 0) {
        lowNibble -= 6;  // Adjust for BCD
        lowBorrow = 1;
    }
    
    // Subtract high nibbles (tens place) with borrow from low nibble
    int highNibble = (a >> 4) - (b >> 4) - lowBorrow;
    if (highNibble < 0) {
        highNibble -= 6;  // Adjust for BCD
    }
    
    // Combine nibbles
    Word result = ((highNibble & 0x0F) << 4) | (lowNibble & 0x0F);
    
    return result;
}

void SBCFlags(CPU * cpu, Word binary, Word decimal) {
    Byte result = cpu->D() ? (decimal & 0xFF) : (binary & 0xFF);
    Byte operand = cpu->mem->read(cpu->addr16);
    
    // Set overflow flag: overflow occurs when sign of result differs from sign of A
    // and A and operand have different signs
    cpu->setV(((cpu->A ^ result) & 0x80) && ((cpu->A ^ operand) & 0x80));
    
    cpu->setZ(result == 0);
    cpu->setN((result & 0x80) > 0);
    
    if (cpu->D()) {
        // In decimal mode, carry is set if NO borrow occurred (result >= 0)
        cpu->setC(!(decimal & 0x100));
    } else {
        // In binary mode, carry is set when NO borrow (i.e., result is non-negative)
        cpu->setC(!(binary & 0x100));
    }
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
