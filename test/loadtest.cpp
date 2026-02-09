#include <gtest/gtest.h>
#include "cpu.h"
#include "memory.h"

class LoadTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    LoadTest()
        : mem()
        , cpu(&mem)
    {};
    ~LoadTest(){};

    void SetUp() override {

        // Setup memory for tests
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(LoadTest, testLDAImmediate) {

        cpu.reset();
        mem.write(0x8000, 0xA9); // LDA Immediate (LDAI)
        mem.write(0x8001, 0x42); // Value to load
        cpu.execute();
        EXPECT_EQ((Byte)0x42, cpu.A);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(LoadTest, testLDAImmediateZeroFlag) {

        cpu.reset();
        mem.write(0x8000, 0xA9); // LDA Immediate (LDAI)
        mem.write(0x8001, 0x00); // Zero value
        cpu.execute();
        EXPECT_EQ((Byte)0x00, cpu.A);
        EXPECT_EQ((Byte)1, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(LoadTest, testLDAImmediateNegativeFlag) {

        cpu.reset();
        mem.write(0x8000, 0xA9); // LDA Immediate (LDAI)
        mem.write(0x8001, 0x80); // Negative value (bit 7 set)
        cpu.execute();
        EXPECT_EQ((Byte)0x80, cpu.A);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)1, cpu.N());
}

TEST_F(LoadTest, testLDAZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0xA5); // LDA Zero Page (LDAZ)
        mem.write(0x8001, 0x10); // Zero page address
        mem.write(0x0010, 0x55); // Value at zero page
        cpu.execute();
        EXPECT_EQ((Byte)0x55, cpu.A);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(LoadTest, testLDAZeroPageX) {

        cpu.reset();
        cpu.X = 0x05;
        mem.write(0x8000, 0xB5); // LDA Zero Page,X (LDAZX)
        mem.write(0x8001, 0x10); // Base address
        mem.write(0x0015, 0x77); // Value at 0x10 + 0x05
        cpu.execute();
        EXPECT_EQ((Byte)0x77, cpu.A);
}

TEST_F(LoadTest, testLDAAbsolute) {

        cpu.reset();
        mem.write(0x8000, 0xAD); // LDA Absolute (LDAA)
        mem.write(0x8001, 0x34); // Low byte of address
        mem.write(0x8002, 0x12); // High byte of address (0x1234)
        mem.write(0x1234, 0x99); // Value at absolute address
        cpu.execute();
        EXPECT_EQ((Byte)0x99, cpu.A);
}

TEST_F(LoadTest, testLDAAbsoluteX) {

        cpu.reset();
        cpu.X = 0x02;
        mem.write(0x8000, 0xBD); // LDA Absolute,X (LDAAX)
        mem.write(0x8001, 0x00); // Low byte
        mem.write(0x8002, 0x20); // High byte (0x2000)
        mem.write(0x2002, 0xAA); // Value at 0x2000 + 0x02
        cpu.execute();
        EXPECT_EQ((Byte)0xAA, cpu.A);
}

TEST_F(LoadTest, testLDAAbsoluteY) {

        cpu.reset();
        cpu.Y = 0x03;
        mem.write(0x8000, 0xB9); // LDA Absolute,Y (LDAAY)
        mem.write(0x8001, 0x00); // Low byte
        mem.write(0x8002, 0x30); // High byte (0x3000)
        mem.write(0x3003, 0xBB); // Value at 0x3000 + 0x03
        cpu.execute();
        EXPECT_EQ((Byte)0xBB, cpu.A);
}

TEST_F(LoadTest, testLDAIndirectX) {

        cpu.reset();
        cpu.X = 0x04;
        mem.write(0x8000, 0xA1); // LDA (Indirect,X) (LDAIX)
        mem.write(0x8001, 0x20); // Base address in zero page
        mem.write(0x0024, 0x00); // Low byte of target address (0x20 + 0x04)
        mem.write(0x0025, 0x40); // High byte of target address
        mem.write(0x4000, 0xCC); // Value at target
        cpu.execute();
        EXPECT_EQ((Byte)0xCC, cpu.A);
}

TEST_F(LoadTest, testLDAIndirectY) {

        cpu.reset();
        cpu.Y = 0x05;
        mem.write(0x8000, 0xB1); // LDA (Indirect),Y (LDAIY)
        mem.write(0x8001, 0x30); // Zero page address
        mem.write(0x0030, 0x00); // Low byte of base address
        mem.write(0x0031, 0x50); // High byte of base address (0x5000)
        mem.write(0x5005, 0xDD); // Value at 0x5000 + 0x05
        cpu.execute();
        EXPECT_EQ((Byte)0xDD, cpu.A);
}

TEST_F(LoadTest, testLDXImmediate) {

        cpu.reset();
        mem.write(0x8000, 0xA2); // LDX Immediate (LDXI)
        mem.write(0x8001, 0x33);
        cpu.execute();
        EXPECT_EQ((Byte)0x33, cpu.X);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(LoadTest, testLDXZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0xA6); // LDX Zero Page (LDXZP)
        mem.write(0x8001, 0x20);
        mem.write(0x0020, 0x44);
        cpu.execute();
        EXPECT_EQ((Byte)0x44, cpu.X);
}

TEST_F(LoadTest, testLDXZeroPageY) {

        cpu.reset();
        cpu.Y = 0x03;
        mem.write(0x8000, 0xB6); // LDX Zero Page,Y (LDXZPY)
        mem.write(0x8001, 0x20);
        mem.write(0x0023, 0x66);
        cpu.execute();
        EXPECT_EQ((Byte)0x66, cpu.X);
}

TEST_F(LoadTest, testLDXAbsolute) {

        cpu.reset();
        mem.write(0x8000, 0xAE); // LDX Absolute (LDXA)
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x60);
        mem.write(0x6000, 0x88);
        cpu.execute();
        EXPECT_EQ((Byte)0x88, cpu.X);
}

TEST_F(LoadTest, testLDXAbsoluteY) {

        cpu.reset();
        cpu.Y = 0x02;
        mem.write(0x8000, 0xBE); // LDX Absolute,Y (LDXAY)
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x70);
        mem.write(0x7002, 0x99);
        cpu.execute();
        EXPECT_EQ((Byte)0x99, cpu.X);
}

TEST_F(LoadTest, testLDYImmediate) {

        cpu.reset();
        mem.write(0x8000, 0xA0); // LDY Immediate (LDYI)
        mem.write(0x8001, 0x22);
        cpu.execute();
        EXPECT_EQ((Byte)0x22, cpu.Y);
        EXPECT_EQ((Byte)0, cpu.Z());
        EXPECT_EQ((Byte)0, cpu.N());
}

TEST_F(LoadTest, testLDYZeroPage) {

        cpu.reset();
        mem.write(0x8000, 0xA4); // LDY Zero Page (LDYZP)
        mem.write(0x8001, 0x30);
        mem.write(0x0030, 0x55);
        cpu.execute();
        EXPECT_EQ((Byte)0x55, cpu.Y);
}

TEST_F(LoadTest, testLDYZeroPageX) {

        cpu.reset();
        cpu.X = 0x04;
        mem.write(0x8000, 0xB4); // LDY Zero Page,X (note: function is named LDYZPY in source)
        mem.write(0x8001, 0x30);
        mem.write(0x0034, 0x77);
        cpu.execute();
        EXPECT_EQ((Byte)0x77, cpu.Y);
}

TEST_F(LoadTest, testLDYAbsolute) {

        cpu.reset();
        mem.write(0x8000, 0xAC); // LDY Absolute (LDYA)
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x81); // Address 0x8100
        mem.write(0x8100, 0xAA); // Value at 0x8100
        cpu.execute();
        EXPECT_EQ((Byte)0xAA, cpu.Y);
}

TEST_F(LoadTest, testLDYAbsoluteX) {

        cpu.reset();
        cpu.X = 0x05;
        mem.write(0x8000, 0xBC); // LDY Absolute,X (note: function is named LDYAY in source)
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x90);
        mem.write(0x9005, 0xBB);
        cpu.execute();
        EXPECT_EQ((Byte)0xBB, cpu.Y);
}
