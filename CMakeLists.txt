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
    cpu.cpp
    memory.cpp
)

set(EMULATOR_HEADERS
    cpu.h
    memory.h
    types.h
)

# Create a library with the emulator code
add_library(6502_emulator STATIC
    ${EMULATOR_SOURCES}
)

target_include_directories(6502_emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Main executable
add_executable(6502_emu main.cpp)
target_link_libraries(6502_emu PRIVATE 6502_emulator)

# Enable testing
enable_testing()
add_subdirectory(test)
