#include "constexpr_memory.h"
#include "constexpr_cpu_impl.h"
#include <iostream>
#include <chrono>

// Helper function to create a simple test program
constexpr ConstexprMemory createTestProgram() {
    ConstexprMemory mem;
    
    // Simple program: Load value into A, store it, then infinite loop
    // Address 0x8000: LDA #$42  (Load immediate value 0x42 into A)
    mem.mem[0x8000] = 0xA9;  // LDA immediate
    mem.mem[0x8001] = 0x42;  // Value to load
    
    // Address 0x8002: STA $0200 (Store A at address 0x0200)
    mem.mem[0x8002] = 0x8D;  // STA absolute
    mem.mem[0x8003] = 0x00;  // Low byte of address
    mem.mem[0x8004] = 0x02;  // High byte of address
    
    // Address 0x8005: JMP $8005 (Infinite loop)
    mem.mem[0x8005] = 0x4C;  // JMP absolute
    mem.mem[0x8006] = 0x05;  // Low byte
    mem.mem[0x8007] = 0x80;  // High byte
    
    // Set reset vector to point to 0x8000
    mem.mem[0xFFFC] = 0x00;  // Low byte
    mem.mem[0xFFFD] = 0x80;  // High byte
    
    return mem;
}

// Compile-time execution of the program
constexpr ConstexprCPUState compileTimeExecution() {
    ConstexprMemory mem = createTestProgram();
    ConstexprCPUState state(mem);
    state = ConstexprCPU::reset(state);
    state = ConstexprCPU::executeUntil(state, 1000);
    return state;
}

// This will be computed at compile time!
constexpr auto finalState = compileTimeExecution();

int main() {
    std::cout << "=== Constexpr 6502 Emulator Demo ===" << std::endl;
    std::cout << std::endl;
    std::cout << "Program executed at COMPILE TIME!" << std::endl;
    std::cout << std::endl;
    
    // Display the final state computed at compile time
    std::cout << "Final CPU State (computed at compile time):" << std::endl;
    std::cout << "  A  = 0x" << std::hex << static_cast<int>(finalState.A) << std::dec << std::endl;
    std::cout << "  X  = 0x" << std::hex << static_cast<int>(finalState.X) << std::dec << std::endl;
    std::cout << "  Y  = 0x" << std::hex << static_cast<int>(finalState.Y) << std::dec << std::endl;
    std::cout << "  PC = 0x" << std::hex << finalState.PC << std::dec << std::endl;
    std::cout << "  SP = 0x" << std::hex << static_cast<int>(finalState.SP) << std::dec << std::endl;
    std::cout << "  P  = 0x" << std::hex << static_cast<int>(finalState.P) << std::dec << std::endl;
    std::cout << "  Cycles = " << static_cast<long long>(finalState.cycles) << std::endl;
    std::cout << std::endl;
    
    // Verify the program wrote the expected value
    Byte storedValue = finalState.memory.read(0x0200);
    std::cout << "Value at 0x0200 = 0x" << std::hex << static_cast<int>(storedValue) << std::dec << std::endl;
    
    if (storedValue == 0x42 && finalState.A == 0x42) {
        std::cout << std::endl;
        std::cout << "SUCCESS! The program executed correctly at compile time." << std::endl;
        std::cout << "The memory state is now baked into the executable!" << std::endl;
    } else {
        std::cout << std::endl;
        std::cout << "ERROR: Unexpected result!" << std::endl;
        return 1;
    }
    
    std::cout << std::endl;
    std::cout << "=== Performance Comparison ===" << std::endl;
    std::cout << std::endl;
    
    // Now compare with runtime execution
    auto start = std::chrono::high_resolution_clock::now();
    
    ConstexprMemory runtimeMem = createTestProgram();
    ConstexprCPUState runtimeState(runtimeMem);
    runtimeState = ConstexprCPU::reset(runtimeState);
    runtimeState = ConstexprCPU::executeUntil(runtimeState, 1000);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    
    std::cout << "Runtime execution took: " << duration.count() << " nanoseconds" << std::endl;
    std::cout << "Compile-time execution took: 0 nanoseconds (computed at compile time!)" << std::endl;
    std::cout << std::endl;
    std::cout << "The compile-time version is infinitely faster as all computation" << std::endl;
    std::cout << "happens during compilation and the result is baked into the binary!" << std::endl;
    
    return 0;
}
