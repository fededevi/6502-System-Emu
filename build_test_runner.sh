#!/bin/bash

# Build script for test runner

echo "Building 6502 test runner..."

# Check if qmake6 is available
if command -v qmake6 &> /dev/null; then
    QMAKE=qmake6
elif command -v qmake &> /dev/null; then
    QMAKE=qmake
else
    echo "Error: qmake not found. Please install Qt development tools."
    exit 1
fi

# Build the test runner
$QMAKE test_runner.pro
make

if [ $? -eq 0 ]; then
    echo ""
    echo "Build successful!"
    echo "Run the tests with: ./test_runner"
else
    echo ""
    echo "Build failed!"
    exit 1
fi
