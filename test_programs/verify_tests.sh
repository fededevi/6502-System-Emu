#!/bin/bash

# Verification script for test programs

echo "Verifying test program integrity..."
echo ""

cd "$(dirname "$0")" || exit 1

# Check if the binary exists
if [ ! -f "6502_functional_test.bin" ]; then
    echo "✗ Error: 6502_functional_test.bin not found"
    exit 1
fi

# Check file size (should be exactly 64KB)
SIZE=$(stat -f%z "6502_functional_test.bin" 2>/dev/null || stat -c%s "6502_functional_test.bin" 2>/dev/null)

if [ -z "$SIZE" ]; then
    echo "✗ Error: Could not determine file size (stat command failed)"
    exit 1
fi

if [ "$SIZE" -eq 65536 ]; then
    echo "✓ Binary size correct: 65536 bytes (64KB)"
else
    echo "✗ Warning: Binary size is $SIZE bytes (expected 65536)"
fi

# Verify hexdump files exist
if [ -f "6502_functional_test.hex" ]; then
    echo "✓ Hexdump file exists: 6502_functional_test.hex"
else
    echo "✗ Warning: Hexdump file not found: 6502_functional_test.hex"
fi

if [ -f "6502_functional_test.xxd" ]; then
    echo "✓ XXD hexdump file exists: 6502_functional_test.xxd"
else
    echo "✗ Warning: XXD hexdump file not found: 6502_functional_test.xxd"
fi

# Verify listing file
if [ -f "6502_functional_test.lst" ]; then
    echo "✓ Listing file exists: 6502_functional_test.lst"
    LINES=$(wc -l < "6502_functional_test.lst")
    echo "  Listing contains $LINES lines"
else
    echo "✗ Warning: Listing file not found: 6502_functional_test.lst"
fi

# Show first few bytes
echo ""
echo "First 32 bytes of binary (hex):"
hexdump -C "6502_functional_test.bin" | head -3

echo ""
echo "Verification complete!"
