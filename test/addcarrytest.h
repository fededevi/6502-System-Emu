#pragma once

#include <QtTest>
#include "../cpu.h"
#include "../memory.h"

class AddCarryTest : public QObject
{
    Q_OBJECT

    Memory mem;
    CPU cpu;

public:
    AddCarryTest()
        : mem()
        , cpu(&mem)
    {};
    ~AddCarryTest(){};

private slots:
    void initTestCase() {
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }

    // Basic ADC tests
    void testADCImmediateBasic() {
        cpu.reset();
        cpu.A = 0x10;
        cpu.setC(false);
        mem.write(0x8000, 0x69); // ADC Immediate (ADCI)
        mem.write(0x8001, 0x20);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x30);
        QCOMPARE(cpu.C(), (Byte)0); // No carry
        QCOMPARE(cpu.Z(), (Byte)0); // Not zero
        QCOMPARE(cpu.N(), (Byte)0); // Not negative
    }

    void testADCImmediateWithCarry() {
        cpu.reset();
        cpu.A = 0x10;
        cpu.setC(true); // Set carry flag
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x20);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x31); // 0x10 + 0x20 + 1
        QCOMPARE(cpu.C(), (Byte)0);
    }

    void testADCImmediateCarryOut() {
        cpu.reset();
        cpu.A = 0xFF;
        cpu.setC(false);
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x01);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x00); // Wraps around
        QCOMPARE(cpu.C(), (Byte)1); // Carry set
        QCOMPARE(cpu.Z(), (Byte)1); // Zero flag set
    }

    void testADCImmediateNegativeFlag() {
        cpu.reset();
        cpu.A = 0x50;
        cpu.setC(false);
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x50);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0xA0); // 0x50 + 0x50 = 0xA0
        QCOMPARE(cpu.N(), (Byte)1); // Negative flag set (bit 7 is 1)
    }

    void testADCImmediateOverflow() {
        cpu.reset();
        cpu.A = 0x50; // Positive
        cpu.setC(false);
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x50); // Positive
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0xA0); // Result appears negative
        QCOMPARE(cpu.V(), (Byte)1); // Overflow flag set
    }

    void testADCImmediateNoOverflow() {
        cpu.reset();
        cpu.A = 0x50; // Positive
        cpu.setC(false);
        mem.write(0x8000, 0x69); // ADC Immediate
        mem.write(0x8001, 0x10); // Positive, result positive
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x60);
        QCOMPARE(cpu.V(), (Byte)0); // No overflow
    }

    // Test other addressing modes
    void testADCZeroPage() {
        cpu.reset();
        cpu.A = 0x15;
        cpu.setC(false);
        mem.write(0x8000, 0x65); // ADC Zero Page (ADCZ)
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x25);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x3A);
    }

    void testADCZeroPageX() {
        cpu.reset();
        cpu.A = 0x20;
        cpu.X = 0x05;
        cpu.setC(false);
        mem.write(0x8000, 0x75); // ADC Zero Page,X (ADCZX)
        mem.write(0x8001, 0x10);
        mem.write(0x0015, 0x30);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x50);
    }

    void testADCAbsolute() {
        cpu.reset();
        cpu.A = 0x40;
        cpu.setC(false);
        mem.write(0x8000, 0x6D); // ADC Absolute (ADCA)
        mem.write(0x8001, 0x34);
        mem.write(0x8002, 0x12);
        mem.write(0x1234, 0x05);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x45);
    }

    void testADCAbsoluteX() {
        cpu.reset();
        cpu.A = 0x11;
        cpu.X = 0x02;
        cpu.setC(false);
        mem.write(0x8000, 0x7D); // ADC Absolute,X (ADCAX)
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2002, 0x22);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x33);
    }

    void testADCAbsoluteY() {
        cpu.reset();
        cpu.A = 0x12;
        cpu.Y = 0x03;
        cpu.setC(false);
        mem.write(0x8000, 0x79); // ADC Absolute,Y (ADCAY)
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x30);
        mem.write(0x3003, 0x23);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x35);
    }

    void testADCIndirectX() {
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
        QCOMPARE(cpu.A, (Byte)0x37);
    }

    void testADCIndirectY() {
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
        QCOMPARE(cpu.A, (Byte)0x39);
    }
};
