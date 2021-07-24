#include "cpu.h"

#include "load.cpp"
#include "store.cpp"

CPU::CPU(Memory *_mem): mem(_mem){}

void BRK(CPU * cpu) {
    cpu->push(cpu->PC >> 8);
    cpu->push(cpu->PC);
    cpu->push(cpu->P);
    cpu->PC = cpu->mem->read16(0XFFFE);
    cpu->setB(true);
}


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
    PC = mem->read16(0XFFFC);
    SP = 0XFD;

    A = 0X0;
    X = 0X0;
    Y = 0X0;

    P = 0X0;
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


Word CPU::immediate() {
    return PC++;
}

Word CPU::zeroPage() {
    CYCL;
    addr8 = re(PC++);
    return addr8;
}

Word CPU::zeroPageX() {
    CYCL;
    addr16 = re(PC++);
    CYCL;
    addr16 += X;
    return addr16;
}

Word CPU::zeroPageY() {
    CYCL;
    addr16 = re(PC++);
    CYCL;
    addr16 += Y;
    return addr16;
}

Word CPU::absolute() {
    CYCL;
    addr16 = re(PC++);
    CYCL;
    addr16 += re(PC++) >> 8;
    return addr16;
}

Word CPU::absoluteX() {
    CYCL;
    addr16 = re(PC++) + X;
    if (addr16 > 0xFF) CYCL;
    CYCL;
    addr16 += re(PC++) >> 8;
    return addr16;
}

Word CPU::absoluteY() {
    CYCL;
    addr16 = re(PC++) + X;
    if (addr16 > 0xFF) CYCL;
    CYCL;
    addr16 += re(PC++) >> 8;
    return addr16;
}

Word CPU::indirectX() {
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

Word CPU::indirectY() {
    CYCL;
    addr16 = re(PC++) + X;
    if (addr16 > 0xFF) CYCL;
    CYCL;
    addr16 += re(PC++) >> 8;
    CYCL;
    addr16 = re(addr16);
    return addr16;
}

void CPU::setN(bool f){
    int pos = 0;
    P  = P & ~(1 << pos);
    P |= (f << pos);
}

void CPU::setV(bool f){
    int pos = 1;
    P  = P & ~(1 << pos);
    P |= (f << pos);
}

void CPU::setB(bool f){
    int pos = 3;
    P  = P & ~(1 << pos);
    P |= (f << pos);
}

void CPU::setD(bool f){
    int pos = 4;
    P  = P & ~(1 << pos);
    P |= (f << pos);
}

void CPU::setI(bool f){
    int pos = 5;
    P  = P & ~(1 << pos);
    P |= (f << pos);
}

void CPU::setZ(bool f){
    int pos = 6;
    P  = P & ~(1 << pos);
    P |= (f << pos);
}

void CPU::setC(bool f){
    int pos = 7;
    P  = P & ~(1 << pos);
    P |= (f << pos);

}


