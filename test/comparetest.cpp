#include <gtest/gtest.h>
#include "../cpu.h"
#include "../memory.h"

class CompareTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    CompareTest()
        : mem()
        , cpu(&mem)
    {};
    ~CompareTest(){};

    void SetUp() override {

        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(CompareTest, testCMP_Immediate_Equal) {

        cpu.reset();
        mem.write(0x8000, 0xC9); // CMP immediate
        mem.write(0x8001, 0x42);
        cpu.A = 0x42;
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.Z()); // Equal
        EXPECT_EQ((Byte)1, cpu.C()); // A >= M
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(CompareTest, testCMP_Immediate_Greater) {

        cpu.reset();
        mem.write(0x8000, 0xC9);
        mem.write(0x8001, 0x10);
        cpu.A = 0x20;
        cpu.execute();
        EXPECT_EQ((Byte)0, cpu.Z()); // Not equal
        EXPECT_EQ((Byte)1, cpu.C()); // A >= M
        EXPECT_EQ((Byte)0, cpu.N()); // Result positive
}

TEST_F(CompareTest, testCMP_Immediate_Less) {

        cpu.reset();
        mem.write(0x8000, 0xC9);
        mem.write(0x8001, 0x50);
        cpu.A = 0x30;
        cpu.execute();
        EXPECT_EQ((Byte)0, cpu.Z()); // Not equal
        EXPECT_EQ((Byte)0, cpu.C()); // A < M
        EXPECT_EQ((Byte)1, cpu.N()); // Result negative
}

TEST_F(CompareTest, testCMP_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0xC5); // CMP zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x42);
        cpu.A = 0x42;
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.Z());
        EXPECT_EQ((Byte)1, cpu.C());
}

TEST_F(CompareTest, testCMP_Absolute) {

        cpu.reset();
        mem.write(0x8000, 0xCD); // CMP absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x80);
        cpu.A = 0x90;
        cpu.execute();
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)1, cpu.C()); // 0x90 >= 0x80
}

TEST_F(CompareTest, testCPX_Immediate_Equal) {

        cpu.reset();
        mem.write(0x8000, 0xE0); // CPX immediate
        mem.write(0x8001, 0x33);
        cpu.X = 0x33;
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.Z());
        EXPECT_EQ((Byte)1, cpu.C());
}

TEST_F(CompareTest, testCPX_Immediate_Less) {

        cpu.reset();
        mem.write(0x8000, 0xE0);
        mem.write(0x8001, 0x50);
        cpu.X = 0x30;
        cpu.execute();
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.C());
}

TEST_F(CompareTest, testCPX_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0xE4); // CPX zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x20);
        cpu.X = 0x30;
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.C()); // X >= M
}

TEST_F(CompareTest, testCPX_Absolute) {

        cpu.reset();
        mem.write(0x8000, 0xEC); // CPX absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x40);
        cpu.X = 0x40;
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.Z());
        EXPECT_EQ((Byte)1, cpu.C());
}

TEST_F(CompareTest, testCPY_Immediate_Equal) {

        cpu.reset();
        mem.write(0x8000, 0xC0); // CPY immediate
        mem.write(0x8001, 0x55);
        cpu.Y = 0x55;
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.Z());
        EXPECT_EQ((Byte)1, cpu.C());
}

TEST_F(CompareTest, testCPY_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0xC4); // CPY zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x60);
        cpu.Y = 0x50;
        cpu.execute();
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.C()); // Y < M
}

TEST_F(CompareTest, testCPY_Absolute) {

        cpu.reset();
        mem.write(0x8000, 0xCC); // CPY absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x30);
        cpu.Y = 0x40;
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.C()); // Y >= M
}
