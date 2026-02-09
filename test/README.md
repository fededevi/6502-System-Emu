# 6502 Emulator Test Suite

This directory contains comprehensive automated tests for the 6502 CPU emulator using Google Test framework.

## Test Coverage

The test suite covers all implemented instructions:

### Core Tests (Original)
- **cputest.cpp** - CPU core functionality, addressing modes, and flag operations
- **loadtest.cpp** - LDA, LDX, LDY instructions with all addressing modes
- **storetest.cpp** - STA, STX, STY instructions
- **addcarrytest.cpp** - ADC (Add with Carry) instruction

### Additional Instruction Tests
- **transfertest.cpp** - TAX, TAY, TXA, TYA, TSX, TXS
- **stacktest.cpp** - PHA, PLA, PHP, PLP
- **logicaltest.cpp** - AND, ORA, EOR with multiple addressing modes
- **branchtest.cpp** - BCC, BCS, BEQ, BNE, BMI, BPL, BVC, BVS
- **incdectest.cpp** - INX, INY, DEX, DEY, INC, DEC
- **comparetest.cpp** - CMP, CPX, CPY
- **shiftstest.cpp** - ASL, LSR, ROL, ROR
- **flagstest.cpp** - CLC, SEC, CLI, SEI, CLV, CLD, SED
- **misctest.cpp** - NOP, JMP, JSR, RTS, RTI, BIT

## Building and Running Tests

### Prerequisites
- CMake 3.14 or higher
- C++17 compatible compiler
- Internet connection (Google Test is downloaded automatically during CMake configuration)

### Build Instructions

From the project root directory:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Or from the test directory (after initial CMake configuration):

```bash
cd build
make 6502_tests
```

### Run Tests

Using CTest (recommended):

```bash
cd build
ctest --output-on-failure
```

Or run the test executable directly:

```bash
cd build
./test/6502_tests
```

For verbose output with individual test details:

```bash
./test/6502_tests --gtest_print_time=1
```

## Continuous Integration

The tests are automatically run on every commit via GitHub Actions. See `.github/workflows/ci.yml` for the CI configuration.

## Test Structure

All tests use Google Test framework and follow this pattern:
- Each test file is a C++ source file (.cpp)
- Test fixtures inherit from ::testing::Test
- Individual test methods use TEST_F macro
- Tests use EXPECT_EQ and other Google Test assertions

Example test structure:

```cpp
#include <gtest/gtest.h>
#include "../cpu.h"
#include "../memory.h"

class MyTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;
    
    MyTest() : cpu(&mem) {}
    
    void SetUp() override {
        // Setup code here
    }
};

TEST_F(MyTest, TestSomething) {
    cpu.reset();
    EXPECT_EQ((Byte)0x00, cpu.A);
}
```

## Adding New Tests

To add new tests:
1. Create a new test source file (e.g., `mynewtest.cpp`)
2. Add the source file to `CMakeLists.txt` in the TEST_SOURCES section
3. Rebuild the project

Example:

```cpp
// mynewtest.cpp
#include <gtest/gtest.h>
#include "../cpu.h"
#include "../memory.h"

class MyNewTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;
    
    MyNewTest() : cpu(&mem) {}
};

TEST_F(MyNewTest, MyFirstTest) {
    // Your test code here
    EXPECT_EQ(1, 1);
}
```
