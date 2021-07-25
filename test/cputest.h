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
        Byte addr = cpu.PC;
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
};




