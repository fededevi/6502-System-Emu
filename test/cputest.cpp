#include <gtest/gtest.h>
#include "../cpu.h"
#include "../memory.h"

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
