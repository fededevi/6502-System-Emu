#include "memory.h"

#include<iostream>
#include<chrono>

// Memory accessors are now defined inline in memory.h for performance (inlining eliminates call overhead in the hot path).
// Definitions moved to header to allow compiler to optimize reads/writes directly.


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
