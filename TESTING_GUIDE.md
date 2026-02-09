# Quick Start Guide - Testing the 6502 Emulator

## Overview

This repository now includes industry-standard binary test programs for comprehensive CPU validation.

## What Was Added

### 1. Test Programs Directory (`test_programs/`)
- **6502_functional_test.bin** - Klaus Dormann's complete functional test suite (64KB)
- **6502_functional_test.lst** - Assembly listing with detailed comments
- **6502_functional_test.hex** - Hexdump verification (first 100 lines)
- **6502_functional_test.xxd** - XXD format hexdump (first 100 lines)
- **README.md** - Complete documentation for test programs
- **verify_tests.sh** - Script to verify test file integrity

### 2. Test Runner Application
- **test_runner.cpp** - C++ application to load and execute binary tests
- **Makefile.test_runner** - Standalone Makefile for building
- **build_test_runner.sh** - Automated build script

### 3. Documentation Updates
- Updated main **README.md** with test program information
- Comprehensive **test_programs/README.md** documenting test suite

## Quick Start

### Verify Test Files

```bash
cd test_programs
./verify_tests.sh
```

Expected output:
```
✓ Binary size correct: 65536 bytes (64KB)
✓ Hexdump file exists: 6502_functional_test.hex
✓ XXD hexdump file exists: 6502_functional_test.xxd
✓ Listing file exists: 6502_functional_test.lst
```

### Build the Test Runner

#### Using the build script (recommended)
```bash
./build_test_runner.sh
```

#### Using the standalone Makefile
```bash
make -f Makefile.test_runner
```

### Run the Tests

```bash
./test_runner
```

## Understanding Test Results

### Success
If the emulator passes all tests, you'll see:
```
✓ TEST PASSED!
Cycles executed: XXXXXXX
Time elapsed: XXX ms
```

The test will loop at address **0x3469**.

### Failure
If a test fails, you'll see:
```
✗ TEST FAILED!
Stopped at address: 0xXXXX
Expected to stop at: 0x3469
Cycles executed: XXXXX
```

The failure address indicates where the test stopped. Check the listing file (`6502_functional_test.lst`) to see which instruction or test case failed.

### Example Output

```
6502 Test Program Runner
=========================

Running Klaus Dormann's 6502 Functional Test...
Loading test_programs/6502_functional_test.bin (65536 bytes) at 0x0
Starting test execution at PC=0x400
Expected success address: 0x3469
.........
Loop detected at PC=0x674

✗ TEST FAILED!
Stopped at address: 0x674
Expected to stop at: 0x3469
Cycles executed: 42075

CPU State:
  PC: 0x674
  SP: 0xff
  A:  0x10
  X:  0xff
  Y:  0x0
  P:  0x1 [N:1 V:0 B:0 D:0 I:0 Z:0 C:0]
```

## Debugging Failures

1. **Find the failing address** in the test output
2. **Open** `test_programs/6502_functional_test.lst`
3. **Search** for the hex address (e.g., "0674")
4. **Read** the comments to understand what instruction/test failed
5. **Fix** the corresponding instruction implementation in the emulator
6. **Rebuild** and re-run the tests

## Test Coverage

Klaus Dormann's functional test validates:
- ✅ All official 6502 instructions
- ✅ All addressing modes
- ✅ Flag behavior (N, V, Z, C, etc.)
- ✅ Edge cases and boundary conditions
- ✅ Decimal mode (BCD) operations
- ✅ Stack operations
- ✅ Interrupt handling

## Additional Resources

- **Test Source**: https://github.com/Klaus2m5/6502_65C02_functional_tests
- **6502 Documentation**: http://www.6502.org/
- **Visual 6502**: http://visual6502.org/

## Integration with CI/CD

The test runner can be integrated into CI/CD pipelines:

```bash
#!/bin/bash
# Build and test
make -f Makefile.test_runner
./test_runner
if [ $? -eq 0 ]; then
    echo "All tests passed!"
    exit 0
else
    echo "Tests failed!"
    exit 1
fi
```

## Hexdump Files

The hexdump files serve multiple purposes:

1. **Verification** - Confirm test binary downloaded correctly
2. **Debugging** - Inspect memory layout and initial state
3. **Documentation** - Show what the binary contains

Generate new hexdumps:
```bash
hexdump -C 6502_functional_test.bin | head -100 > 6502_functional_test.hex
xxd 6502_functional_test.bin | head -100 > 6502_functional_test.xxd
```

## What This Means in Italian

Il repository ora include:
- **Programmi di test** completi per la CPU 6502
- **Hexdump** dei file di test per verificare l'integrità
- **Test runner** per eseguire i test binari
- **Documentazione** completa su come usare i test

Tutto è pronto per verificare che l'emulatore 6502 funzioni correttamente!
