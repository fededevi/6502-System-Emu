#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "memory.h"
#include "cpu.h"

// Test runner for binary 6502 test programs
class TestRunner {
private:
    Memory mem;
    CPU cpu;
    unsigned long long maxCycles;
    
public:
    TestRunner() : cpu(&mem), maxCycles(100000000) {}
    
    bool loadBinary(const std::string& filename, Word startAddr = 0x0000) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return false;
        }
        
        // Determine file size
        file.seekg(0, std::ios::end);
        if (!file) {
            std::cerr << "Error: Failed to seek to end of file " << filename << std::endl;
            return false;
        }

        std::streampos endPos = file.tellg();
        if (endPos < 0) {
            std::cerr << "Error: Failed to determine size of file " << filename << std::endl;
            return false;
        }

        size_t size = static_cast<size_t>(endPos);
        file.seekg(0, std::ios::beg);
        if (!file) {
            std::cerr << "Error: Failed to seek to beginning of file " << filename << std::endl;
            return false;
        }

        // Ensure the binary will fit into memory starting at startAddr
        if (static_cast<size_t>(startAddr) + size > static_cast<size_t>(MEMORY_SIZE)) {
            std::cerr << "Error: Binary " << filename << " (" << size
                      << " bytes) does not fit in memory starting at 0x"
                      << std::hex << startAddr << std::dec << std::endl;
            return false;
        }

        std::cout << "Loading " << filename << " (" << size << " bytes) at 0x"
                  << std::hex << startAddr << std::dec << std::endl;

        // Read into temporary buffer then write to memory
        std::vector<Byte> buffer(size);
        if (size > 0) {
            file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(size));
            if (!file || static_cast<size_t>(file.gcount()) != size) {
                std::cerr << "Error: Failed to read full contents of file " << filename
                          << " (expected " << size << " bytes, got " << file.gcount() << ")" << std::endl;
                return false;
            }
        }
        file.close();
        
        // Use Memory's writeBlock method for better encapsulation
        mem.writeBlock(startAddr, buffer.data(), size);
        
        return true;
    }
    
    void setPC(Word addr) {
        cpu.PC = addr;
    }
    
    bool runUntilLoop(Word expectedSuccess = 0x3469) {
        std::cout << "Starting test execution at PC=0x" << std::hex << cpu.PC << std::dec << std::endl;
        std::cout << "Expected success address: 0x" << std::hex << expectedSuccess << std::dec << std::endl;
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        Word prevPC = cpu.PC;
        Word sameCount = 0;
        
        while (cpu.cycles < maxCycles) {
            cpu.execute();
            
            // Check if we're in a loop (same PC for multiple executions)
            if (cpu.PC == prevPC) {
                sameCount++;
                if (sameCount > 5) {  // Looping detected
                    std::cout << "\nLoop detected at PC=0x" << std::hex << cpu.PC << std::dec << std::endl;
                    
                    if (cpu.PC == expectedSuccess) {
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
                        
                        std::cout << "\n✓ TEST PASSED!" << std::endl;
                        std::cout << "Cycles executed: " << (unsigned long long)cpu.cycles << std::endl;
                        std::cout << "Time elapsed: " << duration.count() << " ms" << std::endl;
                        return true;
                    } else {
                        std::cout << "\n✗ TEST FAILED!" << std::endl;
                        std::cout << "Stopped at address: 0x" << std::hex << cpu.PC << std::dec << std::endl;
                        std::cout << "Expected to stop at: 0x" << std::hex << expectedSuccess << std::dec << std::endl;
                        std::cout << "Cycles executed: " << (unsigned long long)cpu.cycles << std::endl;
                        dumpCPUState();
                        return false;
                    }
                }
            } else {
                sameCount = 0;
            }
            prevPC = cpu.PC;
            
            // Progress indicator
            if (cpu.cycles % 1000000 == 0) {
                std::cout << "." << std::flush;
            }
        }
        
        std::cout << "\n✗ TEST TIMEOUT!" << std::endl;
        std::cout << "Exceeded maximum cycles (" << maxCycles << ")" << std::endl;
        dumpCPUState();
        return false;
    }
    
    void dumpCPUState() {
        std::cout << "\nCPU State:" << std::endl;
        std::cout << "  PC: 0x" << std::hex << cpu.PC << std::dec << std::endl;
        std::cout << "  SP: 0x" << std::hex << (int)cpu.SP << std::dec << std::endl;
        std::cout << "  A:  0x" << std::hex << (int)cpu.A << std::dec << std::endl;
        std::cout << "  X:  0x" << std::hex << (int)cpu.X << std::dec << std::endl;
        std::cout << "  Y:  0x" << std::hex << (int)cpu.Y << std::dec << std::endl;
        std::cout << "  P:  0x" << std::hex << (int)cpu.P << std::dec;
        std::cout << " [N:" << (int)cpu.N() << " V:" << (int)cpu.V() 
                  << " B:" << (int)cpu.B() << " D:" << (int)cpu.D()
                  << " I:" << (int)cpu.I() << " Z:" << (int)cpu.Z() 
                  << " C:" << (int)cpu.C() << "]" << std::endl;
    }
    
    void setMaxCycles(unsigned long long cycles) {
        maxCycles = cycles;
    }
};

int main(int argc, char* argv[]) {
    std::cout << "6502 Test Program Runner" << std::endl;
    std::cout << "=========================" << std::endl << std::endl;
    
    TestRunner runner;
    
    // Test 1: Klaus Dormann's Functional Test
    std::cout << "Running Klaus Dormann's 6502 Functional Test..." << std::endl;
    if (!runner.loadBinary("test_programs/6502_functional_test.bin", 0x0000)) {
        return 1;
    }
    
    runner.setPC(0x0400);  // Test starts at 0x0400
    bool success = runner.runUntilLoop(0x3469);  // Success loop at 0x3469
    
    if (!success) {
        std::cout << "\nNote: Check test_programs/6502_functional_test.lst for details" << std::endl;
        return 1;
    }
    
    std::cout << "\nAll tests completed successfully!" << std::endl;
    return 0;
}
