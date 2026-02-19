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

# PGO support: OFF / GENERATE / USE
set(PGO_MODE "OFF" CACHE STRING "Profile-Guided Optimization mode: OFF, GENERATE, USE")
set_property(CACHE PGO_MODE PROPERTY STRINGS OFF GENERATE USE)

if(NOT PGO_MODE STREQUAL "OFF")
  message(STATUS "Configuring PGO mode: ${PGO_MODE}")
endif()

# Apply PGO flags per-compiler and per-mode to targets when requested
function(apply_pgo_flags target)
  if(PGO_MODE STREQUAL "GENERATE")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      target_compile_options(${target} PRIVATE -fprofile-generate)
      target_link_options(${target} PRIVATE -fprofile-generate)
    elseif(MSVC)
      # MSVC PGO generate (instrumented): enable /GL and instruct linker to instrument
      target_compile_options(${target} PRIVATE /GL)
      target_link_options(${target} PRIVATE /LTCG:PGInstrument)
    endif()
  elseif(PGO_MODE STREQUAL "USE")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      target_compile_options(${target} PRIVATE -fprofile-use -fprofile-correction)
      target_link_options(${target} PRIVATE -fprofile-use)
    elseif(MSVC)
      # MSVC PGO use: enable /GL and ask linker to use the collected profile
      target_compile_options(${target} PRIVATE /GL)
      target_link_options(${target} PRIVATE /LTCG:PGOptimize)
    endif()
  endif()
endfunction()

# Main executable (supports both direct execution and running binary programs)
add_executable(6502_emu src/main.cpp)
target_link_libraries(6502_emu PRIVATE 6502_emulator)

# Apply PGO flags to both targets so the workflow (GENERATE -> run -> USE) works
apply_pgo_flags(6502_emulator)
apply_pgo_flags(6502_emu)

# Enable Link-Time Optimization (LTO) / IPO when available — improves cross-TU inlining
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_ERROR)
if(IPO_SUPPORTED)
  message(STATUS "IPO / LTO supported: enabling interprocedural optimization")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  # Also set per-target property for explicitness
  set_target_properties(6502_emulator 6502_emu PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(STATUS "IPO / LTO not supported: ${IPO_ERROR}")
endif()

# Enable testing
enable_testing()
add_subdirectory(test)

# Add test program verification as a CTest target
add_test(NAME VerifyTestPrograms
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_programs/verify_tests.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_programs
)
