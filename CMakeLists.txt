cmake_minimum_required(VERSION 3.14)

project(6502-System-Emu 
    VERSION 1.0.0
    DESCRIPTION "6502 CPU System Emulator"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for tools like clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define source files for the emulator library
set(EMULATOR_SOURCES
    src/core/cpu.cpp
    src/core/memory.cpp
)

set(EMULATOR_HEADERS
    src/core/cpu.h
    src/core/memory.h
    src/core/types.h
)

# Create a library with the emulator code
add_library(6502_emulator STATIC
    ${EMULATOR_SOURCES}
)

target_include_directories(6502_emulator PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
)

# Main executable (supports both direct execution and running binary programs)
add_executable(6502_emu src/main.cpp)
target_link_libraries(6502_emu PRIVATE 6502_emulator)

# Enable testing
enable_testing()
add_subdirectory(test)

# Add test program verification as a CTest target
add_test(NAME VerifyTestPrograms
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_programs/verify_tests.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_programs
)
