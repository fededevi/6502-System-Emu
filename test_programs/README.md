# 6502 CPU Test Programs

This directory contains comprehensive test programs for validating the 6502 CPU emulator implementation.

## Test Programs Included

### Klaus Dormann's 6502 Functional Test

**File:** `6502_functional_test.bin`

This is the industry-standard functional test suite for 6502 processors created by Klaus Dormann. It provides comprehensive testing of all 6502 instructions and addressing modes.

**Source:** https://github.com/Klaus2m5/6502_65C02_functional_tests

**Features:**
- Tests all official 6502 instructions
- Validates all addressing modes
- Tests edge cases and flag behavior
- Self-contained test that runs entirely from the binary
- Tests decimal mode operations
- Validates interrupt handling

**How the test works:**
1. The test should be loaded at address $0000
2. Execution starts at address $0400
3. If the test passes, it will loop infinitely at address $3469
4. If the test fails, it will loop at the address where the failure occurred
5. The listing file (`6502_functional_test.lst`) shows the test details and expected behavior

**Files:**
- `6502_functional_test.bin` - Binary test program (64KB)
- `6502_functional_test.lst` - Assembly listing with comments
- `6502_functional_test.hex` - Hexdump of first 100 lines (for verification)
- `6502_functional_test.xxd` - XXD format hexdump of first 100 lines

## Hexdump Files

The hexdump files are provided to verify the integrity of the test binaries:

- **`.hex` files** - Created with `hexdump -C`, shows canonical hex+ASCII display
- **`.xxd` files** - Created with `xxd`, shows hex dump in traditional format

These can be used to:
1. Verify the binary was downloaded correctly
2. Inspect the initial code and data sections
3. Debug memory loading issues

## Using the Test Programs

### Loading into the Emulator

To use these test programs with the 6502 emulator:

```cpp
// Example code to load the functional test
Memory mem;
CPU cpu(&mem);

// Load the test binary at address 0x0000
FILE* fp = fopen("test_programs/6502_functional_test.bin", "rb");
uint8_t buffer[65536];
size_t bytesRead = fread(buffer, 1, sizeof(buffer), fp);
fclose(fp);

// Copy loaded data into emulator memory using the public API
if (bytesRead > 0) {
    mem.writeBlock(0x0000, buffer, bytesRead);
}

// Set PC to start address
cpu.PC = 0x0400;

// Run the test
while (cpu.PC != 0x3469) {  // Success address
    cpu.execute();
    // Add timeout check to detect failures
}
```

### Interpreting Results

**Success:**
- PC reaches 0x3469 and loops there indefinitely

**Failure:**
- PC loops at any other address
- Check the listing file to see what instruction failed
- The looping address indicates the test that failed

## Adding More Test Programs

To add additional test programs:

1. Download the test binary and supporting files
2. Place them in this directory
3. Create hexdump files for verification:
   ```bash
   hexdump -C testfile.bin | head -100 > testfile.hex
   xxd testfile.bin | head -100 > testfile.xxd
   ```
4. Update this README with test details

## Verification

To verify the test files are correct:

```bash
# Check file sizes
ls -lh *.bin

# Generate fresh hexdumps and compare
hexdump -C 6502_functional_test.bin | head -100 > verify.hex
diff verify.hex 6502_functional_test.hex
```

## References

- Klaus Dormann's 6502 test suite: https://github.com/Klaus2m5/6502_65C02_functional_tests
- 6502 documentation: http://www.6502.org/
- Visual 6502: http://visual6502.org/
