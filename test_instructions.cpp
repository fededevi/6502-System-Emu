#include <iostream>
#include <cassert>
#include "memory.h"
#include "cpu.h"

void testTransferInstructions() {
    Memory mem;
    CPU cpu(&mem);
    
    // Setup reset vector
    mem.write(0xFFFC, 0x00);
    mem.write(0xFFFD, 0x80);
    cpu.reset();
    
    // Test TAX (0xAA)
    mem.write(0x8000, 0xAA); // TAX
    cpu.A = 0x42;
    cpu.execute();
    assert(cpu.X == 0x42);
    std::cout << "TAX test passed" << std::endl;
    
    // Test TAY (0xA8)
    cpu.PC = 0x8000;
    mem.write(0x8000, 0xA8); // TAY
    cpu.A = 0x55;
    cpu.execute();
    assert(cpu.Y == 0x55);
    std::cout << "TAY test passed" << std::endl;
    
    // Test TXA (0x8A)
    cpu.PC = 0x8000;
    mem.write(0x8000, 0x8A); // TXA
    cpu.X = 0x66;
    cpu.execute();
    assert(cpu.A == 0x66);
    std::cout << "TXA test passed" << std::endl;
}

void testStackInstructions() {
    Memory mem;
    CPU cpu(&mem);
    
    mem.write(0xFFFC, 0x00);
    mem.write(0xFFFD, 0x80);
    cpu.reset();
    
    // Test PHA (0x48)
    mem.write(0x8000, 0x48); // PHA
    cpu.A = 0x77;
    Byte oldSP = cpu.SP;
    cpu.execute();
    assert(mem.read(0x100 + oldSP) == 0x77);
    assert(cpu.SP == oldSP - 1);
    std::cout << "PHA test passed" << std::endl;
}

void testLogicalInstructions() {
    Memory mem;
    CPU cpu(&mem);
    
    mem.write(0xFFFC, 0x00);
    mem.write(0xFFFD, 0x80);
    cpu.reset();
    
    // Test AND immediate (0x29)
    mem.write(0x8000, 0x29); // AND immediate
    mem.write(0x8001, 0x0F);
    cpu.A = 0xFF;
    cpu.execute();
    assert(cpu.A == 0x0F);
    std::cout << "AND immediate test passed" << std::endl;
    
    // Test ORA immediate (0x09)
    cpu.PC = 0x8000;
    mem.write(0x8000, 0x09); // ORA immediate
    mem.write(0x8001, 0xF0);
    cpu.A = 0x0F;
    cpu.execute();
    assert(cpu.A == 0xFF);
    std::cout << "ORA immediate test passed" << std::endl;
    
    // Test EOR immediate (0x49)
    cpu.PC = 0x8000;
    mem.write(0x8000, 0x49); // EOR immediate
    mem.write(0x8001, 0xFF);
    cpu.A = 0xAA;
    cpu.execute();
    assert(cpu.A == 0x55);
    std::cout << "EOR immediate test passed" << std::endl;
}

void testBranchInstructions() {
    Memory mem;
    CPU cpu(&mem);
    
    mem.write(0xFFFC, 0x00);
    mem.write(0xFFFD, 0x80);
    cpu.reset();
    
    // Test BEQ (0xF0) - branch when zero flag is set
    cpu.PC = 0x8000;
    mem.write(0x8000, 0xF0); // BEQ
    mem.write(0x8001, 0x05); // offset +5
    cpu.setZ(true);
    cpu.execute();
    assert(cpu.PC == 0x8007); // 0x8002 + 0x05
    std::cout << "BEQ test passed" << std::endl;
    
    // Test BNE (0xD0) - no branch when zero flag is set
    cpu.PC = 0x8000;
    mem.write(0x8000, 0xD0); // BNE
    mem.write(0x8001, 0x05); // offset +5
    cpu.setZ(true);
    cpu.execute();
    assert(cpu.PC == 0x8002); // No branch
    std::cout << "BNE test passed" << std::endl;
}

void testIncDecInstructions() {
    Memory mem;
    CPU cpu(&mem);
    
    mem.write(0xFFFC, 0x00);
    mem.write(0xFFFD, 0x80);
    cpu.reset();
    
    // Test INX (0xE8)
    cpu.PC = 0x8000;
    mem.write(0x8000, 0xE8); // INX
    cpu.X = 0x10;
    cpu.execute();
    assert(cpu.X == 0x11);
    std::cout << "INX test passed" << std::endl;
    
    // Test DEX (0xCA)
    cpu.PC = 0x8000;
    mem.write(0x8000, 0xCA); // DEX
    cpu.X = 0x10;
    cpu.execute();
    assert(cpu.X == 0x0F);
    std::cout << "DEX test passed" << std::endl;
    
    // Test INY (0xC8)
    cpu.PC = 0x8000;
    mem.write(0x8000, 0xC8); // INY
    cpu.Y = 0x20;
    cpu.execute();
    assert(cpu.Y == 0x21);
    std::cout << "INY test passed" << std::endl;
    
    // Test DEY (0x88)
    cpu.PC = 0x8000;
    mem.write(0x8000, 0x88); // DEY
    cpu.Y = 0x20;
    cpu.execute();
    assert(cpu.Y == 0x1F);
    std::cout << "DEY test passed" << std::endl;
}

void testFlagInstructions() {
    Memory mem;
    CPU cpu(&mem);
    
    mem.write(0xFFFC, 0x00);
    mem.write(0xFFFD, 0x80);
    cpu.reset();
    
    // Test CLC (0x18)
    cpu.PC = 0x8000;
    mem.write(0x8000, 0x18); // CLC
    cpu.setC(true);
    cpu.execute();
    assert(cpu.C() == 0);
    std::cout << "CLC test passed" << std::endl;
    
    // Test SEC (0x38)
    cpu.PC = 0x8000;
    mem.write(0x8000, 0x38); // SEC
    cpu.setC(false);
    cpu.execute();
    assert(cpu.C() == 1);
    std::cout << "SEC test passed" << std::endl;
}

int main() {
    std::cout << "Running basic instruction tests..." << std::endl;
    
    testTransferInstructions();
    testStackInstructions();
    testLogicalInstructions();
    testBranchInstructions();
    testIncDecInstructions();
    testFlagInstructions();
    
    std::cout << "\nAll tests passed!" << std::endl;
    return 0;
}
