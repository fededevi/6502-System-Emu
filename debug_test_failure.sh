#!/bin/bash

# Script to help debug test failures by looking up the failure address in the listing

if [ $# -eq 0 ]; then
    echo "Usage: $0 <hex_address>"
    echo "Example: $0 0x674"
    echo ""
    echo "This script looks up a failure address in the test listing file"
    echo "to help you understand what test failed."
    exit 1
fi

ADDRESS=$1
LISTING="test_programs/6502_functional_test.lst"

if [ ! -f "$LISTING" ]; then
    echo "Error: Listing file not found: $LISTING"
    exit 1
fi

# Remove 0x prefix if present and normalize to lowercase
ADDRESS=${ADDRESS#0x}
ADDRESS=${ADDRESS#0X}
ADDRESS=$(echo "$ADDRESS" | tr '[:upper:]' '[:lower:]')

echo "Looking for address $ADDRESS in the test listing..."
echo "=================================================="
echo ""

# Search for the address and show context (format is "ADDR : ")
RESULT=$(grep -n "^$ADDRESS :" "$LISTING")

if [ -n "$RESULT" ]; then
    LINE_NUM=$(echo "$RESULT" | cut -d: -f1)
    START=$((LINE_NUM - 5))
    END=$((LINE_NUM + 10))
    if [ $START -lt 1 ]; then
        START=1
    fi
    sed -n "${START},${END}p" "$LISTING"
else
    echo "Address $ADDRESS not found in exact format."
    echo ""
    echo "Trying a broader search..."
    grep -i -B 2 -A 5 "$ADDRESS" "$LISTING" | head -30
fi
