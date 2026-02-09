#pragma once

#include "types.h"
#include "constexpr_memory.h"

// Constexpr-compatible CPU state for compile-time optimization
struct ConstexprCPUState
{
    Word PC = 0;
    Byte SP = 0;
    Byte A = 0;
    Byte X = 0;
    Byte Y = 0;
    Byte P = 0;  // Status flags
    __int128 cycles = 0;
    ConstexprMemory memory;

    constexpr ConstexprCPUState() = default;
    constexpr ConstexprCPUState(const ConstexprMemory& mem) : memory(mem) {}

    // Flag accessors
    constexpr bool N() const { return (P & (1 << 7)) != 0; }
    constexpr bool V() const { return (P & (1 << 6)) != 0; }
    constexpr bool B() const { return (P & (1 << 4)) != 0; }
    constexpr bool D() const { return (P & (1 << 3)) != 0; }
    constexpr bool I() const { return (P & (1 << 2)) != 0; }
    constexpr bool Z() const { return (P & (1 << 1)) != 0; }
    constexpr bool C() const { return (P & (1 << 0)) != 0; }

    constexpr void setN(bool value) { P = value ? (P | (1 << 7)) : (P & ~(1 << 7)); }
    constexpr void setV(bool value) { P = value ? (P | (1 << 6)) : (P & ~(1 << 6)); }
    constexpr void setB(bool value) { P = value ? (P | (1 << 4)) : (P & ~(1 << 4)); }
    constexpr void setD(bool value) { P = value ? (P | (1 << 3)) : (P & ~(1 << 3)); }
    constexpr void setI(bool value) { P = value ? (P | (1 << 2)) : (P & ~(1 << 2)); }
    constexpr void setZ(bool value) { P = value ? (P | (1 << 1)) : (P & ~(1 << 1)); }
    constexpr void setC(bool value) { P = value ? (P | (1 << 0)) : (P & ~(1 << 0)); }

    constexpr void cycl() { cycles++; }

    // Stack operations
    constexpr void push(Byte value) {
        memory.write(0x100 + SP, value);
        SP--;
        cycl();
    }

    constexpr Byte pop() {
        SP++;
        cycl();
        return memory.read(0x100 + SP);
    }

    // Helper functions
    constexpr void setNZ(Byte value) {
        setZ(value == 0);
        setN((value & 0x80) != 0);
    }
};

// Constexpr CPU executor
class ConstexprCPU
{
public:
    // Execute a single instruction and return new state
    static constexpr ConstexprCPUState executeInstruction(ConstexprCPUState state);

    // Execute until a certain number of cycles or halt condition
    static constexpr ConstexprCPUState executeUntil(ConstexprCPUState state, __int128 maxCycles);

    // Reset the CPU
    static constexpr ConstexprCPUState reset(ConstexprCPUState state);

private:
    // Addressing mode helpers
    static constexpr Word getImmediateAddr(ConstexprCPUState& state) {
        return state.PC++;
    }

    static constexpr Byte getZeroPageAddr(ConstexprCPUState& state) {
        Byte addr = state.memory.read(state.PC++);
        state.cycl();
        return addr;
    }

    static constexpr Byte getZeroPageXAddr(ConstexprCPUState& state) {
        Byte addr = state.memory.read(state.PC++);
        state.cycl();
        addr = (addr + state.X) & 0xFF;
        state.cycl();
        return addr;
    }

    static constexpr Byte getZeroPageYAddr(ConstexprCPUState& state) {
        Byte addr = state.memory.read(state.PC++);
        state.cycl();
        addr = (addr + state.Y) & 0xFF;
        state.cycl();
        return addr;
    }

    static constexpr Word getAbsoluteAddr(ConstexprCPUState& state) {
        Word addr = state.memory.read16(state.PC);
        state.PC += 2;
        state.cycl();
        state.cycl();
        return addr;
    }

    static constexpr Word getAbsoluteXAddr(ConstexprCPUState& state, bool& pageCrossed) {
        Word addr = state.memory.read16(state.PC);
        state.PC += 2;
        state.cycl();
        state.cycl();
        Word finalAddr = addr + state.X;
        pageCrossed = ((addr & 0xFF00) != (finalAddr & 0xFF00));
        if (pageCrossed) state.cycl();
        return finalAddr;
    }

    static constexpr Word getAbsoluteYAddr(ConstexprCPUState& state, bool& pageCrossed) {
        Word addr = state.memory.read16(state.PC);
        state.PC += 2;
        state.cycl();
        state.cycl();
        Word finalAddr = addr + state.Y;
        pageCrossed = ((addr & 0xFF00) != (finalAddr & 0xFF00));
        if (pageCrossed) state.cycl();
        return finalAddr;
    }

    static constexpr Word getIndirectXAddr(ConstexprCPUState& state) {
        Byte zpAddr = state.memory.read(state.PC++);
        state.cycl();
        zpAddr = (zpAddr + state.X) & 0xFF;
        state.cycl();
        Word addr = state.memory.read(zpAddr) | (state.memory.read((zpAddr + 1) & 0xFF) << 8);
        state.cycl();
        state.cycl();
        return addr;
    }

    static constexpr Word getIndirectYAddr(ConstexprCPUState& state, bool& pageCrossed) {
        Byte zpAddr = state.memory.read(state.PC++);
        state.cycl();
        Word addr = state.memory.read(zpAddr) | (state.memory.read((zpAddr + 1) & 0xFF) << 8);
        state.cycl();
        state.cycl();
        Word finalAddr = addr + state.Y;
        pageCrossed = ((addr & 0xFF00) != (finalAddr & 0xFF00));
        if (pageCrossed) state.cycl();
        return finalAddr;
    }

    // Instruction implementations (to be defined)
    static constexpr void LDA_IMM(ConstexprCPUState& state);
    static constexpr void LDA_ZP(ConstexprCPUState& state);
    static constexpr void LDA_ZPX(ConstexprCPUState& state);
    static constexpr void LDA_ABS(ConstexprCPUState& state);
    static constexpr void LDA_ABSX(ConstexprCPUState& state);
    static constexpr void LDA_ABSY(ConstexprCPUState& state);
    static constexpr void LDA_INDX(ConstexprCPUState& state);
    static constexpr void LDA_INDY(ConstexprCPUState& state);

    static constexpr void STA_ZP(ConstexprCPUState& state);
    static constexpr void STA_ZPX(ConstexprCPUState& state);
    static constexpr void STA_ABS(ConstexprCPUState& state);
    static constexpr void STA_ABSX(ConstexprCPUState& state);
    static constexpr void STA_ABSY(ConstexprCPUState& state);
    static constexpr void STA_INDX(ConstexprCPUState& state);
    static constexpr void STA_INDY(ConstexprCPUState& state);

    static constexpr void JMP_ABS(ConstexprCPUState& state);
    static constexpr void JMP_IND(ConstexprCPUState& state);
    
    static constexpr void NOP(ConstexprCPUState& state);
    static constexpr void BRK(ConstexprCPUState& state);
};
