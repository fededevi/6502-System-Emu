#include "memory.h"

#include<iostream>
#include<chrono>

Byte Memory::read(Word addr) const {
    return mem[addr];
}

Word Memory::read16(Word addr) const {
    Byte low = read(addr);
    Byte high = read(addr+1);
    return (high << 8) + low;
}

void Memory::write(Word addr, Byte value) {
    mem[addr] = value;
}

void Memory::writeBlock(Word startAddr, const Byte* data, size_t length) {
    for (size_t i = 0; i < length && (startAddr + i) < MEMORY_SIZE; i++) {
        mem[startAddr + i] = data[i];
    }
}

Memory::Memory() {
    for(int i=0; i<MEMORY_SIZE; i++)
        mem[i] = 0;
}

Memory Memory::randomMemory()
{
    Memory m;
    unsigned int seed = time(NULL);
    srand(seed);

    std::cout << "Memory initialized with seed: " << seed << std::endl;

    for(int i=0; i<MEMORY_SIZE; i++)
        m.write(i, rand());
    return m;
}
