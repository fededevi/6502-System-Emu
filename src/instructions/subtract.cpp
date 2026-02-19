#include "cpu.h"

#define CYCL cpu->cycl();

Word binarySubtract(CPU * cpu, Byte memValue) {
    // Perform binary subtraction: A - M - (1 - C)
    // Note: On 6502, carry flag acts as "NOT borrow" for subtraction
    return cpu->A - memValue - (cpu->C() ? 0 : 1);
}

Word decimalSubtract(CPU * cpu, Byte memValue) {
    // BCD (Binary Coded Decimal) subtraction
    // Each nibble represents a decimal digit (0-9)
    Byte a = cpu->A;
    Byte b = memValue;
    Byte borrow = cpu->C() ? 0 : 1;  // On 6502, carry flag acts as "NOT borrow"
    
    // Subtract low nibble (ones place)
    int lowNibble = (a & 0x0F) - (b & 0x0F) - borrow;
    int lowBorrow = 0;
    if (lowNibble < 0) {
        lowNibble += 10;  // Borrow from high nibble
        lowBorrow = 1;
    }
    
    // Subtract high nibble (tens place) with borrow from low nibble
    int highNibble = (a >> 4) - (b >> 4) - lowBorrow;
    int highBorrow = 0;
    if (highNibble < 0) {
        highNibble += 10;  // Underflow
        highBorrow = 1;
    }
    
    // Combine nibbles
    Word result = ((highNibble & 0x0F) << 4) | (lowNibble & 0x0F);
    
    // Set bit 8 if there was a borrow (for carry flag detection)
    if (highBorrow) {
        result |= 0x100;
    }
    
    return result;
} 

void SBCFlags(CPU * cpu, Word binary, Word decimal, Byte memValue) {
    Byte result = cpu->D() ? (decimal & 0xFF) : (binary & 0xFF);
    
    // Set overflow flag: overflow occurs when A and operand have different signs,
    // and the result has a different sign from A
    // V = (A ^ M) & (A ^ result) & 0x80
    cpu->setV((((cpu->A ^ memValue) & (cpu->A ^ result) & 0x80)) != 0);
    
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
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySubtract(cpu, memValue);
    Word decimal = decimalSubtract(cpu, memValue);
    SBCFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCZP(CPU * cpu) {
    Word addr = cpu->zeroPage();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySubtract(cpu, memValue);
    Word decimal = decimalSubtract(cpu, memValue);
    SBCFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCI(CPU * cpu) {
    Word addr = cpu->immediate();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySubtract(cpu, memValue);
    Word decimal = decimalSubtract(cpu, memValue);
    SBCFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCA(CPU * cpu) {
    Word addr = cpu->absolute();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySubtract(cpu, memValue);
    Word decimal = decimalSubtract(cpu, memValue);
    SBCFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCIY(CPU * cpu) {
    Word addr = cpu->indirectY();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySubtract(cpu, memValue);
    Word decimal = decimalSubtract(cpu, memValue);
    SBCFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCZPX(CPU * cpu) {
    Word addr = cpu->zeroPageX();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySubtract(cpu, memValue);
    Word decimal = decimalSubtract(cpu, memValue);
    SBCFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCAY(CPU * cpu) {
    Word addr = cpu->absoluteY();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySubtract(cpu, memValue);
    Word decimal = decimalSubtract(cpu, memValue);
    SBCFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void SBCAX(CPU * cpu) {
    Word addr = cpu->absoluteX();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySubtract(cpu, memValue);
    Word decimal = decimalSubtract(cpu, memValue);
    SBCFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}
