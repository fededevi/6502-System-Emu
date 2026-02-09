#include <gtest/gtest.h>
#include "cpu.h"
#include "memory.h"

class ShiftsTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    ShiftsTest()
        : mem()
        , cpu(&mem)
    {};
    ~ShiftsTest(){};

    void SetUp() override {

        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(ShiftsTest, testASL_Accumulator) {

        cpu.reset();
        mem.write(0x8000, 0x0A); // ASL A
        cpu.A = 0x42;
        cpu.execute();
        EXPECT_EQ((Byte)0x84, cpu.A);
        EXPECT_EQ((Byte)0, cpu.C()); // No carry out
        EXPECT_EQ((Byte)1, cpu.N()); // Bit 7 set
}

TEST_F(ShiftsTest, testASL_Accumulator_Carry) {

        cpu.reset();
        mem.write(0x8000, 0x0A);
        cpu.A = 0x81;
        cpu.execute();
        EXPECT_EQ((Byte)0x02, cpu.A);
        EXPECT_EQ((Byte)1, cpu.C()); // Bit 7 shifted to carry
}

TEST_F(ShiftsTest, testASL_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0x06); // ASL zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x55);
        cpu.execute();
        EXPECT_EQ((Byte)0xAA, mem.read(0x0010));
        EXPECT_EQ((Byte)0, cpu.C());
}

TEST_F(ShiftsTest, testLSR_Accumulator) {

        cpu.reset();
        mem.write(0x8000, 0x4A); // LSR A
        cpu.A = 0x42;
        cpu.execute();
        EXPECT_EQ((Byte)0x21, cpu.A);
        EXPECT_EQ((Byte)0, cpu.C());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(ShiftsTest, testLSR_Accumulator_Carry) {

        cpu.reset();
        mem.write(0x8000, 0x4A);
        cpu.A = 0x81;
        cpu.execute();
        EXPECT_EQ((Byte)0x40, cpu.A);
        EXPECT_EQ((Byte)1, cpu.C()); // Bit 0 shifted to carry
}

TEST_F(ShiftsTest, testLSR_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0x46); // LSR zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0xAA);
        cpu.execute();
        EXPECT_EQ((Byte)0x55, mem.read(0x0010));
}

TEST_F(ShiftsTest, testROL_Accumulator) {

        cpu.reset();
        mem.write(0x8000, 0x2A); // ROL A
        cpu.A = 0x42;
        cpu.setC(false);
        cpu.execute();
        EXPECT_EQ((Byte)0x84, cpu.A);
        EXPECT_EQ((Byte)0, cpu.C());
}

TEST_F(ShiftsTest, testROL_Accumulator_WithCarry) {

        cpu.reset();
        mem.write(0x8000, 0x2A);
        cpu.A = 0x42;
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0x85, cpu.A); // Carry rotated into bit 0
}

TEST_F(ShiftsTest, testROL_Accumulator_CarryOut) {

        cpu.reset();
        mem.write(0x8000, 0x2A);
        cpu.A = 0x81;
        cpu.setC(false);
        cpu.execute();
        EXPECT_EQ((Byte)0x02, cpu.A);
        EXPECT_EQ((Byte)1, cpu.C()); // Bit 7 rotated to carry
}

TEST_F(ShiftsTest, testROL_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0x26); // ROL zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x40);
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0x81, mem.read(0x0010));
}

TEST_F(ShiftsTest, testROR_Accumulator) {

        cpu.reset();
        mem.write(0x8000, 0x6A); // ROR A
        cpu.A = 0x42;
        cpu.setC(false);
        cpu.execute();
        EXPECT_EQ((Byte)0x21, cpu.A);
        EXPECT_EQ((Byte)0, cpu.C());
}

TEST_F(ShiftsTest, testROR_Accumulator_WithCarry) {

        cpu.reset();
        mem.write(0x8000, 0x6A);
        cpu.A = 0x42;
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0xA1, cpu.A); // Carry rotated into bit 7
}

TEST_F(ShiftsTest, testROR_Accumulator_CarryOut) {

        cpu.reset();
        mem.write(0x8000, 0x6A);
        cpu.A = 0x81;
        cpu.setC(false);
        cpu.execute();
        EXPECT_EQ((Byte)0x40, cpu.A);
        EXPECT_EQ((Byte)1, cpu.C()); // Bit 0 rotated to carry
}

TEST_F(ShiftsTest, testROR_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0x66); // ROR zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x02);
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0x81, mem.read(0x0010));
}
