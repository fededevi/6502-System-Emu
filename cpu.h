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

    Word immediate();
    Word zeroPage();
    Word zeroPageX();
    Word zeroPageY();
    Word absolute();
    Word absoluteX();
    Word absoluteY();
    Word indirectX();
    Word indirectY();
};


#endif // CPU_6502_H
