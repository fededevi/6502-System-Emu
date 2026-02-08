#pragma once

#include <QtTest>
#include "../cpu.h"
#include "../memory.h"

class MiscTest : public QObject
{
    Q_OBJECT

    Memory mem;
    CPU cpu;

public:
    MiscTest()
        : mem()
        , cpu(&mem)
    {};
    ~MiscTest(){};

private slots:
    void initTestCase() {
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }

    // NOP - No Operation (0xEA)
    void testNOP() {
        cpu.reset();
        mem.write(0x8000, 0xEA);
        Word oldPC = cpu.PC;
        Byte oldA = cpu.A = 0x42;
        Byte oldX = cpu.X = 0x55;
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)(oldPC + 1));
        QCOMPARE(cpu.A, oldA);
        QCOMPARE(cpu.X, oldX);
    }

    // JMP Absolute (0x4C)
    void testJMP_Absolute() {
        cpu.reset();
        mem.write(0x8000, 0x4C); // JMP absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x90);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x9000);
    }

    // JMP Indirect (0x6C)
    void testJMP_Indirect() {
        cpu.reset();
        mem.write(0x8000, 0x6C); // JMP indirect
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x00);
        mem.write(0x2001, 0x30);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x3000);
    }

    // JSR - Jump to Subroutine (0x20)
    void testJSR() {
        cpu.reset();
        mem.write(0x8000, 0x20); // JSR
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x90);
        Byte oldSP = cpu.SP;
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x9000);
        // Check return address on stack (PC-1)
        QCOMPARE(cpu.SP, (Byte)(oldSP - 2));
    }

    // RTS - Return from Subroutine (0x60)
    void testRTS() {
        cpu.reset();
        // Set up stack with return address 0x8005
        cpu.push(0x80);
        cpu.push(0x04);
        mem.write(0x8000, 0x60); // RTS
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x8005);
    }

    // BIT - Test Bits Zero Page (0x24)
    void testBIT_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0x24); // BIT zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0xC0); // Bits 7 and 6 set
        cpu.A = 0xFF;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)0); // Result not zero
        QCOMPARE(cpu.N(), (Byte)1); // Bit 7 of memory
        QCOMPARE(cpu.V(), (Byte)1); // Bit 6 of memory
    }

    void testBIT_ZeroPage_ZeroResult() {
        cpu.reset();
        mem.write(0x8000, 0x24);
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x55);
        cpu.A = 0xAA;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)1); // A & M = 0
    }

    // BIT - Test Bits Absolute (0x2C)
    void testBIT_Absolute() {
        cpu.reset();
        mem.write(0x8000, 0x2C); // BIT absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x80); // Bit 7 set
        cpu.A = 0x80;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.N(), (Byte)1);
        QCOMPARE(cpu.V(), (Byte)0);
    }
};
