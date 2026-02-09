# Constexpr Compile-Time Optimization

This feature enables compile-time execution of 6502 programs, allowing the C++ compiler to optimize and "bake" the execution results directly into the binary.

## Overview

The constexpr feature allows you to:
- Define a 6502 program at compile-time
- Have the C++ compiler execute the program during compilation
- Embed the final CPU and memory state directly into the executable
- Achieve "infinitely fast" execution at runtime (0 nanoseconds) since the computation already happened

## How It Works

The implementation uses C++17's `constexpr` capabilities to make the entire emulator evaluable at compile-time:

1. **ConstexprMemory** - A constexpr-compatible memory class using `std::array`
2. **ConstexprCPUState** - A struct holding the CPU state (registers, flags, memory)
3. **ConstexprCPU** - A static class with constexpr instruction implementations

When you define a 6502 program and mark it as `constexpr`, the C++ compiler will:
- Execute the entire program during compilation
- Optimize the execution completely
- Embed only the final result in the binary

## Files

- `src/core/constexpr_memory.h` - Constexpr-compatible memory implementation
- `src/core/constexpr_cpu_impl.h` - Constexpr-compatible CPU with all instructions
- `src/constexpr_example.cpp` - Example demonstrating compile-time execution

## Usage Example

```cpp
#include "constexpr_memory.h"
#include "constexpr_cpu_impl.h"

// Create your program at compile-time
constexpr ConstexprMemory createProgram() {
    ConstexprMemory mem;
    
    // LDA #$42 - Load 0x42 into accumulator
    mem.mem[0x8000] = 0xA9;  
    mem.mem[0x8001] = 0x42;
    
    // STA $0200 - Store accumulator at 0x0200
    mem.mem[0x8002] = 0x8D;
    mem.mem[0x8003] = 0x00;
    mem.mem[0x8004] = 0x02;
    
    // JMP $8005 - Infinite loop
    mem.mem[0x8005] = 0x4C;
    mem.mem[0x8006] = 0x05;
    mem.mem[0x8007] = 0x80;
    
    // Set reset vector
    mem.mem[0xFFFC] = 0x00;
    mem.mem[0xFFFD] = 0x80;
    
    return mem;
}

// Execute at compile-time
constexpr ConstexprCPUState executeProgram() {
    ConstexprMemory mem = createProgram();
    ConstexprCPUState state(mem);
    state = ConstexprCPU::reset(state);
    state = ConstexprCPU::executeUntil(state, 1000);
    return state;
}

// This computation happens at COMPILE TIME!
constexpr auto result = executeProgram();

int main() {
    // The result is already computed - no runtime cost!
    std::cout << "A = " << (int)result.A << std::endl;  // Prints: A = 66 (0x42)
    std::cout << "Value at 0x0200 = " << (int)result.memory.read(0x0200) << std::endl;
    return 0;
}
```

## Building

```bash
cd build
cmake ..
cmake --build . --target 6502_constexpr_example
./6502_constexpr_example
```

## Supported Instructions

Currently implemented instructions for constexpr execution:
- **LDA** - Load Accumulator (all addressing modes)
- **STA** - Store Accumulator (all addressing modes)
- **JMP** - Jump (absolute and indirect)
- **NOP** - No Operation
- **BRK** - Break

More instructions can be easily added by following the same pattern in `constexpr_cpu_impl.h`.

## Performance Benefits

Since the program is executed at compile-time:
- **Runtime execution: 0 nanoseconds** - The computation already happened!
- **Memory state is baked in** - The final memory is embedded in the binary
- **Perfect for known programs** - If you know the program ahead of time, you get maximum performance

## Limitations

1. The program must be known at compile-time
2. Cannot handle runtime input or dynamic behavior
3. Limited to C++'s constexpr evaluation limits (recursion depth, iterations)
4. Compile time will increase proportional to program complexity

## Use Cases

This feature is ideal for:
- **Boot ROMs** - Execute boot code at compile-time and embed the initialized state
- **Fixed Programs** - Programs that don't change, like test suites or demos
- **State Initialization** - Pre-compute complex initialization sequences
- **Performance Critical** - When you need the absolute fastest "execution" (zero runtime cost)

## Technical Details

### Why Header-Only?

Constexpr functions must be visible in every translation unit that uses them. This is why the implementation is in headers rather than .cpp files.

### Compiler Support

Requires:
- C++17 or later
- A compiler with good constexpr support (GCC 7+, Clang 5+, MSVC 2017+)

### Memory Size

The entire 64KB memory state is embedded in the binary. For the example program, this is fine, but for larger applications, consider:
- Using smaller memory configurations
- Only embedding the relevant portions of memory
- Using compression for the embedded state

## Extending the Implementation

To add more instructions:

1. Add the instruction implementation to `ConstexprCPU` in `constexpr_cpu_impl.h`
2. Add the opcode case to the `executeInstruction` switch statement
3. Ensure all operations are constexpr-compatible (no runtime-only operations)

Example:
```cpp
static constexpr void LDX_IMM(ConstexprCPUState& state) {
    Word addr = getImmediateAddr(state);
    state.X = state.memory.read(addr);
    state.cycl();
    state.setNZ(state.X);
}
```

Then add to the switch:
```cpp
case 0xA2: LDX_IMM(state); break;
```

## Future Enhancements

Possible improvements:
- Full instruction set implementation
- Constexpr-compatible peripheral emulation
- Build-time code generation from 6502 assembly
- Optimization passes at compile-time
- Automatic program verification during compilation
