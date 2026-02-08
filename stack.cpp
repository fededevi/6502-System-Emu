#include "cpu.h"

#define CYCL cpu->cycl();

void SetNZ(CPU * cpu, Byte reg);

// PHA - Push Accumulator (0x48)
void PHA(CPU * cpu) {
    CYCL
    cpu->push(cpu->A);
    CYCL
}

// PLA - Pull Accumulator (0x68)
void PLA(CPU * cpu) {
    CYCL
    CYCL
    cpu->SP++;
    cpu->A = cpu->mem->read(0x100 + cpu->SP);
    CYCL
    SetNZ(cpu, cpu->A);
}

// PHP - Push Processor Status (0x08)
void PHP(CPU * cpu) {
    CYCL
    cpu->push(cpu->P | 0x10);  // B flag is set when pushed
    CYCL
}

// PLP - Pull Processor Status (0x28)
void PLP(CPU * cpu) {
    CYCL
    CYCL
    cpu->SP++;
    cpu->P = cpu->mem->read(0x100 + cpu->SP);
    CYCL
}
