# 6502-System-Emu

A 6502 CPU System Emulator with modern CMake build system.

## Building

### Prerequisites

- CMake 3.14 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)
- Qt5 (qtbase5-dev) for the test suite

### Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests
ctest --output-on-failure
```

### Running the Emulator

After building, run the emulator with:

```bash
./6502_emu
```

### Running Tests

The test suite uses Qt Test framework. Run tests using CTest:

```bash
cd build
ctest --output-on-failure
```

Or run the test executable directly:

```bash
./test/6502_tests
```

## Development

The project is organized as follows:

- `cpu.cpp`, `cpu.h` - CPU implementation
- `memory.cpp`, `memory.h` - Memory management
- `main.cpp` - Main emulator entry point
- `test/` - Test suite using Qt Test framework
- Various instruction implementation files (load.cpp, store.cpp, etc.)

### Building with Different Generators

CMake supports multiple build systems:

```bash
# Ninja (faster builds)
cmake -G Ninja ..
ninja

# Make with verbose output
cmake ..
make VERBOSE=1

# Visual Studio (Windows)
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
```

## Legacy Build System

The project previously used Qt's `.pro` files for building. These files are still present for backwards compatibility:

- `6502.pro` - Main application project file
- `test/test.pro` - Test suite project file

