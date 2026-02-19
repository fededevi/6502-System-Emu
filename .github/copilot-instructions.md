# 6502-System-Emu Copilot Instructions

## Project Overview

This is a comprehensive 6502 CPU emulator written in C++17. It provides:
- Full 6502 instruction set implementation with decimal mode (BCD) support
- Industry-standard test suite integration (Klaus Dormann's functional tests)
- Modern CMake build system
- Comprehensive Google Test-based unit tests
- Command-line executable for running binary programs

**Key Technologies:**
- Language: C++17
- Build System: CMake 3.14+
- Testing Framework: Google Test (automatically downloaded)
- Target: Cross-platform (Linux, macOS, Windows)

## Build Instructions

### Standard Build Process

### Common Build Variations

```bash
# Clean build
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```

**Important:** CMake automatically downloads Google Test during configuration. This requires internet connection on first build.

## Testing

### Running All Tests


**Important — always run the functional and unit test locally before committing:** the functional test exercises the full emulator and catches integration regressions. You can run it via `ctest` (included above) or directly with the emulator:

```bash
# unit test binary (from build directory or use the Release/Debug binary path)
./6502_tests

# run via emulator (from build directory or use the Release/Debug binary path)
./6502_emu -f ../test_programs/6502_functional_test.bin -a 0x0000 -pc 0x0400
```

### Running Individual Tests

```bash
# Unit tests only
./test/6502_tests

# Run specific test suite
./test/6502_tests --gtest_filter="LoadTest.*"

# Functional test via emulator
./6502_emu -f ../test_programs/6502_functional_test.bin -a 0x0000 -pc 0x0400
```

## Project Structure

### Key Files

- **src/core/cpu.h** - CPU class definition, all public methods
- **src/core/cpu.cpp** - Main CPU execution engine, fetch-decode-execute
- **src/core/memory.h** - Memory class with 64KB array
- **CMakeLists.txt** - Build configuration, defines EMULATOR_SOURCES
- **.github/workflows/ci.yml** - CI pipeline configuration


### CPU Implementation

The CPU class in `src/core/cpu.cpp` contains:
- **execute()** - Main instruction execution method
- **reset()** - Reset CPU to initial state
- **fetch()** - Fetch next byte/word
- Instruction implementations organized by category in `src/instructions/`

### Memory System

- 64KB address space (0x0000-0xFFFF)
- Public methods: `read()`, `write()`, `writeBlock()`
- Memory is encapsulated; use public API, not direct array access

### Instruction Files

Each instruction category has its own file in `src/instructions/`:
- One file per category (e.g., all load instructions in load.cpp)
- Instructions are member functions of the CPU class
- Follow existing naming patterns (e.g., `ADC_IM`, `LDA_ZP`)


