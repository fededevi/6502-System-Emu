#pragma once

#include <QtTest>
#include "../cpu.h"
#include "../memory.h"

class ShiftsTest : public QObject
{
    Q_OBJECT

    Memory mem;
    CPU cpu;

public:
    ShiftsTest()
        : mem()
        , cpu(&mem)
    {};
    ~ShiftsTest(){};

private slots:
    void initTestCase() {
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }

    // ASL - Arithmetic Shift Left
    void testASL_Accumulator() {
        cpu.reset();
        mem.write(0x8000, 0x0A); // ASL A
        cpu.A = 0x42;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x84);
        QCOMPARE(cpu.C(), (Byte)0); // No carry out
        QCOMPARE(cpu.N(), (Byte)1); // Bit 7 set
    }

    void testASL_Accumulator_Carry() {
        cpu.reset();
        mem.write(0x8000, 0x0A);
        cpu.A = 0x81;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x02);
        QCOMPARE(cpu.C(), (Byte)1); // Bit 7 shifted to carry
    }

    void testASL_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0x06); // ASL zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x55);
        cpu.execute();
        QCOMPARE(mem.read(0x0010), (Byte)0xAA);
        QCOMPARE(cpu.C(), (Byte)0);
    }

    // LSR - Logical Shift Right
    void testLSR_Accumulator() {
        cpu.reset();
        mem.write(0x8000, 0x4A); // LSR A
        cpu.A = 0x42;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x21);
        QCOMPARE(cpu.C(), (Byte)0);
        QCOMPARE(cpu.N(), (Byte)0);
    }

    void testLSR_Accumulator_Carry() {
        cpu.reset();
        mem.write(0x8000, 0x4A);
        cpu.A = 0x81;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x40);
        QCOMPARE(cpu.C(), (Byte)1); // Bit 0 shifted to carry
    }

    void testLSR_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0x46); // LSR zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0xAA);
        cpu.execute();
        QCOMPARE(mem.read(0x0010), (Byte)0x55);
    }

    // ROL - Rotate Left
    void testROL_Accumulator() {
        cpu.reset();
        mem.write(0x8000, 0x2A); // ROL A
        cpu.A = 0x42;
        cpu.setC(false);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x84);
        QCOMPARE(cpu.C(), (Byte)0);
    }

    void testROL_Accumulator_WithCarry() {
        cpu.reset();
        mem.write(0x8000, 0x2A);
        cpu.A = 0x42;
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x85); // Carry rotated into bit 0
    }

    void testROL_Accumulator_CarryOut() {
        cpu.reset();
        mem.write(0x8000, 0x2A);
        cpu.A = 0x81;
        cpu.setC(false);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x02);
        QCOMPARE(cpu.C(), (Byte)1); // Bit 7 rotated to carry
    }

    void testROL_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0x26); // ROL zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x40);
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(mem.read(0x0010), (Byte)0x81);
    }

    // ROR - Rotate Right
    void testROR_Accumulator() {
        cpu.reset();
        mem.write(0x8000, 0x6A); // ROR A
        cpu.A = 0x42;
        cpu.setC(false);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x21);
        QCOMPARE(cpu.C(), (Byte)0);
    }

    void testROR_Accumulator_WithCarry() {
        cpu.reset();
        mem.write(0x8000, 0x6A);
        cpu.A = 0x42;
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0xA1); // Carry rotated into bit 7
    }

    void testROR_Accumulator_CarryOut() {
        cpu.reset();
        mem.write(0x8000, 0x6A);
        cpu.A = 0x81;
        cpu.setC(false);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x40);
        QCOMPARE(cpu.C(), (Byte)1); // Bit 0 rotated to carry
    }

    void testROR_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0x66); // ROR zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x02);
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(mem.read(0x0010), (Byte)0x81);
    }
};
