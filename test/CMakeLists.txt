# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Define test sources
set(TEST_SOURCES
    main.cpp
    addcarrytest.cpp
    branchtest.cpp
    comparetest.cpp
    cputest.cpp
    flagstest.cpp
    incdectest.cpp
    loadtest.cpp
    logicaltest.cpp
    misctest.cpp
    shiftstest.cpp
    stacktest.cpp
    storetest.cpp
    subtracttest.cpp
    transfertest.cpp
)

# Create test executable
add_executable(6502_tests
    ${TEST_SOURCES}
)

# Link against Google Test and the emulator library
target_link_libraries(6502_tests
    PRIVATE
    GTest::gtest_main
    6502_emulator
)

# Enable Google Test integration with CTest
include(GoogleTest)
gtest_discover_tests(6502_tests)
