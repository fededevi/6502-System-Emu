#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

class Memory
{
public:
    Memory();

    uint8_t mem[65536];

    uint8_t read( uint16_t addr);
    void write( uint16_t addr, uint8_t value );
};

#endif // MEMORY_H
