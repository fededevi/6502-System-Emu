#include <gtest/gtest.h>
#include "cpu.h"
#include "memory.h"

class BranchTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    BranchTest()
        : mem()
        , cpu(&mem)
    {};
    ~BranchTest(){};

    void SetUp() override {

        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(BranchTest, testBEQ_Taken) {

        cpu.reset();
        mem.write(0x8000, 0xF0); // BEQ
        mem.write(0x8001, 0x05); // offset +5
        cpu.setZ(true);
        cpu.execute();
        EXPECT_EQ((Word)0x8007, cpu.PC); // 0x8002 + 0x05
}

TEST_F(BranchTest, testBEQ_NotTaken) {

        cpu.reset();
        mem.write(0x8000, 0xF0);
        mem.write(0x8001, 0x05);
        cpu.setZ(false);
        cpu.execute();
        EXPECT_EQ((Word)0x8002, cpu.PC); // No branch
}

TEST_F(BranchTest, testBEQ_Backward) {

        cpu.reset();
        cpu.PC = 0x8010;
        mem.write(0x8010, 0xF0);
        mem.write(0x8011, 0xFA); // -6 in two's complement
        cpu.setZ(true);
        cpu.execute();
        EXPECT_EQ((Word)0x800C, cpu.PC); // 0x8012 + (-6)
}

TEST_F(BranchTest, testBNE_Taken) {

        cpu.reset();
        mem.write(0x8000, 0xD0);
        mem.write(0x8001, 0x10);
        cpu.setZ(false);
        cpu.execute();
        EXPECT_EQ((Word)0x8012, cpu.PC);
}

TEST_F(BranchTest, testBNE_NotTaken) {

        cpu.reset();
        mem.write(0x8000, 0xD0);
        mem.write(0x8001, 0x10);
        cpu.setZ(true);
        cpu.execute();
        EXPECT_EQ((Word)0x8002, cpu.PC);
}

TEST_F(BranchTest, testBCC_Taken) {

        cpu.reset();
        mem.write(0x8000, 0x90);
        mem.write(0x8001, 0x08);
        cpu.setC(false);
        cpu.execute();
        EXPECT_EQ((Word)0x800A, cpu.PC);
}

TEST_F(BranchTest, testBCC_NotTaken) {

        cpu.reset();
        mem.write(0x8000, 0x90);
        mem.write(0x8001, 0x08);
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Word)0x8002, cpu.PC);
}

TEST_F(BranchTest, testBCS_Taken) {

        cpu.reset();
        mem.write(0x8000, 0xB0);
        mem.write(0x8001, 0x04);
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Word)0x8006, cpu.PC);
}

TEST_F(BranchTest, testBMI_Taken) {

        cpu.reset();
        mem.write(0x8000, 0x30);
        mem.write(0x8001, 0x02);
        cpu.setN(true);
        cpu.execute();
        EXPECT_EQ((Word)0x8004, cpu.PC);
}

TEST_F(BranchTest, testBMI_NotTaken) {

        cpu.reset();
        mem.write(0x8000, 0x30);
        mem.write(0x8001, 0x02);
        cpu.setN(false);
        cpu.execute();
        EXPECT_EQ((Word)0x8002, cpu.PC);
}

TEST_F(BranchTest, testBPL_Taken) {

        cpu.reset();
        mem.write(0x8000, 0x10);
        mem.write(0x8001, 0x03);
        cpu.setN(false);
        cpu.execute();
        EXPECT_EQ((Word)0x8005, cpu.PC);
}

TEST_F(BranchTest, testBVC_Taken) {

        cpu.reset();
        mem.write(0x8000, 0x50);
        mem.write(0x8001, 0x07);
        cpu.setV(false);
        cpu.execute();
        EXPECT_EQ((Word)0x8009, cpu.PC);
}

TEST_F(BranchTest, testBVS_Taken) {

        cpu.reset();
        mem.write(0x8000, 0x70);
        mem.write(0x8001, 0x01);
        cpu.setV(true);
        cpu.execute();
        EXPECT_EQ((Word)0x8003, cpu.PC);
}
