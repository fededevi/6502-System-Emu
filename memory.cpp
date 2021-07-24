#include "memory.h"

Byte Memory::read(Word addr) const {
    return mem[addr];
}

Word Memory::read16(Word addr) const {
    Word w;
    w = read(addr) + (read(addr+1) << 8);
    return w;
}

void Memory::write(Word addr, Byte value) {
    mem[addr] = value;
}
