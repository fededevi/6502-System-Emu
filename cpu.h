#ifndef CPU_6502_H
#define CPU_6502_H

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

    Byte N;
    Byte Z;
    Byte C;
    Byte I;
    Byte D;
    Byte V;

    void cycl() const;
    void push(Byte);
    Byte pop();

    Byte immediate();
    Byte zeroPage();
    Byte zeroPageX();
    Byte zeroPageY();
    Byte absolute();
    Byte absoluteX();
    Byte absoluteY();
    Byte indirectX();
    Byte indirectY();
};


#endif // CPU_6502_H
