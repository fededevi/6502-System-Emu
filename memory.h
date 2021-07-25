#pragma once

#define MEMORY_SIZE 65536

#include "types.h"

class Memory
{
public:
    Memory();
    static Memory randomMemory();

public:
    Byte mem[MEMORY_SIZE];
    Byte read( Word addr) const;
    Word read16( Word addr) const;
    void write( Word addr, Byte value );
};
