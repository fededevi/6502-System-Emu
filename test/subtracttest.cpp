#include <gtest/gtest.h>
#include "../cpu.h"
#include "../memory.h"

class SubtractTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    SubtractTest()
        : mem()
        , cpu(&mem)
    {};
    ~SubtractTest(){};

    void SetUp() override {

        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(SubtractTest, testSBC_Immediate_NoCarry) {

        cpu.reset();
        mem.write(0x8000, 0xE9); // SBC immediate
        mem.write(0x8001, 0x01);
        cpu.A = 0x05;
        cpu.setC(true); // No borrow
        cpu.execute();
        EXPECT_EQ((Byte)0x04, cpu.A);
        EXPECT_EQ((Byte)1, cpu.C()); // No borrow occurred
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(SubtractTest, testSBC_Immediate_WithBorrow) {

        cpu.reset();
        mem.write(0x8000, 0xE9);
        mem.write(0x8001, 0x01);
        cpu.A = 0x05;
        cpu.setC(false); // Borrow
        cpu.execute();
        EXPECT_EQ((Byte)0x03, cpu.A); // 5 - 1 - 1 = 3
        EXPECT_EQ((Byte)1, cpu.C()); // No borrow in result
}

TEST_F(SubtractTest, testSBC_Immediate_Zero) {

        cpu.reset();
        mem.write(0x8000, 0xE9);
        mem.write(0x8001, 0x42);
        cpu.A = 0x42;
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0x00, cpu.A);
        EXPECT_EQ((Byte)1, cpu.Z()); // Zero flag set
}

TEST_F(SubtractTest, testSBC_Immediate_Negative) {

        cpu.reset();
        mem.write(0x8000, 0xE9);
        mem.write(0x8001, 0x50);
        cpu.A = 0x30;
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0xE0, cpu.A); // 48 - 80 = -32 = 0xE0
        EXPECT_EQ((Byte)1, cpu.N()); // Negative
        EXPECT_EQ((Byte)0, cpu.C()); // Borrow occurred
}

TEST_F(SubtractTest, testSBC_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0xE5); // SBC zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x20);
        cpu.A = 0x50;
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0x30, cpu.A); // 80 - 32 = 48
        EXPECT_EQ((Byte)1, cpu.C());
}

TEST_F(SubtractTest, testSBC_ZeroPageX) {

        cpu.reset();
        mem.write(0x8000, 0xF5); // SBC zero page,X
        mem.write(0x8001, 0x10);
        mem.write(0x0015, 0x10);
        cpu.A = 0x30;
        cpu.X = 0x05;
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0x20, cpu.A);
}

TEST_F(SubtractTest, testSBC_Absolute) {

        cpu.reset();
        mem.write(0x8000, 0xED); // SBC absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x15);
        cpu.A = 0x25;
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0x10, cpu.A); // 37 - 21 = 16
}

TEST_F(SubtractTest, testSBC_AbsoluteX) {

        cpu.reset();
        mem.write(0x8000, 0xFD); // SBC absolute,X
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2005, 0x05);
        cpu.A = 0x15;
        cpu.X = 0x05;
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0x10, cpu.A);
}

TEST_F(SubtractTest, testSBC_AbsoluteY) {

        cpu.reset();
        mem.write(0x8000, 0xF9); // SBC absolute,Y
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2003, 0x08);
        cpu.A = 0x18;
        cpu.Y = 0x03;
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0x10, cpu.A);
}

TEST_F(SubtractTest, testSBC_IndirectX) {

        cpu.reset();
        mem.write(0x8000, 0xE1); // SBC (indirect,X)
        mem.write(0x8001, 0x10);
        mem.write(0x0015, 0x00);
        mem.write(0x0016, 0x20);
        mem.write(0x2000, 0x12);
        cpu.A = 0x32;
        cpu.X = 0x05;
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0x20, cpu.A); // 50 - 18 = 32
}

TEST_F(SubtractTest, testSBC_IndirectY) {

        cpu.reset();
        mem.write(0x8000, 0xF1); // SBC (indirect),Y
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x00);
        mem.write(0x0011, 0x20);
        mem.write(0x2004, 0x14);
        cpu.A = 0x34;
        cpu.Y = 0x04;
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0x20, cpu.A); // 52 - 20 = 32
}

TEST_F(SubtractTest, testSBCDecimalModeBasic) {

        cpu.reset();
        cpu.A = 0x50; // 50 in BCD
        cpu.setC(true); // No borrow
        cpu.setD(true); // Enable decimal mode
        mem.write(0x8000, 0xE9); // SBC Immediate
        mem.write(0x8001, 0x25); // 25 in BCD
        cpu.execute();
        EXPECT_EQ((Byte)0x25, cpu.A); // 50 - 25 = 25 in BCD
        EXPECT_EQ((Byte)1, cpu.C()); // No borrow
}

TEST_F(SubtractTest, testSBCDecimalModeWithBorrow) {

        cpu.reset();
        cpu.A = 0x50; // 50 in BCD
        cpu.setC(false); // Borrow
        cpu.setD(true);
        mem.write(0x8000, 0xE9); // SBC Immediate
        mem.write(0x8001, 0x25); // 25 in BCD
        cpu.execute();
        EXPECT_EQ((Byte)0x24, cpu.A); // 50 - 25 - 1 = 24 in BCD
        EXPECT_EQ((Byte)1, cpu.C());
}

TEST_F(SubtractTest, testSBCDecimalModeNeedsBorrow) {

        cpu.reset();
        cpu.A = 0x32; // 32 in BCD
        cpu.setC(true);
        cpu.setD(true);
        mem.write(0x8000, 0xE9); // SBC Immediate
        mem.write(0x8001, 0x04); // 4 in BCD
        cpu.execute();
        EXPECT_EQ((Byte)0x28, cpu.A); // 32 - 4 = 28 in BCD
        EXPECT_EQ((Byte)1, cpu.C());
}

TEST_F(SubtractTest, testSBCDecimalModeZeroResult) {

        cpu.reset();
        cpu.A = 0x25; // 25 in BCD
        cpu.setC(true);
        cpu.setD(true);
        mem.write(0x8000, 0xE9); // SBC Immediate
        mem.write(0x8001, 0x25); // 25 in BCD
        cpu.execute();
        EXPECT_EQ((Byte)0x00, cpu.A); // 25 - 25 = 0
        EXPECT_EQ((Byte)1, cpu.Z()); // Zero flag set
        EXPECT_EQ((Byte)1, cpu.C());
}

TEST_F(SubtractTest, testSBCDecimalModeCrossNibble) {

        cpu.reset();
        cpu.A = 0x46; // 46 in BCD
        cpu.setC(true);
        cpu.setD(true);
        mem.write(0x8000, 0xE9); // SBC Immediate
        mem.write(0x8001, 0x08); // 8 in BCD
        cpu.execute();
        EXPECT_EQ((Byte)0x38, cpu.A); // 46 - 8 = 38 in BCD
        EXPECT_EQ((Byte)1, cpu.C());
}

TEST_F(SubtractTest, testSBCDecimalModeZeroPage) {

        cpu.reset();
        cpu.A = 0x75; // 75 in BCD
        cpu.setC(true);
        cpu.setD(true);
        mem.write(0x8000, 0xE5); // SBC Zero Page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x30); // 30 in BCD
        cpu.execute();
        EXPECT_EQ((Byte)0x45, cpu.A); // 75 - 30 = 45 in BCD
        EXPECT_EQ((Byte)1, cpu.C());
}
