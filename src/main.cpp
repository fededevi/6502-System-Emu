#include "memory.h"
#include "cpu.h"
#include "display.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>

void printUsage(const char* progName) {
    std::cout << "Usage: " << progName << " [options]\n"
              << "Options:\n"
              << "  -f <file>       Load binary program from file\n"
              << "  -a <address>    Address to load program at (default: 0x0000, hex format)\n"
              << "  -pc <address>   Set program counter (default: from reset vector, hex format)\n"
              << "  -m <cycles>     Maximum cycles to execute (default: 100000000)\n"
              << "  -g              Enable graphical display mode\n"
              << "  -r <rate>       Display refresh rate in Hz (default: 30, only with -g)\n"
              << "  -h              Show this help message\n";
}

bool loadBinary(Memory& mem, const std::string& filename, Word startAddr = 0x0000) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    file.seekg(0, std::ios::end);
    std::streampos endPos = file.tellg();
    if (endPos < 0) {
        std::cerr << "Error: Failed to determine size of file " << filename << std::endl;
        return false;
    }
    
    size_t size = static_cast<size_t>(endPos);
    file.seekg(0, std::ios::beg);
    
    if (static_cast<size_t>(startAddr) + size > static_cast<size_t>(MEMORY_SIZE)) {
        std::cerr << "Error: Binary " << filename << " (" << size
                  << " bytes) does not fit in memory starting at 0x"
                  << std::hex << startAddr << std::dec << std::endl;
        return false;
    }
    
    std::cout << "Loading " << filename << " (" << size << " bytes) at 0x"
              << std::hex << startAddr << std::dec << std::endl;
    
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
    
    mem.writeBlock(startAddr, buffer.data(), size);
    return true;
}

int main(int argc, char* argv[]) {
    Memory mem;
    CPU cpu(&mem);
    Display display(&mem);
    
    std::string programFile;
    Word loadAddr = 0x0000;
    Word programCounter = 0xFFFF;  // Use reset vector by default
    unsigned long long maxCycles = 100000000;
    bool hasCustomPC = false;
    bool graphicsMode = false;
    int refreshRate = 30;  // Hz
    
    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-f" && i + 1 < argc) {
            programFile = argv[++i];
        } else if (arg == "-a" && i + 1 < argc) {
            loadAddr = static_cast<Word>(std::stoul(argv[++i], nullptr, 16));
        } else if (arg == "-pc" && i + 1 < argc) {
            programCounter = static_cast<Word>(std::stoul(argv[++i], nullptr, 16));
            hasCustomPC = true;
        } else if (arg == "-m" && i + 1 < argc) {
            maxCycles = std::stoull(argv[++i], nullptr, 0);
        } else if (arg == "-g") {
            graphicsMode = true;
        } else if (arg == "-r" && i + 1 < argc) {
            refreshRate = std::stoi(argv[++i]);
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // Load program if specified
    if (!programFile.empty()) {
        if (!loadBinary(mem, programFile, loadAddr)) {
            return 1;
        }
    }
    
    // Set up CPU and reset
    cpu.reset();
    
    // Override program counter if specified
    if (hasCustomPC) {
        cpu.PC = programCounter;
    } else if (!programFile.empty()) {
        // If a program was loaded but no PC specified, use the load address
        cpu.PC = loadAddr;
    }
    
    // Enable graphics mode if requested
    if (graphicsMode) {
        display.setEnabled(true);
        display.clear();
        std::cout << "Graphics mode enabled (32x32 display at $0200-$05FF)\n";
        std::cout << "Refresh rate: " << refreshRate << " Hz\n";
    }
    
    std::cout << "Starting execution at PC=0x" << std::hex << cpu.PC << std::dec << std::endl;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    auto lastDisplayUpdate = startTime;
    Word prevPC = cpu.PC;
    Word sameCount = 0;
    
    const int displayUpdateInterval = 1000000 / refreshRate;  // microseconds
    
    // Execute until max cycles or infinite loop detected
    while (cpu.cycles < maxCycles) {
        cpu.execute();
        
        // Update display at specified refresh rate
        if (graphicsMode) {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - lastDisplayUpdate);
            
            if (elapsed.count() >= displayUpdateInterval) {
                display.render();
                lastDisplayUpdate = now;
            }
        }
        
        // Detect infinite loops (PC not changing for multiple iterations)
        if (cpu.PC == prevPC) {
            sameCount++;
            if (sameCount > 5) {
                if (!graphicsMode) {
                    std::cout << "\nInfinite loop detected at PC=0x" << std::hex << cpu.PC << std::dec << std::endl;
                }
                break;
            }
        } else {
            sameCount = 0;
            prevPC = cpu.PC;
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Final display update
    if (graphicsMode) {
        display.render();
        // Move cursor below the display
        std::cout << "\n";
    }
    
    std::cout << "\nExecution completed:" << std::endl;
    std::cout << "  Cycles: " << static_cast<unsigned long long>(cpu.cycles) << std::endl;
    std::cout << "  Final PC: 0x" << std::hex << cpu.PC << std::dec << std::endl;
    std::cout << "  Time: " << duration.count() << " ms" << std::endl;
    
    return 0;
}
