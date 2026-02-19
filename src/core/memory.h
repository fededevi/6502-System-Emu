#pragma once

#define MEMORY_SIZE 65536

#include "types.h"
#include <cstddef>  // for size_t

class Memory
{
public:
    Memory();
    static Memory randomMemory();

public:
    Byte mem[MEMORY_SIZE];
    inline Byte read(Word addr) const { return mem[addr]; }
    inline Word read16(Word addr) const { Byte low = mem[addr]; Byte high = mem[(addr + 1) & 0xFFFF]; return static_cast<Word>((high << 8) | low); }
    inline void write(Word addr, Byte value) { mem[addr] = value; }
    inline void writeBlock(Word startAddr, const Byte* data, size_t length) { for (size_t i = 0; i < length && (startAddr + i) < MEMORY_SIZE; ++i) mem[startAddr + i] = data[i]; }
};
