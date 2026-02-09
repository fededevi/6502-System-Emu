#include <gtest/gtest.h>
#include "cpu.h"
#include "memory.h"

class CpuTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    CpuTest()
        : mem(Memory::randomMemory())
        , cpu(&mem)
    {};
    ~CpuTest(){};

};

TEST_F(CpuTest, resetTest) {
    cpu.reset();
    EXPECT_EQ(0, cpu.P);
    EXPECT_EQ(0, cpu.A);
    EXPECT_EQ(0, cpu.X);
    EXPECT_EQ(0, cpu.Y);
    EXPECT_EQ(mem.read16(0xFFFC), cpu.PC);
    EXPECT_EQ(0xFD, cpu.SP);
    EXPECT_EQ(7, cpu.cycles);
}

TEST_F(CpuTest, immediateTest) {
    cpu.reset();
    Word addr = cpu.PC;
    EXPECT_EQ(addr, cpu.immediate());
}

TEST_F(CpuTest, zeroPage) {
    cpu.reset();
    Byte addr = mem.read(cpu.PC);
    EXPECT_EQ(addr, cpu.zeroPage());
}

TEST_F(CpuTest, zeroPageX) {
    cpu.reset();
    Byte addr = mem.read(cpu.PC) + cpu.X;
    EXPECT_EQ(addr, cpu.zeroPageX());
}

TEST_F(CpuTest, zeroPageX_checkOverflow) {
    cpu.reset();
    cpu.X = 0xB7;
    mem.write(cpu.PC, 0xB7);
    Byte addr = Byte(0xB7 + 0xB7);
    EXPECT_EQ(addr, cpu.zeroPageX());
}

TEST_F(CpuTest, zeroPageY) {
    cpu.reset();
    Byte addr = mem.read(cpu.PC) + cpu.Y;
    EXPECT_EQ(addr, cpu.zeroPageY());
}

TEST_F(CpuTest, zeroPageY_checkOverflow) {
    cpu.reset();
    cpu.Y = 0xB7;
    mem.write(cpu.PC, 0xB7);
    Byte addr = Byte(0xB7 + 0xB7);
    EXPECT_EQ(addr, cpu.zeroPageY());
}

TEST_F(CpuTest, absolute) {
    cpu.reset();
    Word addr = mem.read16(cpu.PC);
    EXPECT_EQ(addr, cpu.absolute());
}

TEST_F(CpuTest, absoluteX) {
    cpu.reset();
    cpu.X = 0x10;
    Word base = mem.read16(cpu.PC);
    EXPECT_EQ((Word)(base + cpu.X), cpu.absoluteX());
}

TEST_F(CpuTest, absoluteY) {
    cpu.reset();
    cpu.Y = 0x20;
    Word base = mem.read16(cpu.PC);
    EXPECT_EQ((Word)(base + cpu.Y), cpu.absoluteY());
}

TEST_F(CpuTest, indirectX) {
    cpu.reset();
    cpu.X = 0x05;
    Byte zpAddr = mem.read(cpu.PC);
    Byte finalZP = zpAddr + cpu.X;
    Word target = mem.read16(finalZP);
    EXPECT_EQ(target, cpu.indirectX());
}

TEST_F(CpuTest, indirectY) {
    cpu.reset();
    cpu.Y = 0x08;
    Byte zpAddr = mem.read(cpu.PC);
    Word base = mem.read16(zpAddr);
    EXPECT_EQ((Word)(base + cpu.Y), cpu.indirectY());
}

TEST_F(CpuTest, testFlagN) {

        cpu.reset();
        cpu.setN(true);
        EXPECT_EQ((Byte)1, cpu.N());
        cpu.setN(false);
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(CpuTest, testFlagV) {

        cpu.reset();
        cpu.setV(true);
        EXPECT_EQ((Byte)1, cpu.V());
        cpu.setV(false);
        EXPECT_EQ((Byte)0, cpu.V());
}

TEST_F(CpuTest, testFlagZ) {

        cpu.reset();
        cpu.setZ(true);
        EXPECT_EQ((Byte)1, cpu.Z());
        cpu.setZ(false);
        EXPECT_EQ((Byte)0, cpu.Z());
}

TEST_F(CpuTest, testFlagC) {

        cpu.reset();
        cpu.setC(true);
        EXPECT_EQ((Byte)1, cpu.C());
        cpu.setC(false);
        EXPECT_EQ((Byte)0, cpu.C());
}

TEST_F(CpuTest, testFlagB) {

        cpu.reset();
        cpu.setB(true);
        EXPECT_EQ((Byte)1, cpu.B());
        cpu.setB(false);
        EXPECT_EQ((Byte)0, cpu.B());
}

TEST_F(CpuTest, testFlagD) {

        cpu.reset();
        cpu.setD(true);
        EXPECT_EQ((Byte)1, cpu.D());
        cpu.setD(false);
        EXPECT_EQ((Byte)0, cpu.D());
}

TEST_F(CpuTest, testFlagI) {

        cpu.reset();
        cpu.setI(true);
        EXPECT_EQ((Byte)1, cpu.I());
        cpu.setI(false);
        EXPECT_EQ((Byte)0, cpu.I());
}

TEST_F(CpuTest, testPush) {

        cpu.reset();
        Byte initialSP = cpu.SP;
        cpu.push(0x42);
        EXPECT_EQ((Byte)(initialSP - 1), cpu.SP);
        EXPECT_EQ((Byte)0x42, mem.read(0x100 + initialSP));
}

TEST_F(CpuTest, testPushPop) {

        cpu.reset();
        cpu.push(0x55);
        cpu.SP++; // Simulate pop's SP increment
        Byte value = cpu.pop();
        EXPECT_EQ((Byte)0x55, value);
}
