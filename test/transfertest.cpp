#include <gtest/gtest.h>
#include "cpu.h"
#include "memory.h"

class TransferTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    TransferTest()
        : mem()
        , cpu(&mem)
    {};
    ~TransferTest(){};

    void SetUp() override {

        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(TransferTest, testTAX) {

        cpu.reset();
        mem.write(0x8000, 0xAA);
        cpu.A = 0x42;
        cpu.execute();
        EXPECT_EQ((Byte)0x42, cpu.X);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(TransferTest, testTAX_Zero) {

        cpu.reset();
        mem.write(0x8000, 0xAA);
        cpu.A = 0x00;
        cpu.execute();
        EXPECT_EQ((Byte)0x00, cpu.X);
        EXPECT_EQ((Byte)1, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(TransferTest, testTAX_Negative) {

        cpu.reset();
        mem.write(0x8000, 0xAA);
        cpu.A = 0x80;
        cpu.execute();
        EXPECT_EQ((Byte)0x80, cpu.X);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)1, cpu.N());
}

TEST_F(TransferTest, testTAY) {

        cpu.reset();
        mem.write(0x8000, 0xA8);
        cpu.A = 0x55;
        cpu.execute();
        EXPECT_EQ((Byte)0x55, cpu.Y);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(TransferTest, testTXA) {

        cpu.reset();
        mem.write(0x8000, 0x8A);
        cpu.X = 0x33;
        cpu.execute();
        EXPECT_EQ((Byte)0x33, cpu.A);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(TransferTest, testTYA) {

        cpu.reset();
        mem.write(0x8000, 0x98);
        cpu.Y = 0x66;
        cpu.execute();
        EXPECT_EQ((Byte)0x66, cpu.A);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(TransferTest, testTSX) {

        cpu.reset();
        mem.write(0x8000, 0xBA);
        cpu.SP = 0xF0;
        cpu.execute();
        EXPECT_EQ((Byte)0xF0, cpu.X);
        EXPECT_EQ((Byte)1, cpu.N()); // 0xF0 has bit 7 set
}

TEST_F(TransferTest, testTXS) {

        cpu.reset();
        mem.write(0x8000, 0x9A);
        cpu.X = 0xA5;
        cpu.execute();
        EXPECT_EQ((Byte)0xA5, cpu.SP);
        // TXS doesn't affect flags
}
