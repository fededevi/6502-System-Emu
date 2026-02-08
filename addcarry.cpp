

#include "cpu.h"

Word binarySum(CPU * cpu, Word addr) {
    return cpu->A + cpu->C() + cpu->mem->read(addr);
}

Word decimalSum(CPU * cpu, Word addr) {
    // BCD (Binary Coded Decimal) addition
    // Each nibble represents a decimal digit (0-9)
    Byte a = cpu->A;
    Byte b = cpu->mem->read(addr);
    Byte carry = cpu->C();
    
    // Add low nibbles (ones place)
    Word lowNibble = (a & 0x0F) + (b & 0x0F) + carry;
    if (lowNibble > 9) {
        lowNibble += 6;  // Adjust for BCD
    }
    
    // Add high nibbles (tens place) with carry from low nibble
    Word highNibble = (a >> 4) + (b >> 4) + (lowNibble > 15 ? 1 : 0);
    if (highNibble > 9) {
        highNibble += 6;  // Adjust for BCD
    }
    
    // Combine nibbles
    Word result = ((highNibble & 0x0F) << 4) | (lowNibble & 0x0F);
    
    return result;
}

void ADFlags(CPU * cpu, Word binary, Word decimal) {
    Byte result = cpu->D() ? (decimal & 0xFF) : (binary & 0xFF);
    
    // In binary mode, overflow is based on signed arithmetic
    // In decimal mode, the behavior varies by implementation
    // Most commonly, V flag reflects binary overflow even in decimal mode
    cpu->setV((binary & 0x80) != (cpu->A & 0x80));
    
    cpu->setZ(result == 0);
    cpu->setN((result & 0x80) > 0);
    
    if (cpu->D()) {
        // In decimal mode, carry is set if result > 99
        cpu->setC(decimal > 0x99);
    } else {
        // In binary mode, carry is set if result > 255
        cpu->setC(binary > 0xFF);
    }
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
