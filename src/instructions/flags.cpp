#include "cpu.h"

#define CYCL cpu->cycl();

// CLC - Clear Carry Flag (0x18)
void CLC(CPU * cpu) {
    CYCL
    cpu->setC(false);
}

// SEC - Set Carry Flag (0x38)
void SEC(CPU * cpu) {
    CYCL
    cpu->setC(true);
}

// CLI - Clear Interrupt Disable (0x58)
void CLI(CPU * cpu) {
    CYCL
    cpu->setI(false);
}

// SEI - Set Interrupt Disable (0x78)
void SEI(CPU * cpu) {
    CYCL
    cpu->setI(true);
}

// CLV - Clear Overflow Flag (0xB8)
void CLV(CPU * cpu) {
    CYCL
    cpu->setV(false);
}

// CLD - Clear Decimal Mode (0xD8)
void CLD(CPU * cpu) {
    CYCL
    cpu->setD(false);
}

// SED - Set Decimal Mode (0xF8)
void SED(CPU * cpu) {
    CYCL
    cpu->setD(true);
}
