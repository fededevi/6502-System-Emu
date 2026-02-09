#include "cpu.h"

// Forward declaration of common helper function
void SetNZ(CPU * cpu, Byte reg);

#include "load.cpp"
#include "store.cpp"
#include "addcarry.cpp"
#include "subtract.cpp"
#include "transfer.cpp"
#include "stack.cpp"
#include "flags.cpp"
#include "incdec.cpp"
#include "logical.cpp"
#include "compare.cpp"
#include "branch.cpp"
#include "shifts.cpp"
#include "misc.cpp"

CPU::CPU(Memory *_mem): mem(_mem){

}

void BRK(CPU * cpu) {
    cpu->PC++;  // BRK has a dummy operand byte, skip it
    cpu->push(cpu->PC >> 8);
    cpu->push(cpu->PC & 0xFF);
    cpu->push(cpu->P | 0x30);  // B flag and unused flag (bits 4 & 5) are set when pushed
    cpu->PC = cpu->mem->read16(0XFFFE);
    cpu->setI(true);  // Set interrupt disable flag
}

// Common helper function for setting N and Z flags
void SetNZ(CPU * cpu, Byte reg) {
    cpu->setZ(reg == 0);
    cpu->setN((reg & 0b10000000) > 0);
}


#define re mem->read
#define wr mem->write
#define CYCL cycl()

//Instructions
void (*functptr[256])(CPU *) = {
        //          0        1       2       3       4       5       6       7       8       9       A       B       C       D       E       F
        /*0*/       &BRK,    &ORAIX, 0,      0,      0,      &ORAZP, &ASLZP, 0,      &PHP,   &ORAI,  &ASLA,  0,      0,      &ORAA,  &ASLABS,0,
        /*1*/       &BPL,    &ORAIY, 0,      0,      0,      &ORAZPX,&ASLZPX,0,      &CLC,   &ORAAY, 0,      0,      0,      &ORAAX, &ASLABSX,0,
        /*2*/       &JSR,    &ANDIX, 0,      0,      &BITZP, &ANDZP, &ROLZP, 0,      &PLP,   &ANDI,  &ROLA,  0,      &BITABS,&ANDA,  &ROLABS,0,
        /*3*/       &BMI,    &ANDIY, 0,      0,      0,      &ANDZPX,&ROLZPX,0,      &SEC,   &ANDAY, 0,      0,      0,      &ANDAX, &ROLABSX,0,
        /*4*/       &RTI,    &EORIX, 0,      0,      0,      &EORZP, &LSRZP, 0,      &PHA,   &EORI,  &LSRA,  0,      &JMPABS,&EORA,  &LSRABS,0,
        /*5*/       &BVC,    &EORIY, 0,      0,      0,      &EORZPX,&LSRZPX,0,      &CLI,   &EORAY, 0,      0,      0,      &EORAX, &LSRABSX,0,
        /*6*/       &RTS,    &ADCIX, 0,      0,      0,      &ADCZ,  &RORZP, 0,      &PLA,   &ADCI,  &RORA,  0,      &JMPIND,&ADCA,  &RORABS,0,
        /*7*/       &BVS,    &ADCIY, 0,      0,      0,      &ADCZX, &RORZPX,0,      &SEI,   &ADCAY, 0,      0,      0,      &ADCAX, &RORABSX,0,
        /*8*/       0,       &STAIX, 0,      0,      &STYZP, &STAZ,  &STXZP, 0,      &DEY,   0,      &TXA,   0,      &STYA,  &STAA,  &STXA,  0,
        /*9*/       &BCC,    &STAIY, 0,      0,      &STYZPX,&STAZX, &STXZPY,0,      &TYA,   &STAAY, &TXS,   0,      0,      &STAAX, 0,      0,
        /*A*/       &LDYI,   &LDAIX, &LDXI,  0,      &LDYZP, &LDAZ,  &LDXZP, 0,      &TAY,   &LDAI,  &TAX,   0,      &LDYA,  &LDAA,  &LDXA,  0,
        /*B*/       &BCS,    &LDAIY, 0,      0,      &LDYZPY,&LDAZX, &LDXZPY,0,      &CLV,   &LDAAY, &TSX,   0,      &LDYAY, &LDAAX, &LDXAY, 0,
        /*C*/       &CPYI,   &CMPIX, 0,      0,      &CPYZP, &CMPZP, &DECZP, 0,      &INY,   &CMPI,  &DEX,   0,      &CPYA,  &CMPA,  &DECA,  0,
        /*D*/       &BNE,    &CMPIY, 0,      0,      0,      &CMPZPX,&DECZPX,0,      &CLD,   &CMPAY, 0,      0,      0,      &CMPAX, &DECAX, 0,
        /*E*/       &CPXI,   &SBCIX, 0,      0,      &CPXZP, &SBCZP, &INCZP, 0,      &INX,   &SBCI,  &NOP,   0,      &CPXA,  &SBCA,  &INCA,  0,
        /*F*/       &BEQ,    &SBCIY, 0,      0,      0,      &SBCZPX,&INCZPX,0,      &SED,   &SBCAY, 0,      0,      0,      &SBCAX, &INCAX, 0
        } ;


void CPU::reset()
{
    A = 0X0;
    X = 0X0;
    Y = 0X0;
    P = 0X0;

    SP = 0X0;
    CYCL;
    CYCL;
    CYCL;
    SP--;
    CYCL;
    SP--;
    CYCL;
    SP--;
    CYCL;
    PC = mem->read16(0XFFFC);
    CYCL;

}

void CPU::execute()
{
    Byte instruction = mem->read(PC++);
    functptr[instruction](this);
}

void CPU::cycl() {
    cycles++;
}

void CPU::push(uint8_t v) {
    mem->write(0x100+SP, v);
    SP--;
}

uint8_t CPU::pop() {
    return mem->read(0x100+SP);
    SP++;
}

void CPU::pushPC() {

}

uint8_t CPU::popPC() {

}


Word CPU::immediate() {
    return PC++;
}

Byte CPU::zeroPage() {
    CYCL;
    addr8 = re(PC++);
    return addr8;
}

Byte CPU::zeroPageX() {
    CYCL;
    addr16 = re(PC++);
    CYCL;
    addr16 += X;
    return addr16;
}

Byte CPU::zeroPageY() {
    CYCL;
    addr16 = re(PC++);
    CYCL;
    addr16 += Y;
    return addr16;
}

Word CPU::absolute() {
    Word addr = mem->read16(PC);
    CYCL; PC++;
    CYCL; PC++;
    return addr;
}

Word CPU::absoluteX() {
    CYCL;
    addr16 = re(PC++) + X;
    if (addr16 > 0xFF) CYCL;
    CYCL;
    addr16 += re(PC++) << 8;
    return addr16;
}

Word CPU::absoluteY() {
    CYCL;
    addr16 = re(PC++) + Y;
    if (addr16 > 0xFF) CYCL;
    CYCL;
    addr16 += re(PC++) << 8;
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
    addr16 += re(addr8+1) << 8;
    return addr16;
}

Word CPU::indirectY() {
    CYCL;
    addr8 = re(PC++);
    CYCL;
    addr16 = re(addr8);
    CYCL;
    addr16 += re(addr8+1) << 8;
    CYCL;
    addr16 += Y;
    if (addr16 > 0xFF) CYCL;
    return addr16;
}

void CPU::setN(bool f){
    int pos = 7;
    P  = P & ~(1 << pos);
    P |= (f << pos);
}

void CPU::setV(bool f){
    int pos = 6;
    P  = P & ~(1 << pos);
    P |= (f << pos);
}

void CPU::setB(bool f){
    int pos = 4;
    P  = P & ~(1 << pos);
    P |= (f << pos);
}

void CPU::setD(bool f){
    int pos = 3;
    P  = P & ~(1 << pos);
    P |= (f << pos);
}

void CPU::setI(bool f){
    int pos = 2;
    P  = P & ~(1 << pos);
    P |= (f << pos);
}

void CPU::setZ(bool f){
    int pos = 1;
    P  = P & ~(1 << pos);
    P |= (f << pos);
}


void CPU::setC(bool f){
    int pos = 0;
    P  = P & ~(1 << pos);
    P |= (f << pos);

}


