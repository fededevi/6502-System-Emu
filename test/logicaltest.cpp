#include <gtest/gtest.h>
#include "cpu.h"
#include "memory.h"

class LogicalTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    LogicalTest()
        : mem()
        , cpu(&mem)
    {};
    ~LogicalTest(){};

    void SetUp() override {

        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(LogicalTest, testAND_Immediate) {

        cpu.reset();
        mem.write(0x8000, 0x29); // AND immediate
        mem.write(0x8001, 0x0F);
        cpu.A = 0xFF;
        cpu.execute();
        EXPECT_EQ((Byte)0x0F, cpu.A);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(LogicalTest, testAND_ZeroResult) {

        cpu.reset();
        mem.write(0x8000, 0x29);
        mem.write(0x8001, 0x00);
        cpu.A = 0xFF;
        cpu.execute();
        EXPECT_EQ((Byte)0x00, cpu.A);
        EXPECT_EQ((Byte)1, cpu.Z());
}

TEST_F(LogicalTest, testAND_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0x25); // AND zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x55);
        cpu.A = 0xAA;
        cpu.execute();
        EXPECT_EQ((Byte)0x00, cpu.A);
}

TEST_F(LogicalTest, testAND_Absolute) {

        cpu.reset();
        mem.write(0x8000, 0x2D); // AND absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0xF0);
        cpu.A = 0xFF;
        cpu.execute();
        EXPECT_EQ((Byte)0xF0, cpu.A);
}

TEST_F(LogicalTest, testORA_Immediate) {

        cpu.reset();
        mem.write(0x8000, 0x09); // ORA immediate
        mem.write(0x8001, 0xF0);
        cpu.A = 0x0F;
        cpu.execute();
        EXPECT_EQ((Byte)0xFF, cpu.A);
        EXPECT_EQ((Byte)1, cpu.N());
}

TEST_F(LogicalTest, testORA_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0x05); // ORA zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x55);
        cpu.A = 0xAA;
        cpu.execute();
        EXPECT_EQ((Byte)0xFF, cpu.A);
}

TEST_F(LogicalTest, testORA_Absolute) {

        cpu.reset();
        mem.write(0x8000, 0x0D); // ORA absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x0F);
        cpu.A = 0x00;
        cpu.execute();
        EXPECT_EQ((Byte)0x0F, cpu.A);
}

TEST_F(LogicalTest, testEOR_Immediate) {

        cpu.reset();
        mem.write(0x8000, 0x49); // EOR immediate
        mem.write(0x8001, 0xFF);
        cpu.A = 0xAA;
        cpu.execute();
        EXPECT_EQ((Byte)0x55, cpu.A);
}

TEST_F(LogicalTest, testEOR_ZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0x45); // EOR zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x0F);
        cpu.A = 0xFF;
        cpu.execute();
        EXPECT_EQ((Byte)0xF0, cpu.A);
}

TEST_F(LogicalTest, testEOR_SelfCancel) {

        cpu.reset();
        mem.write(0x8000, 0x49); // EOR immediate
        mem.write(0x8001, 0x42);
        cpu.A = 0x42;
        cpu.execute();
        EXPECT_EQ((Byte)0x00, cpu.A);
        EXPECT_EQ((Byte)1, cpu.Z());
}
