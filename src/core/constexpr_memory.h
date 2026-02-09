#pragma once

#include "types.h"
#include <array>
#include <cstddef>  // for size_t

#define MEMORY_SIZE 65536

// Constexpr-compatible Memory class for compile-time optimization
class ConstexprMemory
{
public:
    std::array<Byte, MEMORY_SIZE> mem;

    // Default constructor initializes all memory to zero
    constexpr ConstexprMemory() : mem{} {}

    // Constructor that takes initial memory state
    constexpr ConstexprMemory(const std::array<Byte, MEMORY_SIZE>& initial) : mem(initial) {}

    // Read a byte from memory
    constexpr Byte read(Word addr) const {
        return mem[addr];
    }

    // Read a 16-bit word from memory (little-endian)
    constexpr Word read16(Word addr) const {
        Byte low = read(addr);
        Byte high = read(addr + 1);
        return (high << 8) | low;
    }

    // Write a byte to memory
    constexpr void write(Word addr, Byte value) {
        mem[addr] = value;
    }

    // Write a block of data to memory
    constexpr void writeBlock(Word startAddr, const Byte* data, size_t length) {
        for (size_t i = 0; i < length && (startAddr + i) < MEMORY_SIZE; i++) {
            mem[startAddr + i] = data[i];
        }
    }
};
