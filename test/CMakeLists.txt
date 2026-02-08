# Find Qt5 Test module
find_package(Qt5 COMPONENTS Test REQUIRED)

# Enable automoc for Qt
set(CMAKE_AUTOMOC ON)

# Define test sources
set(TEST_SOURCES
    main.cpp
)

# Define test headers
set(TEST_HEADERS
    addcarrytest.h
    branchtest.h
    comparetest.h
    cputest.h
    flagstest.h
    incdectest.h
    loadtest.h
    logicaltest.h
    misctest.h
    shiftstest.h
    stacktest.h
    storetest.h
    subtracttest.h
    transfertest.h
)

# Create test executable
add_executable(6502_tests
    ${TEST_SOURCES}
    ${TEST_HEADERS}
)

# Link against Qt5::Test and the emulator library
target_link_libraries(6502_tests
    PRIVATE
    Qt5::Test
    6502_emulator
)

# Add test to CTest
add_test(NAME 6502_emulator_tests COMMAND 6502_tests)
