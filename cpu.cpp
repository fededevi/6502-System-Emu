#include "cpu.h"

#include "load.cpp"
#include "store.cpp"

CPU::CPU(Memory *_mem): mem(_mem){}

void BRK(CPU *) {}


#define re mem->read
#define wr mem->write
#define CYCL cycl()

//Instructions
void (*functptr[256])(CPU *) = {
        //          0        1       2       3       4       5       6       7       8       9       A       B       C       D       E       F
        /*0*/       &BRK,    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        /*1*/       0,       0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        /*2*/       0,       0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        /*3*/       0,       0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        /*4*/       0,       0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        /*5*/       0,       0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        /*6*/       0,       0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        /*7*/       0,       0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        /*8*/       0,       &STAIX, 0,      0,      &STYZP, &STAZ,  &STXZP, 0,      0,      0,      0,      0,      &STYA,  &STAA,  &STXA,  0,
        /*9*/       0,       &STAIY, 0,      0,      &STYZPX,&STAZX, &STXZPY,0,      0,      &STAAY, 0,      0,      0,      &STAAX, 0,      0,
        /*A*/       &LDYI,   &LDAIX, &LDXI,  0,      LDYZP,  &LDAZ,  &LDXZP, 0,      0,      &LDAI,  0,      0,      LDYA,   &LDAA,  &LDXA,  0,
        /*B*/       0,       &LDAIY, 0,      0,      LDYZPY, &LDAZX, &LDXZPY,0,      0,      &LDAAY, 0,      0,      LDYAY,  &LDAAX, &LDXAY, 0,
        /*C*/       0,       0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        /*D*/       0,       0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        /*E*/       0,       0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        /*F*/       0,       0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0
        } ;


void CPU::reset()
{
    PC = 0XFFFE;
    SP = 0XFF;

    A = 0X0;
    X = 0X0;
    Y = 0X0;

    N = 0X0;
    Z = 0X0;
    C = 0X0;
    I = 0X0;
    D = 0X0;
    V = 0X0;
}

void CPU::execute()
{
    Byte instruction = mem->read(PC++);
    functptr[instruction](this);
}

void CPU::cycl() const
{

}

void CPU::push(uint8_t v) {
    mem->write(0x100+SP, v);
    SP--;
}

uint8_t CPU::pop() {
    return mem->read(0x100+SP);
    SP++;
}

uint16_t CPU::immediate() {
    return PC++;
}

uint16_t CPU::zeroPage() {
    CYCL;
    addr8 = re(PC++);
    return addr8;
}

uint16_t CPU::zeroPageX() {
    CYCL;
    addr16 = re(PC++);
    CYCL;
    addr16 += X;
    return addr16;
}

uint16_t CPU::zeroPageY() {
    CYCL;
    addr16 = re(PC++);
    CYCL;
    addr16 += Y;
    return addr16;
}

uint16_t CPU::absolute() {
    CYCL;
    addr16 = re(PC++);
    CYCL;
    addr16 += re(PC++) >> 8;
    return addr16;
}

uint16_t CPU::absoluteX() {
    CYCL;
    addr16 = re(PC++) + X;
    if (addr16 > 0xFF) CYCL;
    CYCL;
    addr16 += re(PC++) >> 8;
    return addr16;
}

uint16_t CPU::absoluteY() {
    CYCL;
    addr16 = re(PC++) + X;
    if (addr16 > 0xFF) CYCL;
    CYCL;
    addr16 += re(PC++) >> 8;
    return addr16;
}

uint16_t CPU::indirectX() {
    CYCL;
    addr8 = re(PC++);
    CYCL;
    addr8 += X;
    CYCL;
    addr16 = re(addr8);
    CYCL;
    addr16 += re(addr8++) >> 8;
    return addr16;
}

uint16_t CPU::indirectY() {
    CYCL;
    addr16 = re(PC++) + X;
    if (addr16 > 0xFF) CYCL;
    CYCL;
    addr16 += re(PC++) >> 8;
    CYCL;
    addr16 = re(addr16);
    return addr16;
}


