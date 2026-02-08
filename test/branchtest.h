#pragma once

#include <QtTest>
#include "../cpu.h"
#include "../memory.h"

class BranchTest : public QObject
{
    Q_OBJECT

    Memory mem;
    CPU cpu;

public:
    BranchTest()
        : mem()
        , cpu(&mem)
    {};
    ~BranchTest(){};

private slots:
    void initTestCase() {
        mem.write(0xFFFC, 0x00);
        mem.write(0xFFFD, 0x80);
    }

    // BEQ - Branch if Equal (0xF0)
    void testBEQ_Taken() {
        cpu.reset();
        mem.write(0x8000, 0xF0); // BEQ
        mem.write(0x8001, 0x05); // offset +5
        cpu.setZ(true);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x8007); // 0x8002 + 0x05
    }

    void testBEQ_NotTaken() {
        cpu.reset();
        mem.write(0x8000, 0xF0);
        mem.write(0x8001, 0x05);
        cpu.setZ(false);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x8002); // No branch
    }

    void testBEQ_Backward() {
        cpu.reset();
        cpu.PC = 0x8010;
        mem.write(0x8010, 0xF0);
        mem.write(0x8011, 0xFA); // -6 in two's complement
        cpu.setZ(true);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x800C); // 0x8012 + (-6)
    }

    // BNE - Branch if Not Equal (0xD0)
    void testBNE_Taken() {
        cpu.reset();
        mem.write(0x8000, 0xD0);
        mem.write(0x8001, 0x10);
        cpu.setZ(false);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x8012);
    }

    void testBNE_NotTaken() {
        cpu.reset();
        mem.write(0x8000, 0xD0);
        mem.write(0x8001, 0x10);
        cpu.setZ(true);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x8002);
    }

    // BCC - Branch if Carry Clear (0x90)
    void testBCC_Taken() {
        cpu.reset();
        mem.write(0x8000, 0x90);
        mem.write(0x8001, 0x08);
        cpu.setC(false);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x800A);
    }

    void testBCC_NotTaken() {
        cpu.reset();
        mem.write(0x8000, 0x90);
        mem.write(0x8001, 0x08);
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x8002);
    }

    // BCS - Branch if Carry Set (0xB0)
    void testBCS_Taken() {
        cpu.reset();
        mem.write(0x8000, 0xB0);
        mem.write(0x8001, 0x04);
        cpu.setC(true);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x8006);
    }

    // BMI - Branch if Minus (0x30)
    void testBMI_Taken() {
        cpu.reset();
        mem.write(0x8000, 0x30);
        mem.write(0x8001, 0x02);
        cpu.setN(true);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x8004);
    }

    void testBMI_NotTaken() {
        cpu.reset();
        mem.write(0x8000, 0x30);
        mem.write(0x8001, 0x02);
        cpu.setN(false);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x8002);
    }

    // BPL - Branch if Plus (0x10)
    void testBPL_Taken() {
        cpu.reset();
        mem.write(0x8000, 0x10);
        mem.write(0x8001, 0x03);
        cpu.setN(false);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x8005);
    }

    // BVC - Branch if Overflow Clear (0x50)
    void testBVC_Taken() {
        cpu.reset();
        mem.write(0x8000, 0x50);
        mem.write(0x8001, 0x07);
        cpu.setV(false);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x8009);
    }

    // BVS - Branch if Overflow Set (0x70)
    void testBVS_Taken() {
        cpu.reset();
        mem.write(0x8000, 0x70);
        mem.write(0x8001, 0x01);
        cpu.setV(true);
        cpu.execute();
        QCOMPARE(cpu.PC, (Word)0x8003);
    }
};
