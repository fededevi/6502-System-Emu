#pragma once

#include <QtTest>
#include "../cpu.h"
#include "../memory.h"

class StackTest : public QObject
{
    Q_OBJECT

    Memory mem;
    CPU cpu;

public:
    StackTest()
        : mem()
        , cpu(&mem)
    {};
    ~StackTest(){};

private slots:
    void initTestCase() {
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }

    // PHA - Push Accumulator (0x48)
    void testPHA() {
        cpu.reset();
        mem.write(0x8000, 0x48);
        cpu.A = 0x77;
        Byte oldSP = cpu.SP;
        cpu.execute();
        QCOMPARE(mem.read(0x100 + oldSP), (Byte)0x77);
        QCOMPARE(cpu.SP, (Byte)(oldSP - 1));
    }

    // PLA - Pull Accumulator (0x68)
    void testPLA() {
        cpu.reset();
        mem.write(0x8000, 0x68);
        cpu.push(0x88);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x88);
    }

    void testPLA_Zero() {
        cpu.reset();
        mem.write(0x8000, 0x68);
        cpu.push(0x00);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x00);
        QCOMPARE(cpu.Z(), (Byte)1);
    }

    void testPLA_Negative() {
        cpu.reset();
        mem.write(0x8000, 0x68);
        cpu.push(0x80);
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x80);
        QCOMPARE(cpu.N(), (Byte)1);
    }

    // PHP - Push Processor Status (0x08)
    void testPHP() {
        cpu.reset();
        mem.write(0x8000, 0x08);
        cpu.P = 0xC5;
        Byte oldSP = cpu.SP;
        cpu.execute();
        QCOMPARE(mem.read(0x100 + oldSP), (Byte)0xD5); // B flag is set when pushed
        QCOMPARE(cpu.SP, (Byte)(oldSP - 1));
    }

    // PLP - Pull Processor Status (0x28)
    void testPLP() {
        cpu.reset();
        mem.write(0x8000, 0x28);
        cpu.push(0x42);
        cpu.execute();
        QCOMPARE(cpu.P, (Byte)0x42);
    }

    // Test PHA/PLA together
    void testPHAPLA() {
        cpu.reset();
        mem.write(0x8000, 0x48); // PHA
        mem.write(0x8001, 0x68); // PLA
        cpu.A = 0x99;
        cpu.execute();
        cpu.execute();
        QCOMPARE(cpu.A, (Byte)0x99);
    }
};
