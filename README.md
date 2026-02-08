# 6502-System-Emu

A comprehensive 6502 CPU emulator implementation with extensive testing capabilities.

## Features

- Full 6502 instruction set implementation
- Comprehensive unit tests for all instructions
- Industry-standard functional test programs integrated
- Hexdump utilities for verification

## Project Structure

- **Core Emulator:**
  - `cpu.h`, `cpu.cpp` - CPU implementation
  - `memory.h`, `memory.cpp` - Memory system
  - `*.cpp` - Individual instruction implementations

- **Unit Tests:**
  - `test/` - Qt-based unit test suite
  - See `test/README.md` for details

- **Binary Test Programs:**
  - `test_programs/` - Industry-standard 6502 test binaries
  - Includes Klaus Dormann's functional test suite
  - See `test_programs/README.md` for details

## Building

### Main Emulator

```bash
qmake6 6502.pro
make
```

### Unit Tests

```bash
cd test
qmake6 test.pro
make
./test
```

### Test Runner (for binary test programs)

```bash
./build_test_runner.sh
./test_runner
```

Or manually:

```bash
qmake6 test_runner.pro
make
./test_runner
```

## Test Programs

The `test_programs/` directory contains comprehensive binary test programs:

- **6502_functional_test.bin** - Klaus Dormann's complete functional test
- **Hexdump files** - For verification and debugging
- **Listing files** - Assembly source with comments

To verify test program integrity:

```bash
cd test_programs
./verify_tests.sh
```

## Testing

This project includes multiple levels of testing:

1. **Unit Tests** - Qt-based tests for individual instructions (`test/`)
2. **Functional Tests** - Binary test programs that exercise the full CPU (`test_programs/`)
3. **Hexdump Verification** - Ensure test binaries are loaded correctly

## Resources

- [Klaus Dormann's 6502 Test Suite](https://github.com/Klaus2m5/6502_65C02_functional_tests)
- [6502.org Documentation](http://www.6502.org/)
- [Visual 6502](http://visual6502.org/)
