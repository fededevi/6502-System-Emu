#include <gtest/gtest.h>
#include "../cpu.h"
#include "../memory.h"

class AddCarryTest : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    AddCarryTest()
        : mem()
        , cpu(&mem)
    {};
    ~AddCarryTest(){};

    void SetUp() override {

        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }
};

TEST_F(AddCarryTest, testADCImmediateBasic) {

        cpu.reset();
        cpu.A = 0x10;
        cpu.setC(false);
        mem.write(0x8000, 0x69); // ADC Immediate (ADCI)
        mem.write(0x8001, 0x20);
        cpu.execute();
        EXPECT_EQ((Byte)0x30, cpu.A);
        EXPECT_EQ((Byte)0, cpu.C()); // No carry
        EXPECT_EQ((Byte)0, cpu.Z()); // Not zero
        EXPECT_EQ((Byte)0, cpu.N()); // Not negative
}

TEST_F(AddCarryTest, testADCImmediateWithCarry) {

        cpu.reset();
        cpu.A = 0x10;
        cpu.setC(true); // Set carry flag
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x20);
        cpu.execute();
        EXPECT_EQ((Byte)0x31, cpu.A); // 0x10 + 0x20 + 1
        EXPECT_EQ((Byte)0, cpu.C());
}

TEST_F(AddCarryTest, testADCImmediateCarryOut) {

        cpu.reset();
        cpu.A = 0xFF;
        cpu.setC(false);
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x01);
        cpu.execute();
        EXPECT_EQ((Byte)0x00, cpu.A); // Wraps around
        EXPECT_EQ((Byte)1, cpu.C()); // Carry set
        EXPECT_EQ((Byte)1, cpu.Z()); // Zero flag set
}

TEST_F(AddCarryTest, testADCImmediateNegativeFlag) {

        cpu.reset();
        cpu.A = 0x50;
        cpu.setC(false);
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x50);
        cpu.execute();
        EXPECT_EQ((Byte)0xA0, cpu.A); // 0x50 + 0x50 = 0xA0
        EXPECT_EQ((Byte)1, cpu.N()); // Negative flag set (bit 7 is 1)
}

TEST_F(AddCarryTest, testADCImmediateOverflow) {

        cpu.reset();
        cpu.A = 0x50; // Positive
        cpu.setC(false);
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x50); // Positive
        cpu.execute();
        EXPECT_EQ((Byte)0xA0, cpu.A); // Result appears negative
        EXPECT_EQ((Byte)1, cpu.V()); // Overflow flag set
}

TEST_F(AddCarryTest, testADCImmediateNoOverflow) {

        cpu.reset();
        cpu.A = 0x50; // Positive
        cpu.setC(false);
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x10); // Positive, result positive
        cpu.execute();
        EXPECT_EQ((Byte)0x60, cpu.A);
        EXPECT_EQ((Byte)0, cpu.V()); // No overflow
}

TEST_F(AddCarryTest, testADCZeroPage) {

        cpu.reset();
        cpu.A = 0x15;
        cpu.setC(false);
        mem.write(0x8000, 0x65); // ADC Zero Page (ADCZ)
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x25);
        cpu.execute();
        EXPECT_EQ((Byte)0x3A, cpu.A);
}

TEST_F(AddCarryTest, testADCZeroPageX) {

        cpu.reset();
        cpu.A = 0x20;
        cpu.X = 0x05;
        cpu.setC(false);
        mem.write(0x8000, 0x75); // ADC Zero Page,X (ADCZX)
        mem.write(0x8001, 0x10);
        mem.write(0x0015, 0x30);
        cpu.execute();
        EXPECT_EQ((Byte)0x50, cpu.A);
}

TEST_F(AddCarryTest, testADCAbsolute) {

        cpu.reset();
        cpu.A = 0x40;
        cpu.setC(false);
        mem.write(0x8000, 0x6D); // ADC Absolute (ADCA)
        mem.write(0x8001, 0x34);
        mem.write(0x8002, 0x12);
        mem.write(0x1234, 0x05);
        cpu.execute();
        EXPECT_EQ((Byte)0x45, cpu.A);
}

TEST_F(AddCarryTest, testADCAbsoluteX) {

        cpu.reset();
        cpu.A = 0x11;
        cpu.X = 0x02;
        cpu.setC(false);
        mem.write(0x8000, 0x7D); // ADC Absolute,X (ADCAX)
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2002, 0x22);
        cpu.execute();
        EXPECT_EQ((Byte)0x33, cpu.A);
}

TEST_F(AddCarryTest, testADCAbsoluteY) {

        cpu.reset();
        cpu.A = 0x12;
        cpu.Y = 0x03;
        cpu.setC(false);
        mem.write(0x8000, 0x79); // ADC Absolute,Y (ADCAY)
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x30);
        mem.write(0x3003, 0x23);
        cpu.execute();
        EXPECT_EQ((Byte)0x35, cpu.A);
}

TEST_F(AddCarryTest, testADCIndirectX) {

        cpu.reset();
        cpu.A = 0x13;
        cpu.X = 0x04;
        cpu.setC(false);
        mem.write(0x8000, 0x61); // ADC (Indirect,X) (ADCIX)
        mem.write(0x8001, 0x20);
        mem.write(0x0024, 0x00);
        mem.write(0x0025, 0x40);
        mem.write(0x4000, 0x24);
        cpu.execute();
        EXPECT_EQ((Byte)0x37, cpu.A);
}

TEST_F(AddCarryTest, testADCIndirectY) {

        cpu.reset();
        cpu.A = 0x14;
        cpu.Y = 0x05;
        cpu.setC(false);
        mem.write(0x8000, 0x71); // ADC (Indirect),Y (ADCIY)
        mem.write(0x8001, 0x30);
        mem.write(0x0030, 0x00);
        mem.write(0x0031, 0x50);
        mem.write(0x5005, 0x25);
        cpu.execute();
        EXPECT_EQ((Byte)0x39, cpu.A);
}

TEST_F(AddCarryTest, testADCDecimalModeBasic) {

        cpu.reset();
        cpu.A = 0x09; // 9 in BCD
        cpu.setC(false);
        cpu.setD(true); // Enable decimal mode
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x01); // 1 in BCD
        cpu.execute();
        EXPECT_EQ((Byte)0x10, cpu.A); // 9 + 1 = 10 in BCD
        EXPECT_EQ((Byte)0, cpu.C()); // No decimal carry
}

TEST_F(AddCarryTest, testADCDecimalModeWithCarry) {

        cpu.reset();
        cpu.A = 0x08; // 8 in BCD
        cpu.setC(true);
        cpu.setD(true);
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x05); // 5 in BCD
        cpu.execute();
        EXPECT_EQ((Byte)0x14, cpu.A); // 8 + 5 + 1 = 14 in BCD
        EXPECT_EQ((Byte)0, cpu.C());
}

TEST_F(AddCarryTest, testADCDecimalModeCarryOut) {

        cpu.reset();
        cpu.A = 0x99; // 99 in BCD
        cpu.setC(false);
        cpu.setD(true);
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x01); // 1 in BCD
        cpu.execute();
        EXPECT_EQ((Byte)0x00, cpu.A); // 99 + 1 = 100, wraps to 00
        EXPECT_EQ((Byte)1, cpu.C()); // Carry set (result > 99)
}

TEST_F(AddCarryTest, testADCDecimalModeMultipleDigits) {

        cpu.reset();
        cpu.A = 0x25; // 25 in BCD
        cpu.setC(false);
        cpu.setD(true);
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x34); // 34 in BCD
        cpu.execute();
        EXPECT_EQ((Byte)0x59, cpu.A); // 25 + 34 = 59 in BCD
        EXPECT_EQ((Byte)0, cpu.C());
}

TEST_F(AddCarryTest, testADCDecimalModeWithNibbleCarry) {

        cpu.reset();
        cpu.A = 0x19; // 19 in BCD
        cpu.setC(false);
        cpu.setD(true);
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x22); // 22 in BCD
        cpu.execute();
        EXPECT_EQ((Byte)0x41, cpu.A); // 19 + 22 = 41 in BCD
        EXPECT_EQ((Byte)0, cpu.C());
}

TEST_F(AddCarryTest, testADCDecimalModeZeroPage) {

        cpu.reset();
        cpu.A = 0x15; // 15 in BCD
        cpu.setC(false);
        cpu.setD(true);
        mem.write(0x8000, 0x65); // ADC Zero Page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x25); // 25 in BCD
        cpu.execute();
        EXPECT_EQ((Byte)0x40, cpu.A); // 15 + 25 = 40 in BCD
        EXPECT_EQ((Byte)0, cpu.C()); // No carry
}
