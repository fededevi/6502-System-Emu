# 6502-System-Emu


A comprehensive 6502 CPU emulator implementation with extensive testing capabilities.

## Features

- Full 6502 instruction set implementation
- Decimal mode (BCD) arithmetic support
- Comprehensive unit tests for all instructions
- Industry-standard functional test programs integrated
- Hexdump utilities for verification
- Modern CMake build system

### Prerequisites

- CMake 3.14 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)
- Internet connection (for downloading Google Test during build)

### Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests
ctest --output-on-failure
```

### Building with Different Generators

CMake supports multiple build systems:

```bash
# Ninja (faster builds)
cmake -G Ninja ..
ninja

# Make with verbose output
cmake ..
make VERBOSE=1

# Visual Studio (Windows)
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
```

## Project Structure

- **Source Code:**
  - `src/core/` - Core emulator components
    - `cpu.h`, `cpu.cpp` - CPU implementation
    - `memory.h`, `memory.cpp` - Memory system
    - `types.h` - Type definitions
  - `src/instructions/` - Individual instruction implementations
    - `load.cpp`, `store.cpp`, `addcarry.cpp`, etc.
  - `src/main.cpp` - Main executable with command-line interface

- **Unit Tests:**
  - `test/` - Google Test-based unit test suite
  - See `test/README.md` for details

- **Binary Test Programs:**
  - `test_programs/` - Industry-standard 6502 test binaries
  - Includes Klaus Dormann's functional test suite
  - See `test_programs/README.md` for details

## Running the Emulator

After building, the main executable supports command-line arguments for flexible execution:

```bash
# Show help
./build/6502_emu -h

# Run with default reset vector
./build/6502_emu

# Load and run a binary program
./build/6502_emu -f program.bin -a 8000 -pc 8000

# Load program with custom cycle limit
./build/6502_emu -f program.bin -a 0000 -m 1000000
```

**Command-line Options:**
- `-f <file>` - Load binary program from file
- `-a <address>` - Address to load program at (hex, default: 0x0000)
- `-pc <address>` - Set program counter (hex, default: from reset vector)
- `-m <cycles>` - Maximum cycles to execute (default: 100000000)
- `-h` - Display help message

## Testing

This project includes multiple levels of testing:

### 1. Unit Tests

Run the test suite using CTest:

```bash
cd build
ctest --output-on-failure
```

Or run the test executable directly:

```bash
./test/6502_tests
```

### 2. Functional Tests (Binary Test Programs)

The `test_programs/` directory contains comprehensive binary test programs:

- **6502_functional_test.bin** - Klaus Dormann's complete functional test
- **Hexdump files** - For verification and debugging
- **Listing files** - Assembly source with comments

Run all tests (including functional tests):

```bash
cd build
ctest --output-on-failure
```

Or run functional tests using the main executable:

```bash
./build/6502_emu -f test_programs/6502_functional_test.bin -a 0000 -pc 0400
```

### 3. Hexdump Verification

Ensure test binaries are loaded correctly using the provided hexdump utilities.

## Resources

- [Klaus Dormann's 6502 Test Suite](https://github.com/Klaus2m5/6502_65C02_functional_tests)
- [Google Test Documentation](https://google.github.io/googletest/)
- [6502.org Documentation](http://www.6502.org/)
- [Visual 6502](http://visual6502.org/)
