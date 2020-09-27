#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

class Memory
{
public:
    Byte mem[65536];

    Byte read( Byte addr) const;
    void write( Byte addr, Byte value );
};

#endif // MEMORY_H
