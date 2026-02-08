#pragma once

#include <QtTest>
#include "../cpu.h"
#include "../memory.h"

class TransferTest : public QObject
{
    Q_OBJECT

    Memory mem;
    CPU cpu;

public:
    TransferTest()
        : mem()
        , cpu(&mem)
    {};
    ~TransferTest(){};

private slots:
    void initTestCase() {
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }

    // TAX - Transfer A to X (0xAA)
    void testTAX() {
        cpu.reset();
        mem.write(0x8000, 0xAA);
        cpu.A = 0x42;
        cpu.execute();
        QCOMPARE(cpu.X, (Byte)0x42);
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.N(), (Byte)0);
    }

    void testTAX_Zero() {
        cpu.reset();
        mem.write(0x8000, 0xAA);
        cpu.A = 0x00;
        cpu.execute();
        QCOMPARE(cpu.X, (Byte)0x00);
        QCOMPARE(cpu.Z(), (Byte)1);
        QCOMPARE(cpu.N(), (Byte)0);
    }

    void testTAX_Negative() {
        cpu.reset();
        mem.write(0x8000, 0xAA);
        cpu.A = 0x80;
        cpu.execute();
        QCOMPARE(cpu.X, (Byte)0x80);
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.N(), (Byte)1);
    }

    // TAY - Transfer A to Y (0xA8)
    void testTAY() {
        cpu.reset();
        mem.write(0x8000, 0xA8);
        cpu.A = 0x55;
        cpu.execute();
        QCOMPARE(cpu.Y, (Byte)0x55);
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.N(), (Byte)0);
    }

    // TXA - Transfer X to A (0x8A)
    void testTXA() {
        cpu.reset();
        mem.write(0x8000, 0x8A);
        cpu.X = 0x33;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x33);
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.N(), (Byte)0);
    }

    // TYA - Transfer Y to A (0x98)
    void testTYA() {
        cpu.reset();
        mem.write(0x8000, 0x98);
        cpu.Y = 0x66;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x66);
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.N(), (Byte)0);
    }

    // TSX - Transfer SP to X (0xBA)
    void testTSX() {
        cpu.reset();
        mem.write(0x8000, 0xBA);
        cpu.SP = 0xF0;
        cpu.execute();
        QCOMPARE(cpu.X, (Byte)0xF0);
        QCOMPARE(cpu.N(), (Byte)1); // 0xF0 has bit 7 set
    }

    // TXS - Transfer X to SP (0x9A)
    void testTXS() {
        cpu.reset();
        mem.write(0x8000, 0x9A);
        cpu.X = 0xA5;
        cpu.execute();
        QCOMPARE(cpu.SP, (Byte)0xA5);
        // TXS doesn't affect flags
    }
};
