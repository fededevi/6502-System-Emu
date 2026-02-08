#pragma once

#include <QtTest>
#include "../cpu.h"
#include "../memory.h"

class LogicalTest : public QObject
{
    Q_OBJECT

    Memory mem;
    CPU cpu;

public:
    LogicalTest()
        : mem()
        , cpu(&mem)
    {};
    ~LogicalTest(){};

private slots:
    void initTestCase() {
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }

    // AND - Logical AND
    void testAND_Immediate() {
        cpu.reset();
        mem.write(0x8000, 0x29); // AND immediate
        mem.write(0x8001, 0x0F);
        cpu.A = 0xFF;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x0F);
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.N(), (Byte)0);
    }

    void testAND_ZeroResult() {
        cpu.reset();
        mem.write(0x8000, 0x29);
        mem.write(0x8001, 0x00);
        cpu.A = 0xFF;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x00);
        QCOMPARE(cpu.Z(), (Byte)1);
    }

    void testAND_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0x25); // AND zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x55);
        cpu.A = 0xAA;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x00);
    }

    void testAND_Absolute() {
        cpu.reset();
        mem.write(0x8000, 0x2D); // AND absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0xF0);
        cpu.A = 0xFF;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0xF0);
    }

    // ORA - Logical OR
    void testORA_Immediate() {
        cpu.reset();
        mem.write(0x8000, 0x09); // ORA immediate
        mem.write(0x8001, 0xF0);
        cpu.A = 0x0F;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0xFF);
        QCOMPARE(cpu.N(), (Byte)1);
    }

    void testORA_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0x05); // ORA zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x55);
        cpu.A = 0xAA;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0xFF);
    }

    void testORA_Absolute() {
        cpu.reset();
        mem.write(0x8000, 0x0D); // ORA absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x0F);
        cpu.A = 0x00;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x0F);
    }

    // EOR - Exclusive OR
    void testEOR_Immediate() {
        cpu.reset();
        mem.write(0x8000, 0x49); // EOR immediate
        mem.write(0x8001, 0xFF);
        cpu.A = 0xAA;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x55);
    }

    void testEOR_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0x45); // EOR zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x0F);
        cpu.A = 0xFF;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0xF0);
    }

    void testEOR_SelfCancel() {
        cpu.reset();
        mem.write(0x8000, 0x49); // EOR immediate
        mem.write(0x8001, 0x42);
        cpu.A = 0x42;
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x00);
        QCOMPARE(cpu.Z(), (Byte)1);
    }
};
