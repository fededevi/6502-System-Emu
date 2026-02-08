# 6502 Emulator Test Suite

This directory contains comprehensive automated tests for the 6502 CPU emulator.

## Test Coverage

The test suite covers all implemented instructions:

### Core Tests (Original)
- **cputest.h** - CPU core functionality, addressing modes, and flag operations
- **loadtest.h** - LDA, LDX, LDY instructions with all addressing modes
- **storetest.h** - STA, STX, STY instructions
- **addcarrytest.h** - ADC (Add with Carry) instruction

### New Instruction Tests
- **transfertest.h** - TAX, TAY, TXA, TYA, TSX, TXS
- **stacktest.h** - PHA, PLA, PHP, PLP
- **logicaltest.h** - AND, ORA, EOR with multiple addressing modes
- **branchtest.h** - BCC, BCS, BEQ, BNE, BMI, BPL, BVC, BVS
- **incdectest.h** - INX, INY, DEX, DEY, INC, DEC
- **comparetest.h** - CMP, CPX, CPY
- **shiftstest.h** - ASL, LSR, ROL, ROR
- **flagstest.h** - CLC, SEC, CLI, SEI, CLV, CLD, SED
- **misctest.h** - NOP, JMP, JSR, RTS, RTI, BIT

## Building and Running Tests

### Prerequisites
- Qt 6 development libraries
- qmake6
- g++ or compatible C++ compiler

### Build Instructions

```bash
cd test
qmake6 test.pro
make
```

### Run Tests

```bash
./test
```

## Continuous Integration

The tests are automatically run on every commit via GitHub Actions. See `.github/workflows/ci.yml` for the CI configuration.

## Test Structure

All tests use Qt Test framework and follow this pattern:
- Each test file is a header-only test class
- Tests inherit from QObject and use Q_OBJECT macro
- Individual test methods are declared as private slots
- Tests use QCOMPARE macro for assertions

## Adding New Tests

To add new tests:
1. Create a new test header file (e.g., `mynewtest.h`)
2. Add the header to `test.pro` in the HEADERS section
3. Include the header in `main.cpp`
4. Add a RUN_TEST line in main.cpp

Example:
```cpp
#include "mynewtest.h"
// ...
RUN_TEST(MyNewTest);
```
