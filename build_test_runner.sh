#!/bin/bash

# Build script for test runner using standalone Makefile

echo "Building 6502 test runner..."

# Build the test runner using the standalone Makefile
make -f Makefile.test_runner

if [ $? -eq 0 ]; then
    echo ""
    echo "Build successful!"
    echo "Run the tests with: ./test_runner"
else
    echo ""
    echo "Build failed!"
    exit 1
fi
