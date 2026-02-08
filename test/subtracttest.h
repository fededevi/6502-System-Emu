#pragma once

#include <QtTest>
#include "../cpu.h"
#include "../memory.h"

class SubtractTest : public QObject
{
    Q_OBJECT

    Memory mem;
    CPU cpu;

public:
    SubtractTest()
        : mem()
        , cpu(&mem)
    {};
    ~SubtractTest(){};

private slots:
    void initTestCase() {
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }

    // SBC - Subtract with Carry
    void testSBC_Immediate_NoCarry() {
        cpu.reset();
        mem.write(0x8000, 0xE9); // SBC immediate
        mem.write(0x8001, 0x01);
        cpu.A = 0x05;
        cpu.setC(true); // No borrow
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x04);
        QCOMPARE(cpu.C(), (Byte)1); // No borrow occurred
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.N(), (Byte)0);
    }

    void testSBC_Immediate_WithBorrow() {
        cpu.reset();
        mem.write(0x8000, 0xE9);
        mem.write(0x8001, 0x01);
        cpu.A = 0x05;
        cpu.setC(false); // Borrow
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x03); // 5 - 1 - 1 = 3
        QCOMPARE(cpu.C(), (Byte)1); // No borrow in result
    }

    void testSBC_Immediate_Zero() {
        cpu.reset();
        mem.write(0x8000, 0xE9);
        mem.write(0x8001, 0x42);
        cpu.A = 0x42;
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x00);
        QCOMPARE(cpu.Z(), (Byte)1); // Zero flag set
    }

    void testSBC_Immediate_Negative() {
        cpu.reset();
        mem.write(0x8000, 0xE9);
        mem.write(0x8001, 0x50);
        cpu.A = 0x30;
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0xE0); // 48 - 80 = -32 = 0xE0
        QCOMPARE(cpu.N(), (Byte)1); // Negative
        QCOMPARE(cpu.C(), (Byte)0); // Borrow occurred
    }

    void testSBC_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0xE5); // SBC zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x20);
        cpu.A = 0x50;
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x30); // 80 - 32 = 48
        QCOMPARE(cpu.C(), (Byte)1);
    }

    void testSBC_ZeroPageX() {
        cpu.reset();
        mem.write(0x8000, 0xF5); // SBC zero page,X
        mem.write(0x8001, 0x10);
        mem.write(0x0015, 0x10);
        cpu.A = 0x30;
        cpu.X = 0x05;
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x20);
    }

    void testSBC_Absolute() {
        cpu.reset();
        mem.write(0x8000, 0xED); // SBC absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x15);
        cpu.A = 0x25;
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x10); // 37 - 21 = 16
    }

    void testSBC_AbsoluteX() {
        cpu.reset();
        mem.write(0x8000, 0xFD); // SBC absolute,X
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2005, 0x05);
        cpu.A = 0x15;
        cpu.X = 0x05;
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x10);
    }

    void testSBC_AbsoluteY() {
        cpu.reset();
        mem.write(0x8000, 0xF9); // SBC absolute,Y
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2003, 0x08);
        cpu.A = 0x18;
        cpu.Y = 0x03;
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x10);
    }

    void testSBC_IndirectX() {
        cpu.reset();
        mem.write(0x8000, 0xE1); // SBC (indirect,X)
        mem.write(0x8001, 0x10);
        mem.write(0x0015, 0x00);
        mem.write(0x0016, 0x20);
        mem.write(0x2000, 0x12);
        cpu.A = 0x32;
        cpu.X = 0x05;
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x20); // 50 - 18 = 32
    }

    void testSBC_IndirectY() {
        cpu.reset();
        mem.write(0x8000, 0xF1); // SBC (indirect),Y
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x00);
        mem.write(0x0011, 0x20);
        mem.write(0x2004, 0x14);
        cpu.A = 0x34;
        cpu.Y = 0x04;
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x20); // 52 - 20 = 32
    }

    // Decimal Mode Tests
    void testSBCDecimalModeBasic() {
        cpu.reset();
        cpu.A = 0x50; // 50 in BCD
        cpu.setC(true); // No borrow
        cpu.setD(true); // Enable decimal mode
        mem.write(0x8000, 0xE9); // SBC Immediate
        mem.write(0x8001, 0x25); // 25 in BCD
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x25); // 50 - 25 = 25 in BCD
        QCOMPARE(cpu.C(), (Byte)1); // No borrow
    }

    void testSBCDecimalModeWithBorrow() {
        cpu.reset();
        cpu.A = 0x50; // 50 in BCD
        cpu.setC(false); // Borrow
        cpu.setD(true);
        mem.write(0x8000, 0xE9); // SBC Immediate
        mem.write(0x8001, 0x25); // 25 in BCD
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x24); // 50 - 25 - 1 = 24 in BCD
        QCOMPARE(cpu.C(), (Byte)1);
    }

    void testSBCDecimalModeNeedsBorrow() {
        cpu.reset();
        cpu.A = 0x32; // 32 in BCD
        cpu.setC(true);
        cpu.setD(true);
        mem.write(0x8000, 0xE9); // SBC Immediate
        mem.write(0x8001, 0x04); // 4 in BCD
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x28); // 32 - 4 = 28 in BCD
        QCOMPARE(cpu.C(), (Byte)1);
    }

    void testSBCDecimalModeZeroResult() {
        cpu.reset();
        cpu.A = 0x25; // 25 in BCD
        cpu.setC(true);
        cpu.setD(true);
        mem.write(0x8000, 0xE9); // SBC Immediate
        mem.write(0x8001, 0x25); // 25 in BCD
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x00); // 25 - 25 = 0
        QCOMPARE(cpu.Z(), (Byte)1); // Zero flag set
        QCOMPARE(cpu.C(), (Byte)1);
    }

    void testSBCDecimalModeCrossNibble() {
        cpu.reset();
        cpu.A = 0x46; // 46 in BCD
        cpu.setC(true);
        cpu.setD(true);
        mem.write(0x8000, 0xE9); // SBC Immediate
        mem.write(0x8001, 0x08); // 8 in BCD
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x38); // 46 - 8 = 38 in BCD
        QCOMPARE(cpu.C(), (Byte)1);
    }

    void testSBCDecimalModeZeroPage() {
        cpu.reset();
        cpu.A = 0x75; // 75 in BCD
        cpu.setC(true);
        cpu.setD(true);
        mem.write(0x8000, 0xE5); // SBC Zero Page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x30); // 30 in BCD
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x45); // 75 - 30 = 45 in BCD
        QCOMPARE(cpu.C(), (Byte)1);
    }
};
