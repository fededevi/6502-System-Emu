#include <gtest/gtest.h>
#include "cpu.h"
#include "memory.h"

class IncDecTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    IncDecTest()
        : mem()
        , cpu(&mem)
    {};
    ~IncDecTest(){};

    void SetUp() override {

        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(IncDecTest, testINX) {

        cpu.reset();
        mem.write(0x8000, 0xE8);
        cpu.X = 0x10;
        cpu.execute();
        EXPECT_EQ((Byte)0x11, cpu.X);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(IncDecTest, testINX_Overflow) {

        cpu.reset();
        mem.write(0x8000, 0xE8);
        cpu.X = 0xFF;
        cpu.execute();
        EXPECT_EQ((Byte)0x00, cpu.X);
        EXPECT_EQ((Byte)1, cpu.Z());
}

TEST_F(IncDecTest, testINX_Negative) {

        cpu.reset();
        mem.write(0x8000, 0xE8);
        cpu.X = 0x7F;
        cpu.execute();
        EXPECT_EQ((Byte)0x80, cpu.X);
        EXPECT_EQ((Byte)1, cpu.N());
}

TEST_F(IncDecTest, testINY) {

        cpu.reset();
        mem.write(0x8000, 0xC8);
        cpu.Y = 0x20;
        cpu.execute();
        EXPECT_EQ((Byte)0x21, cpu.Y);
}

TEST_F(IncDecTest, testDEX) {

        cpu.reset();
        mem.write(0x8000, 0xCA);
        cpu.X = 0x10;
        cpu.execute();
        EXPECT_EQ((Byte)0x0F, cpu.X);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(IncDecTest, testDEX_Zero) {

        cpu.reset();
        mem.write(0x8000, 0xCA);
        cpu.X = 0x01;
        cpu.execute();
        EXPECT_EQ((Byte)0x00, cpu.X);
        EXPECT_EQ((Byte)1, cpu.Z());
}

TEST_F(IncDecTest, testDEX_Underflow) {

        cpu.reset();
        mem.write(0x8000, 0xCA);
        cpu.X = 0x00;
        cpu.execute();
        EXPECT_EQ((Byte)0xFF, cpu.X);
        EXPECT_EQ((Byte)1, cpu.N());
}

TEST_F(IncDecTest, testDEY) {

        cpu.reset();
        mem.write(0x8000, 0x88);
        cpu.Y = 0x20;
        cpu.execute();
        EXPECT_EQ((Byte)0x1F, cpu.Y);
}

TEST_F(IncDecTest, testINC_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0xE6);
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x42);
        cpu.execute();
        EXPECT_EQ((Byte)0x43, mem.read(0x0010));
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(IncDecTest, testINC_ZeroPageX) {

        cpu.reset();
        mem.write(0x8000, 0xF6);
        mem.write(0x8001, 0x10);
        mem.write(0x0015, 0x7F);
        cpu.X = 0x05;
        cpu.execute();
        EXPECT_EQ((Byte)0x80, mem.read(0x0015));
        EXPECT_EQ((Byte)1, cpu.N());
}

TEST_F(IncDecTest, testINC_Absolute) {

        cpu.reset();
        mem.write(0x8000, 0xEE);
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0xFF);
        cpu.execute();
        EXPECT_EQ((Byte)0x00, mem.read(0x2000));
        EXPECT_EQ((Byte)1, cpu.Z());
}

TEST_F(IncDecTest, testDEC_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0xC6);
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x42);
        cpu.execute();
        EXPECT_EQ((Byte)0x41, mem.read(0x0010));
}

TEST_F(IncDecTest, testDEC_ZeroPageX) {

        cpu.reset();
        mem.write(0x8000, 0xD6);
        mem.write(0x8001, 0x10);
        mem.write(0x0015, 0x01);
        cpu.X = 0x05;
        cpu.execute();
        EXPECT_EQ((Byte)0x00, mem.read(0x0015));
        EXPECT_EQ((Byte)1, cpu.Z());
}

TEST_F(IncDecTest, testDEC_Absolute) {

        cpu.reset();
        mem.write(0x8000, 0xCE);
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x00);
        cpu.execute();
        EXPECT_EQ((Byte)0xFF, mem.read(0x2000));
        EXPECT_EQ((Byte)1, cpu.N());
}
