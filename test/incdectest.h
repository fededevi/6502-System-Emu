#pragma once

#include <QtTest>
#include "../cpu.h"
#include "../memory.h"

class IncDecTest : public QObject
{
    Q_OBJECT

    Memory mem;
    CPU cpu;

public:
    IncDecTest()
        : mem()
        , cpu(&mem)
    {};
    ~IncDecTest(){};

private slots:
    void initTestCase() {
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }

    // INX - Increment X (0xE8)
    void testINX() {
        cpu.reset();
        mem.write(0x8000, 0xE8);
        cpu.X = 0x10;
        cpu.execute();
        QCOMPARE(cpu.X, (Byte)0x11);
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.N(), (Byte)0);
    }

    void testINX_Overflow() {
        cpu.reset();
        mem.write(0x8000, 0xE8);
        cpu.X = 0xFF;
        cpu.execute();
        QCOMPARE(cpu.X, (Byte)0x00);
        QCOMPARE(cpu.Z(), (Byte)1);
    }

    void testINX_Negative() {
        cpu.reset();
        mem.write(0x8000, 0xE8);
        cpu.X = 0x7F;
        cpu.execute();
        QCOMPARE(cpu.X, (Byte)0x80);
        QCOMPARE(cpu.N(), (Byte)1);
    }

    // INY - Increment Y (0xC8)
    void testINY() {
        cpu.reset();
        mem.write(0x8000, 0xC8);
        cpu.Y = 0x20;
        cpu.execute();
        QCOMPARE(cpu.Y, (Byte)0x21);
    }

    // DEX - Decrement X (0xCA)
    void testDEX() {
        cpu.reset();
        mem.write(0x8000, 0xCA);
        cpu.X = 0x10;
        cpu.execute();
        QCOMPARE(cpu.X, (Byte)0x0F);
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.N(), (Byte)0);
    }

    void testDEX_Zero() {
        cpu.reset();
        mem.write(0x8000, 0xCA);
        cpu.X = 0x01;
        cpu.execute();
        QCOMPARE(cpu.X, (Byte)0x00);
        QCOMPARE(cpu.Z(), (Byte)1);
    }

    void testDEX_Underflow() {
        cpu.reset();
        mem.write(0x8000, 0xCA);
        cpu.X = 0x00;
        cpu.execute();
        QCOMPARE(cpu.X, (Byte)0xFF);
        QCOMPARE(cpu.N(), (Byte)1);
    }

    // DEY - Decrement Y (0x88)
    void testDEY() {
        cpu.reset();
        mem.write(0x8000, 0x88);
        cpu.Y = 0x20;
        cpu.execute();
        QCOMPARE(cpu.Y, (Byte)0x1F);
    }

    // INC - Increment Memory (0xE6 zero page)
    void testINC_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0xE6);
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x42);
        cpu.execute();
        QCOMPARE(mem.read(0x0010), (Byte)0x43);
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.N(), (Byte)0);
    }

    void testINC_ZeroPageX() {
        cpu.reset();
        mem.write(0x8000, 0xF6);
        mem.write(0x8001, 0x10);
        mem.write(0x0015, 0x7F);
        cpu.X = 0x05;
        cpu.execute();
        QCOMPARE(mem.read(0x0015), (Byte)0x80);
        QCOMPARE(cpu.N(), (Byte)1);
    }

    void testINC_Absolute() {
        cpu.reset();
        mem.write(0x8000, 0xEE);
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0xFF);
        cpu.execute();
        QCOMPARE(mem.read(0x2000), (Byte)0x00);
        QCOMPARE(cpu.Z(), (Byte)1);
    }

    // DEC - Decrement Memory (0xC6 zero page)
    void testDEC_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0xC6);
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x42);
        cpu.execute();
        QCOMPARE(mem.read(0x0010), (Byte)0x41);
    }

    void testDEC_ZeroPageX() {
        cpu.reset();
        mem.write(0x8000, 0xD6);
        mem.write(0x8001, 0x10);
        mem.write(0x0015, 0x01);
        cpu.X = 0x05;
        cpu.execute();
        QCOMPARE(mem.read(0x0015), (Byte)0x00);
        QCOMPARE(cpu.Z(), (Byte)1);
    }

    void testDEC_Absolute() {
        cpu.reset();
        mem.write(0x8000, 0xCE);
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x00);
        cpu.execute();
        QCOMPARE(mem.read(0x2000), (Byte)0xFF);
        QCOMPARE(cpu.N(), (Byte)1);
    }
};
