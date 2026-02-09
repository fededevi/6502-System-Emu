#include <gtest/gtest.h>
#include "cpu.h"
#include "memory.h"

class StoreTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    StoreTest()
        : mem()
        , cpu(&mem)
    {};
    ~StoreTest(){};

    void SetUp() override {

        // Setup memory for tests
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(StoreTest, testSTAZeroPage) {

        cpu.reset();
        cpu.A = 0x42;
        mem.write(0x8000, 0x85); // STA Zero Page (STAZ)
        mem.write(0x8001, 0x10); // Zero page address
        cpu.execute();
        EXPECT_EQ((Byte)0x42, mem.read(0x0010));
}

TEST_F(StoreTest, testSTAZeroPageX) {

        cpu.reset();
        cpu.A = 0x55;
        cpu.X = 0x05;
        mem.write(0x8000, 0x95); // STA Zero Page,X (STAZX)
        mem.write(0x8001, 0x10); // Base address
        cpu.execute();
        EXPECT_EQ((Byte)0x55, mem.read(0x0015));
}

TEST_F(StoreTest, testSTAAbsolute) {

        cpu.reset();
        cpu.A = 0x77;
        mem.write(0x8000, 0x8D); // STA Absolute (STAA)
        mem.write(0x8001, 0x34);
        mem.write(0x8002, 0x12); // Address 0x1234
        cpu.execute();
        EXPECT_EQ((Byte)0x77, mem.read(0x1234));
}

TEST_F(StoreTest, testSTAAbsoluteX) {

        cpu.reset();
        cpu.A = 0x88;
        cpu.X = 0x02;
        mem.write(0x8000, 0x9D); // STA Absolute,X (STAAX)
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20); // Base 0x2000
        cpu.execute();
        EXPECT_EQ((Byte)0x88, mem.read(0x2002));
}

TEST_F(StoreTest, testSTAAbsoluteY) {

        cpu.reset();
        cpu.A = 0x99;
        cpu.Y = 0x03;
        mem.write(0x8000, 0x99); // STA Absolute,Y (STAAY)
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x30); // Base 0x3000
        cpu.execute();
        EXPECT_EQ((Byte)0x99, mem.read(0x3003));
}

TEST_F(StoreTest, testSTAIndirectX) {

        cpu.reset();
        cpu.A = 0xAA;
        cpu.X = 0x04;
        mem.write(0x8000, 0x81); // STA (Indirect,X) (STAIX)
        mem.write(0x8001, 0x20); // Base zero page address
        mem.write(0x0024, 0x00); // Low byte of target (0x20 + 0x04)
        mem.write(0x0025, 0x40); // High byte of target
        cpu.execute();
        EXPECT_EQ((Byte)0xAA, mem.read(0x4000));
}

TEST_F(StoreTest, testSTAIndirectY) {

        cpu.reset();
        cpu.A = 0xBB;
        cpu.Y = 0x05;
        mem.write(0x8000, 0x91); // STA (Indirect),Y (STAIY)
        mem.write(0x8001, 0x30); // Zero page address
        mem.write(0x0030, 0x00); // Low byte of base
        mem.write(0x0031, 0x50); // High byte of base (0x5000)
        cpu.execute();
        EXPECT_EQ((Byte)0xBB, mem.read(0x5005));
}

TEST_F(StoreTest, testSTXZeroPage) {

        cpu.reset();
        cpu.X = 0x33;
        mem.write(0x8000, 0x86); // STX Zero Page (STXZP)
        mem.write(0x8001, 0x20);
        cpu.execute();
        EXPECT_EQ((Byte)0x33, mem.read(0x0020));
}

TEST_F(StoreTest, testSTXZeroPageY) {

        cpu.reset();
        cpu.X = 0x44;
        cpu.Y = 0x03;
        mem.write(0x8000, 0x96); // STX Zero Page,Y (STXZPY)
        mem.write(0x8001, 0x20);
        cpu.execute();
        EXPECT_EQ((Byte)0x44, mem.read(0x0023));
}

TEST_F(StoreTest, testSTXAbsolute) {

        cpu.reset();
        cpu.X = 0x66;
        mem.write(0x8000, 0x8E); // STX Absolute (STXA)
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x60);
        cpu.execute();
        EXPECT_EQ((Byte)0x66, mem.read(0x6000));
}

TEST_F(StoreTest, testSTYZeroPage) {

        cpu.reset();
        cpu.Y = 0x22;
        mem.write(0x8000, 0x84); // STY Zero Page (STYZP)
        mem.write(0x8001, 0x30);
        cpu.execute();
        EXPECT_EQ((Byte)0x22, mem.read(0x0030));
}

TEST_F(StoreTest, testSTYZeroPageX) {

        cpu.reset();
        cpu.Y = 0x55;
        cpu.X = 0x04;
        mem.write(0x8000, 0x94); // STY Zero Page,X (STYZPX)
        mem.write(0x8001, 0x30);
        cpu.execute();
        EXPECT_EQ((Byte)0x55, mem.read(0x0034));
}

TEST_F(StoreTest, testSTYAbsolute) {

        cpu.reset();
        cpu.Y = 0x77;
        mem.write(0x8000, 0x8C); // STY Absolute (STYA)
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x70);
        cpu.execute();
        EXPECT_EQ((Byte)0x77, mem.read(0x7000));
}
