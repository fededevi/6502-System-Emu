#pragma once

#include <QtTest>
#include "../cpu.h"
#include "../memory.h"

class CompareTest : public QObject
{
    Q_OBJECT

    Memory mem;
    CPU cpu;

public:
    CompareTest()
        : mem()
        , cpu(&mem)
    {};
    ~CompareTest(){};

private slots:
    void initTestCase() {
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }

    // CMP - Compare Accumulator
    void testCMP_Immediate_Equal() {
        cpu.reset();
        mem.write(0x8000, 0xC9); // CMP immediate
        mem.write(0x8001, 0x42);
        cpu.A = 0x42;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)1); // Equal
        QCOMPARE(cpu.C(), (Byte)1); // A >= M
        QCOMPARE(cpu.N(), (Byte)0);
    }

    void testCMP_Immediate_Greater() {
        cpu.reset();
        mem.write(0x8000, 0xC9);
        mem.write(0x8001, 0x10);
        cpu.A = 0x20;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)0); // Not equal
        QCOMPARE(cpu.C(), (Byte)1); // A >= M
        QCOMPARE(cpu.N(), (Byte)0); // Result positive
    }

    void testCMP_Immediate_Less() {
        cpu.reset();
        mem.write(0x8000, 0xC9);
        mem.write(0x8001, 0x50);
        cpu.A = 0x30;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)0); // Not equal
        QCOMPARE(cpu.C(), (Byte)0); // A < M
        QCOMPARE(cpu.N(), (Byte)1); // Result negative
    }

    void testCMP_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0xC5); // CMP zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x42);
        cpu.A = 0x42;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)1);
        QCOMPARE(cpu.C(), (Byte)1);
    }

    void testCMP_Absolute() {
        cpu.reset();
        mem.write(0x8000, 0xCD); // CMP absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x80);
        cpu.A = 0x90;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.C(), (Byte)1); // 0x90 >= 0x80
    }

    // CPX - Compare X Register
    void testCPX_Immediate_Equal() {
        cpu.reset();
        mem.write(0x8000, 0xE0); // CPX immediate
        mem.write(0x8001, 0x33);
        cpu.X = 0x33;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)1);
        QCOMPARE(cpu.C(), (Byte)1);
    }

    void testCPX_Immediate_Less() {
        cpu.reset();
        mem.write(0x8000, 0xE0);
        mem.write(0x8001, 0x50);
        cpu.X = 0x30;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.C(), (Byte)0);
    }

    void testCPX_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0xE4); // CPX zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x20);
        cpu.X = 0x30;
        cpu.execute();
        QCOMPARE(cpu.C(), (Byte)1); // X >= M
    }

    void testCPX_Absolute() {
        cpu.reset();
        mem.write(0x8000, 0xEC); // CPX absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x40);
        cpu.X = 0x40;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)1);
        QCOMPARE(cpu.C(), (Byte)1);
    }

    // CPY - Compare Y Register
    void testCPY_Immediate_Equal() {
        cpu.reset();
        mem.write(0x8000, 0xC0); // CPY immediate
        mem.write(0x8001, 0x55);
        cpu.Y = 0x55;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)1);
        QCOMPARE(cpu.C(), (Byte)1);
    }

    void testCPY_ZeroPage() {
        cpu.reset();
        mem.write(0x8000, 0xC4); // CPY zero page
        mem.write(0x8001, 0x10);
        mem.write(0x0010, 0x60);
        cpu.Y = 0x50;
        cpu.execute();
        QCOMPARE(cpu.Z(), (Byte)0);
        QCOMPARE(cpu.C(), (Byte)0); // Y < M
    }

    void testCPY_Absolute() {
        cpu.reset();
        mem.write(0x8000, 0xCC); // CPY absolute
        mem.write(0x8001, 0x00);
        mem.write(0x8002, 0x20);
        mem.write(0x2000, 0x30);
        cpu.Y = 0x40;
        cpu.execute();
        QCOMPARE(cpu.C(), (Byte)1); // Y >= M
    }
};
