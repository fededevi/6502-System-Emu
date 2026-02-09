#include <gtest/gtest.h>
#include "../cpu.h"
#include "../memory.h"

class StackTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    StackTest()
        : mem()
        , cpu(&mem)
    {};
    ~StackTest(){};

    void SetUp() override {

        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(StackTest, testPHA) {

        cpu.reset();
        mem.write(0x8000, 0x48);
        cpu.A = 0x77;
        Byte oldSP = cpu.SP;
        cpu.execute();
        EXPECT_EQ((Byte)0x77, mem.read(0x100 + oldSP));
        EXPECT_EQ((Byte)(oldSP - 1), cpu.SP);
}

TEST_F(StackTest, testPLA) {

        cpu.reset();
        mem.write(0x8000, 0x68);
        cpu.push(0x88);
        cpu.execute();
        EXPECT_EQ((Byte)0x88, cpu.A);
}

TEST_F(StackTest, testPLA_Zero) {

        cpu.reset();
        mem.write(0x8000, 0x68);
        cpu.push(0x00);
        cpu.execute();
        EXPECT_EQ((Byte)0x00, cpu.A);
        EXPECT_EQ((Byte)1, cpu.Z());
}

TEST_F(StackTest, testPLA_Negative) {

        cpu.reset();
        mem.write(0x8000, 0x68);
        cpu.push(0x80);
        cpu.execute();
        EXPECT_EQ((Byte)0x80, cpu.A);
        EXPECT_EQ((Byte)1, cpu.N());
}

TEST_F(StackTest, testPHP) {

        cpu.reset();
        mem.write(0x8000, 0x08);
        cpu.P = 0xC5;
        Byte oldSP = cpu.SP;
        cpu.execute();
        EXPECT_EQ((Byte)0xD5, mem.read(0x100 + oldSP)); // B flag is set when pushed
        EXPECT_EQ((Byte)(oldSP - 1), cpu.SP);
}

TEST_F(StackTest, testPLP) {

        cpu.reset();
        mem.write(0x8000, 0x28);
        cpu.push(0x42);
        cpu.execute();
        EXPECT_EQ((Byte)0x42, cpu.P);
}

TEST_F(StackTest, testPHAPLA) {

        cpu.reset();
        mem.write(0x8000, 0x48); // PHA
        mem.write(0x8001, 0x68); // PLA
        cpu.A = 0x99;
        cpu.execute();
        cpu.execute();
        EXPECT_EQ((Byte)0x99, cpu.A);
}
