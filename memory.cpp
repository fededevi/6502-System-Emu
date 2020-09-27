#include "memory.h"

uint8_t Memory::read(uint8_t addr) const {
    return mem[addr];
}

void Memory::write(uint8_t addr, uint8_t value) {
    mem[addr] = value;
}
