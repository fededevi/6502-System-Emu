#include <gtest/gtest.h>
#include "cpu.h"
#include "memory.h"

class MiscTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    MiscTest()
        : mem()
        , cpu(&mem)
    {};
    ~MiscTest(){};

    void SetUp() override {

        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(MiscTest, testNOP) {

        cpu.reset();
        mem.write(0x8000, 0xEA);
        Word oldPC = cpu.PC;
        Byte oldA = cpu.A = 0x42;
        Byte oldX = cpu.X = 0x55;
        cpu.execute();
        EXPECT_EQ((Word)(oldPC + 1), cpu.PC);
        EXPECT_EQ(oldA, cpu.A);
        EXPECT_EQ(oldX, cpu.X);
}

TEST_F(MiscTest, testJMP_Absolute) {

        cpu.reset();
        mem.write(0x8000, 0x4C); // JMP absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x90);
        cpu.execute();
        EXPECT_EQ((Word)0x9000, cpu.PC);
}

TEST_F(MiscTest, testJMP_Indirect) {

        cpu.reset();
        mem.write(0x8000, 0x6C); // JMP indirect
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x00);
        mem.write(0x2001, 0x30);
        cpu.execute();
        EXPECT_EQ((Word)0x3000, cpu.PC);
}

TEST_F(MiscTest, testJSR) {

        cpu.reset();
        mem.write(0x8000, 0x20); // JSR
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x90);
        Byte oldSP = cpu.SP;
        cpu.execute();
        EXPECT_EQ((Word)0x9000, cpu.PC);
        // Check return address on stack (PC-1)
        EXPECT_EQ((Byte)(oldSP - 2), cpu.SP);
}

TEST_F(MiscTest, testRTS) {

        cpu.reset();
        // Set up stack with return address 0x8005
        cpu.push(0x80);
        cpu.push(0x04);
        mem.write(0x8000, 0x60); // RTS
        cpu.execute();
        EXPECT_EQ((Word)0x8005, cpu.PC);
}

TEST_F(MiscTest, testBIT_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0x24); // BIT zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0xC0); // Bits 7 and 6 set
        cpu.A = 0xFF;
        cpu.execute();
        EXPECT_EQ((Byte)0, cpu.Z()); // Result not zero
        EXPECT_EQ((Byte)1, cpu.N()); // Bit 7 of memory
        EXPECT_EQ((Byte)1, cpu.V()); // Bit 6 of memory
}

TEST_F(MiscTest, testBIT_ZeroPage_ZeroResult) {

        cpu.reset();
        mem.write(0x8000, 0x24);
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x55);
        cpu.A = 0xAA;
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.Z()); // A & M = 0
}

TEST_F(MiscTest, testBIT_Absolute) {

        cpu.reset();
        mem.write(0x8000, 0x2C); // BIT absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x80); // Bit 7 set
        cpu.A = 0x80;
        cpu.execute();
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)1, cpu.N());
        EXPECT_EQ((Byte)0, cpu.V());
}
