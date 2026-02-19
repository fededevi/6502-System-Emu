

#include "cpu.h"

#define CYCL cpu->cycl();

Word binarySum(CPU * cpu, Byte memValue) {
    return cpu->A + cpu->C() + memValue;
}

Word decimalSum(CPU * cpu, Byte memValue) {
    // BCD (Binary Coded Decimal) addition
    // Each nibble represents a decimal digit (0-9)
    Byte a = cpu->A;
    Byte b = memValue;
    Byte carry = cpu->C();
    
    // Add low nibbles (ones place)
    Word lowNibble = (a & 0x0F) + (b & 0x0F) + carry;
    Word lowCarry = 0;
    if (lowNibble > 9) {
        lowNibble += 6;  // Adjust for BCD
    }
    // Check if we need to carry to high nibble
    if (lowNibble > 15) {
        lowCarry = 1;
    }
    
    // Add high nibble (tens place) with carry from low nibble
    Word highNibble = (a >> 4) + (b >> 4) + lowCarry;
    Word highCarry = 0;
    if (highNibble > 9) {
        highNibble += 6;  // Adjust for BCD
    }
    // Check if we need to carry out
    if (highNibble > 15) {
        highCarry = 1;
    }
    
    // Combine nibbles - result is in BCD format
    Word result = ((highNibble & 0x0F) << 4) | (lowNibble & 0x0F);
    
    // Return with carry bit in upper byte if overflow
    if (highCarry) {
        result |= 0x100;
    }
    
    return result;
} 

void ADFlags(CPU * cpu, Word binary, Word decimal, Byte memValue) {
    Byte result = cpu->D() ? (decimal & 0xFF) : (binary & 0xFF);
    
    // Overflow flag: set if sign of result differs from sign of both operands
    // V = ~(A ^ M) & (A ^ result) & 0x80
    // This is true when A and M have the same sign, but result has different sign
    cpu->setV(((~(cpu->A ^ memValue)) & (cpu->A ^ result) & 0x80) != 0);
    
    cpu->setZ(result == 0);
    cpu->setN((result & 0x80) > 0);
    
    if (cpu->D()) {
        // In decimal mode, carry is set if result overflowed (bit 8 is set)
        cpu->setC(decimal > 0xFF);
    } else {
        // In binary mode, carry is set if result > 255
        cpu->setC(binary > 0xFF);
    }
}

void ADCIX( CPU * cpu) {
    Word addr = cpu->indirectX();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySum(cpu, memValue);
    Word decimal = decimalSum(cpu, memValue);
    ADFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCZ( CPU * cpu) {
    Word addr = cpu->zeroPage();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySum(cpu, memValue);
    Word decimal = decimalSum(cpu, memValue);
    ADFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCI( CPU * cpu) {
    Word addr = cpu->immediate();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySum(cpu, memValue);
    Word decimal = decimalSum(cpu, memValue);
    ADFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCA( CPU * cpu) {
    Word addr = cpu->absolute();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySum(cpu, memValue);
    Word decimal = decimalSum(cpu, memValue);
    ADFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCIY( CPU * cpu) {
    Word addr = cpu->indirectY();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySum(cpu, memValue);
    Word decimal = decimalSum(cpu, memValue);
    ADFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCZX( CPU * cpu) {
    Word addr = cpu->zeroPageX();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySum(cpu, memValue);
    Word decimal = decimalSum(cpu, memValue);
    ADFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCAY( CPU * cpu) {
    Word addr = cpu->absoluteY();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySum(cpu, memValue);
    Word decimal = decimalSum(cpu, memValue);
    ADFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}

void ADCAX( CPU * cpu) {
    Word addr = cpu->absoluteX();
    CYCL
    Byte memValue = cpu->mem->read(addr);
    Word binary = binarySum(cpu, memValue);
    Word decimal = decimalSum(cpu, memValue);
    ADFlags(cpu, binary, decimal, memValue);
    cpu->A = cpu->D() ? decimal : binary & 0xFF;
}
