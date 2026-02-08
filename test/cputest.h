#pragma once

#include <QtTest>
#include "../cpu.h"
#include "../memory.h"

class CpuTest : public QObject
{
    Q_OBJECT

    Memory mem;
    CPU cpu;

public:
    CpuTest()
        : mem(Memory::randomMemory())
        , cpu(&mem)
    {};

    ~CpuTest(){};


private slots:
    void resetTest(){
        cpu.reset();
        QCOMPARE( cpu.P, 0);
        QCOMPARE( cpu.A, 0);
        QCOMPARE( cpu.X, 0);
        QCOMPARE( cpu.Y, 0);
        QCOMPARE( cpu.PC, mem.read16(0xFFFC));
        QCOMPARE( cpu.SP, 0XFD);
        QCOMPARE( cpu.cycles, 7);
    };

    void immediateTest(){
        cpu.reset();
        Word addr = cpu.PC;
        QCOMPARE( cpu.immediate(), addr);
    };

    void zeroPage(){
        cpu.reset();
        Byte addr = mem.read(cpu.PC);
        QCOMPARE( cpu.zeroPage(), addr);
    };

    void zeroPageX(){
        cpu.reset();
        Byte addr = mem.read(cpu.PC) + cpu.X;
        QCOMPARE( cpu.zeroPageX(), addr);
    };

    void zeroPageX_checkOverflow(){
        cpu.reset();
        cpu.X = 0xB7;
        mem.write(cpu.PC, 0xB7);
        Byte addr = Byte(0xB7 + 0xB7);

        QCOMPARE( cpu.zeroPageX(), addr);
    };

    void zeroPageY(){
        cpu.reset();
        Byte addr = mem.read(cpu.PC) + cpu.Y;
        QCOMPARE( cpu.zeroPageY(), addr);
    };

    void zeroPageY_checkOverflow(){
        cpu.reset();
        cpu.Y = 0xB7;
        mem.write(cpu.PC, 0xB7);
        Byte addr = Byte(0xB7 + 0xB7);

        QCOMPARE( cpu.zeroPageY(), addr);
    };

    void absolute(){
        cpu.reset();
        Word addr = mem.read16(cpu.PC);
        QCOMPARE( cpu.absolute(), addr);
    };

    void absoluteX(){
        cpu.reset();
        cpu.X = 0x10;
        Word base = mem.read16(cpu.PC);
        QCOMPARE( cpu.absoluteX(), (Word)(base + cpu.X));
    };

    void absoluteY(){
        cpu.reset();
        cpu.Y = 0x20;
        Word base = mem.read16(cpu.PC);
        QCOMPARE( cpu.absoluteY(), (Word)(base + cpu.Y));
    };

    void indirectX(){
        cpu.reset();
        cpu.X = 0x05;
        Byte zpAddr = mem.read(cpu.PC);
        Byte finalZP = zpAddr + cpu.X;
        Word target = mem.read16(finalZP);
        QCOMPARE( cpu.indirectX(), target);
    };

    void indirectY(){
        cpu.reset();
        cpu.Y = 0x08;
        Byte zpAddr = mem.read(cpu.PC);
        Word base = mem.read16(zpAddr);
        QCOMPARE( cpu.indirectY(), (Word)(base + cpu.Y));
    };

    // Flag tests
    void testFlagN(){
        cpu.reset();
        cpu.setN(true);
        QCOMPARE( cpu.N(), (Byte)1);
        cpu.setN(false);
        QCOMPARE( cpu.N(), (Byte)0);
    };

    void testFlagV(){
        cpu.reset();
        cpu.setV(true);
        QCOMPARE( cpu.V(), (Byte)1);
        cpu.setV(false);
        QCOMPARE( cpu.V(), (Byte)0);
    };

    void testFlagZ(){
        cpu.reset();
        cpu.setZ(true);
        QCOMPARE( cpu.Z(), (Byte)1);
        cpu.setZ(false);
        QCOMPARE( cpu.Z(), (Byte)0);
    };

    void testFlagC(){
        cpu.reset();
        cpu.setC(true);
        QCOMPARE( cpu.C(), (Byte)1);
        cpu.setC(false);
        QCOMPARE( cpu.C(), (Byte)0);
    };

    void testFlagB(){
        cpu.reset();
        cpu.setB(true);
        QCOMPARE( cpu.B(), (Byte)1);
        cpu.setB(false);
        QCOMPARE( cpu.B(), (Byte)0);
    };

    void testFlagD(){
        cpu.reset();
        cpu.setD(true);
        QCOMPARE( cpu.D(), (Byte)1);
        cpu.setD(false);
        QCOMPARE( cpu.D(), (Byte)0);
    };

    void testFlagI(){
        cpu.reset();
        cpu.setI(true);
        QCOMPARE( cpu.I(), (Byte)1);
        cpu.setI(false);
        QCOMPARE( cpu.I(), (Byte)0);
    };

    // Stack tests
    void testPush(){
        cpu.reset();
        Byte initialSP = cpu.SP;
        cpu.push(0x42);
        QCOMPARE( cpu.SP, (Byte)(initialSP - 1));
        QCOMPARE( mem.read(0x100 + initialSP), (Byte)0x42);
    };

    void testPushPop(){
        cpu.reset();
        cpu.push(0x55);
        cpu.SP++; // Simulate pop's SP increment
        Byte value = cpu.pop();
        QCOMPARE( value, (Byte)0x55);
    };
};




