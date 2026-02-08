#pragma once

#include <QtTest>
#include "../cpu.h"
#include "../memory.h"

class FlagsTest : public QObject
{
    Q_OBJECT

    Memory mem;
    CPU cpu;

public:
    FlagsTest()
        : mem()
        , cpu(&mem)
    {};
    ~FlagsTest(){};

private slots:
    void initTestCase() {
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }

    // CLC - Clear Carry (0x18)
    void testCLC() {
        cpu.reset();
        mem.write(0x8000, 0x18);
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.C(), (Byte)0);
    }

    // SEC - Set Carry (0x38)
    void testSEC() {
        cpu.reset();
        mem.write(0x8000, 0x38);
        cpu.setC(false);
        cpu.execute();
        QCOMPARE(cpu.C(), (Byte)1);
    }

    // CLI - Clear Interrupt Disable (0x58)
    void testCLI() {
        cpu.reset();
        mem.write(0x8000, 0x58);
        cpu.setI(true);
        cpu.execute();
        QCOMPARE(cpu.I(), (Byte)0);
    }

    // SEI - Set Interrupt Disable (0x78)
    void testSEI() {
        cpu.reset();
        mem.write(0x8000, 0x78);
        cpu.setI(false);
        cpu.execute();
        QCOMPARE(cpu.I(), (Byte)1);
    }

    // CLV - Clear Overflow (0xB8)
    void testCLV() {
        cpu.reset();
        mem.write(0x8000, 0xB8);
        cpu.setV(true);
        cpu.execute();
        QCOMPARE(cpu.V(), (Byte)0);
    }

    // CLD - Clear Decimal (0xD8)
    void testCLD() {
        cpu.reset();
        mem.write(0x8000, 0xD8);
        cpu.setD(true);
        cpu.execute();
        QCOMPARE(cpu.D(), (Byte)0);
    }

    // SED - Set Decimal (0xF8)
    void testSED() {
        cpu.reset();
        mem.write(0x8000, 0xF8);
        cpu.setD(false);
        cpu.execute();
        QCOMPARE(cpu.D(), (Byte)1);
    }

    // Test multiple flag operations
    void testMultipleFlags() {
        cpu.reset();
        mem.write(0x8000, 0x38); // SEC
        mem.write(0x8001, 0x78); // SEI
        mem.write(0x8002, 0xF8); // SED
        
        cpu.execute();
        QCOMPARE(cpu.C(), (Byte)1);
        
        cpu.execute();
        QCOMPARE(cpu.I(), (Byte)1);
        
        cpu.execute();
        QCOMPARE(cpu.D(), (Byte)1);
    }
};
