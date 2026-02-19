#pragma once

#include "types.h"
#include "memory.h"

class CPU
{

public:
    CPU(Memory *);
    Memory * mem;

    Byte addr8;
    Word addr16;

    void reset();
    void execute();

    Word PC;
    Byte SP;

    Byte A;
    Byte X;
    Byte Y;

    Byte P;

    inline void cycl() { ++cycles; }
    void push(Byte);
    Byte pop();

    Word immediate();
    Byte zeroPage();
    Byte zeroPageX();
    Byte zeroPageY();
    Word absolute();
    Word absoluteX();
    Word absoluteY();
    Word indirectX();
    Word indirectY();

    void setN(bool);
    Byte N() { return (P & (1 << 7)) > 0; };
    void setV(bool);
    Byte V() { return (P & (1 << 6)) > 0; };
    void setB(bool);
    Byte B() { return (P & (1 << 4)) > 0; };
    void setD(bool);
    Byte D() { return (P & (1 << 3)) > 0; };
    void setI(bool);
    Byte I() { return (P & (1 << 2)) > 0; };
    void setZ(bool);
    Byte Z() { return (P & (1 << 1)) > 0; };
    void setC(bool);
    Byte C() { return (P & (1 << 0)) > 0; };

    // Cycles
    long long cycles = 0;
};
