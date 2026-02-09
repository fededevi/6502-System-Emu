#include <gtest/gtest.h>
#include "cpu.h"
#include "memory.h"

class FlagsTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    FlagsTest()
        : mem()
        , cpu(&mem)
    {};
    ~FlagsTest(){};

    void SetUp() override {

        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(FlagsTest, testCLC) {

        cpu.reset();
        mem.write(0x8000, 0x18);
        cpu.setC(true);
        cpu.execute();
        EXPECT_EQ((Byte)0, cpu.C());
}

TEST_F(FlagsTest, testSEC) {

        cpu.reset();
        mem.write(0x8000, 0x38);
        cpu.setC(false);
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.C());
}

TEST_F(FlagsTest, testCLI) {

        cpu.reset();
        mem.write(0x8000, 0x58);
        cpu.setI(true);
        cpu.execute();
        EXPECT_EQ((Byte)0, cpu.I());
}

TEST_F(FlagsTest, testSEI) {

        cpu.reset();
        mem.write(0x8000, 0x78);
        cpu.setI(false);
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.I());
}

TEST_F(FlagsTest, testCLV) {

        cpu.reset();
        mem.write(0x8000, 0xB8);
        cpu.setV(true);
        cpu.execute();
        EXPECT_EQ((Byte)0, cpu.V());
}

TEST_F(FlagsTest, testCLD) {

        cpu.reset();
        mem.write(0x8000, 0xD8);
        cpu.setD(true);
        cpu.execute();
        EXPECT_EQ((Byte)0, cpu.D());
}

TEST_F(FlagsTest, testSED) {

        cpu.reset();
        mem.write(0x8000, 0xF8);
        cpu.setD(false);
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.D());
}

TEST_F(FlagsTest, testMultipleFlags) {

        cpu.reset();
        mem.write(0x8000, 0x38); // SEC
        mem.write(0x8001, 0x78); // SEI
        mem.write(0x8002, 0xF8); // SED
        
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.C());
        
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.I());
        
        cpu.execute();
        EXPECT_EQ((Byte)1, cpu.D());
}
