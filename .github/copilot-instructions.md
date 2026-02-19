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

**ALWAYS follow this exact sequence:**

```bash
# 1. Create build directory (if it doesn't exist)
mkdir -p build
cd build

# 2. Configure with CMake
cmake ..

# 3. Build the project
cmake --build .
```

**Build time:** Initial build takes ~30-60 seconds (includes downloading Google Test). Incremental builds are faster.

### Common Build Variations

```bash
# Ninja (faster builds)
cmake -G Ninja ..
ninja

# Verbose output (for debugging build issues)
make VERBOSE=1

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

**ALWAYS run tests from the build directory:**

```bash
cd build
ctest --output-on-failure
```

This runs:
1. All Google Test unit tests
2. Klaus Dormann's functional test suite
3. Test program verification

**Test execution time:** Unit tests complete in <5 seconds. Full functional test can take 30-60 seconds.

### Running Individual Tests

```bash
# Unit tests only
./test/6502_tests

# Run specific test suite
./test/6502_tests --gtest_filter="LoadTest.*"

# Functional test via emulator
./6502_emu -f ../test_programs/6502_functional_test.bin -a 0x0000 -pc 0x0400
```

### Test Expectations

- **All unit tests should pass** - If they fail, there's a regression in CPU implementation
- **Functional test currently fails at 0x674** - This is expected; the test reveals existing bugs
- Test infrastructure is working correctly even when tests fail

## Project Structure

### Directory Layout

```
.
├── src/                        # Source code
│   ├── core/                   # Core emulator components
│   │   ├── cpu.h, cpu.cpp     # CPU implementation (main execution engine)
│   │   ├── memory.h, memory.cpp  # Memory system (64KB)
│   │   └── types.h            # Type definitions (Byte, Word, etc.)
│   ├── instructions/          # Instruction implementations
│   │   ├── load.cpp           # LDA, LDX, LDY
│   │   ├── store.cpp          # STA, STX, STY
│   │   ├── addcarry.cpp       # ADC
│   │   ├── subtract.cpp       # SBC
│   │   ├── logical.cpp        # AND, ORA, EOR
│   │   ├── transfer.cpp       # TAX, TAY, TXA, etc.
│   │   ├── stack.cpp          # PHA, PLA, PHP, PLP
│   │   ├── branch.cpp         # BCC, BCS, BEQ, etc.
│   │   ├── compare.cpp        # CMP, CPX, CPY
│   │   ├── incdec.cpp         # INX, INY, DEX, DEY, INC, DEC
│   │   ├── shifts.cpp         # ASL, LSR, ROL, ROR
│   │   ├── flags.cpp          # CLC, SEC, CLI, etc.
│   │   └── misc.cpp           # NOP, JMP, JSR, RTS, RTI, BIT
│   └── main.cpp               # CLI executable
├── test/                      # Google Test unit tests
│   ├── cputest.cpp           # Core CPU tests
│   ├── loadtest.cpp          # Load instruction tests
│   ├── storetest.cpp         # Store instruction tests
│   ├── addcarrytest.cpp      # ADC tests
│   ├── subtracttest.cpp      # SBC tests
│   ├── logicaltest.cpp       # AND, ORA, EOR tests
│   ├── transfertest.cpp      # Transfer instruction tests
│   ├── stacktest.cpp         # Stack operation tests
│   ├── branchtest.cpp        # Branch instruction tests
│   ├── comparetest.cpp       # Compare instruction tests
│   ├── incdectest.cpp        # Increment/decrement tests
│   ├── shiftstest.cpp        # Shift/rotate tests
│   ├── flagstest.cpp         # Flag manipulation tests
│   └── misctest.cpp          # Miscellaneous instruction tests
├── test_programs/            # Binary test programs
│   ├── 6502_functional_test.bin  # Klaus Dormann's test (64KB)
│   ├── 6502_functional_test.lst  # Assembly listing
│   └── verify_tests.sh       # Verification script
├── .github/
│   └── workflows/
│       └── ci.yml            # CI configuration
├── CMakeLists.txt            # Root CMake configuration
└── README.md                 # Project documentation
```

### Key Files

- **src/core/cpu.h** - CPU class definition, all public methods
- **src/core/cpu.cpp** - Main CPU execution engine, fetch-decode-execute
- **src/core/memory.h** - Memory class with 64KB array
- **CMakeLists.txt** - Build configuration, defines EMULATOR_SOURCES
- **.github/workflows/ci.yml** - CI pipeline configuration

## Continuous Integration

The CI pipeline (`.github/workflows/ci.yml`) runs on:
- All pushes to `main` branch
- All pushes to `copilot/**` branches
- All pull requests to `main`

**CI Steps:**
1. Install dependencies (cmake, build-essential)
2. Configure CMake
3. Build project
4. Run all tests via `ctest --output-on-failure`
5. Run Klaus functional test with success detection

**Expected CI behavior:**
- Unit tests should pass
- Functional test currently fails (expected)
- Build must complete without errors

## Code Organization

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

## Adding New Code

### Adding New Instructions

1. Add implementation to appropriate file in `src/instructions/`
2. Declare method in `src/core/cpu.h`
3. Add to execute() switch statement in `src/core/cpu.cpp`
4. Create corresponding test in `test/` directory
5. Add test file to CMakeLists.txt in test/ if new file created

### Adding New Tests

1. Create test file in `test/` directory (e.g., `mynewtest.cpp`)
2. Follow existing test structure (inherit from ::testing::Test)
3. Add to TEST_SOURCES in `test/CMakeLists.txt`
4. Rebuild and run tests

**Test Template:**
```cpp
#include <gtest/gtest.h>
#include "cpu.h"
#include "memory.h"

class MyTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;
    MyTest() : cpu(&mem) {}
};

TEST_F(MyTest, TestName) {
    cpu.reset();
    // Test code here
    EXPECT_EQ(expected, actual);
}
```

## Common Issues & Solutions

### Build Issues

1. **"Could not find GoogleTest"** - This is normal; CMake downloads it automatically on first build
2. **Build takes a long time first time** - Google Test download can take 30-60 seconds
3. **"Cannot find cmake"** - Install: `sudo apt-get install cmake build-essential`

### Test Issues

1. **Tests fail after code changes** - Run individual test suites to identify which instruction broke
2. **Functional test fails** - Check PC value; look up in `6502_functional_test.lst` for details
3. **Tests timeout** - Increase the maximum cycle limit for the emulator using the `-m` flag (e.g., `./6502_emu -m 200000000`). This sets how many CPU cycles the emulator will execute before terminating.

### Memory Issues

- **Always use Memory class public API** - Don't access internal arrays directly
- **Bounds checking** - Memory class handles this internally
- **writeBlock()** - Use for bulk memory writes (e.g., loading programs)

## Validation Steps

Before committing changes:

1. **Build successfully:** `cd build && cmake --build .`
2. **Run unit tests:** `ctest --output-on-failure`
3. **Check specific tests affected by your changes**
4. **Verify no new warnings** during compilation
5. **Test on clean build** if adding new files

## Style Guidelines

- **Minimize comments by writing clear code** - Document complex CPU behaviors, edge cases, and non-obvious implementation details that aren't self-evident from code alone
- **Follow existing patterns** - Match the style of surrounding code
- **Use existing types** - Byte, Word, SByte from types.h
- **Consistent naming** - Instructions use opcode_mode format (e.g., LDA_IM, LDA_ZP)

## Quick Reference

### Most Common Commands

```bash
# Full build and test cycle
mkdir -p build && cd build && cmake .. && cmake --build . && ctest --output-on-failure

# Rebuild after changes
cd build && cmake --build . && ctest --output-on-failure

# Run specific test
cd build && ./test/6502_tests --gtest_filter="LoadTest.*"

# Clean build
rm -rf build && mkdir build && cd build && cmake .. && cmake --build .
```

### File Patterns

- CPU implementation: `src/core/cpu.cpp`
- Instruction implementation: `src/instructions/*.cpp`
- Tests: `test/*test.cpp`
- Headers: `src/core/*.h`

## Trust These Instructions

These instructions have been validated by running all commands and observing the results. If something doesn't work as described, check for typos or environment differences before extensive searching.
