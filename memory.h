#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

class Memory
{
public:
    Byte mem[65536];

    Byte read( Word addr) const;
    Word read16( Word addr) const;
    void write( Word addr, Byte value );
};

#endif // MEMORY_H
