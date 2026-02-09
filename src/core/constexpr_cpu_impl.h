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

    // LDA Instructions
    static constexpr void LDA_IMM(ConstexprCPUState& state) {
        Word addr = getImmediateAddr(state);
        state.A = state.memory.read(addr);
        state.cycl();
        state.setNZ(state.A);
    }

    static constexpr void LDA_ZP(ConstexprCPUState& state) {
        Byte addr = getZeroPageAddr(state);
        state.A = state.memory.read(addr);
        state.cycl();
        state.setNZ(state.A);
    }

    static constexpr void LDA_ZPX(ConstexprCPUState& state) {
        Byte addr = getZeroPageXAddr(state);
        state.A = state.memory.read(addr);
        state.cycl();
        state.setNZ(state.A);
    }

    static constexpr void LDA_ABS(ConstexprCPUState& state) {
        Word addr = getAbsoluteAddr(state);
        state.A = state.memory.read(addr);
        state.cycl();
        state.setNZ(state.A);
    }

    static constexpr void LDA_ABSX(ConstexprCPUState& state) {
        bool pageCrossed = false;
        Word addr = getAbsoluteXAddr(state, pageCrossed);
        state.A = state.memory.read(addr);
        state.cycl();
        state.setNZ(state.A);
    }

    static constexpr void LDA_ABSY(ConstexprCPUState& state) {
        bool pageCrossed = false;
        Word addr = getAbsoluteYAddr(state, pageCrossed);
        state.A = state.memory.read(addr);
        state.cycl();
        state.setNZ(state.A);
    }

    static constexpr void LDA_INDX(ConstexprCPUState& state) {
        Word addr = getIndirectXAddr(state);
        state.A = state.memory.read(addr);
        state.cycl();
        state.setNZ(state.A);
    }

    static constexpr void LDA_INDY(ConstexprCPUState& state) {
        bool pageCrossed = false;
        Word addr = getIndirectYAddr(state, pageCrossed);
        state.A = state.memory.read(addr);
        state.cycl();
        state.setNZ(state.A);
    }

    // STA Instructions
    static constexpr void STA_ZP(ConstexprCPUState& state) {
        Byte addr = getZeroPageAddr(state);
        state.memory.write(addr, state.A);
        state.cycl();
    }

    static constexpr void STA_ZPX(ConstexprCPUState& state) {
        Byte addr = getZeroPageXAddr(state);
        state.memory.write(addr, state.A);
        state.cycl();
    }

    static constexpr void STA_ABS(ConstexprCPUState& state) {
        Word addr = getAbsoluteAddr(state);
        state.memory.write(addr, state.A);
        state.cycl();
    }

    static constexpr void STA_ABSX(ConstexprCPUState& state) {
        bool pageCrossed = false;
        Word addr = getAbsoluteXAddr(state, pageCrossed);
        state.memory.write(addr, state.A);
        state.cycl();
    }

    static constexpr void STA_ABSY(ConstexprCPUState& state) {
        bool pageCrossed = false;
        Word addr = getAbsoluteYAddr(state, pageCrossed);
        state.memory.write(addr, state.A);
        state.cycl();
    }

    static constexpr void STA_INDX(ConstexprCPUState& state) {
        Word addr = getIndirectXAddr(state);
        state.memory.write(addr, state.A);
        state.cycl();
    }

    static constexpr void STA_INDY(ConstexprCPUState& state) {
        bool pageCrossed = false;
        Word addr = getIndirectYAddr(state, pageCrossed);
        state.memory.write(addr, state.A);
        state.cycl();
    }

    // JMP Instructions
    static constexpr void JMP_ABS(ConstexprCPUState& state) {
        Word addr = state.memory.read16(state.PC);
        state.PC = addr;
        state.cycl();
        state.cycl();
        state.cycl();
    }

    static constexpr void JMP_IND(ConstexprCPUState& state) {
        Word indAddr = state.memory.read16(state.PC);
        state.cycl();
        state.cycl();
        
        // Handle 6502 bug where indirect JMP doesn't cross page boundaries
        Byte low = state.memory.read(indAddr);
        Byte high = state.memory.read((indAddr & 0xFF00) | ((indAddr + 1) & 0x00FF));
        state.PC = (high << 8) | low;
        state.cycl();
        state.cycl();
        state.cycl();
    }

    // NOP
    static constexpr void NOP(ConstexprCPUState& state) {
        state.cycl();
        state.cycl();
    }

    // BRK
    static constexpr void BRK(ConstexprCPUState& state) {
        state.PC++;
        state.push(state.PC >> 8);
        state.push(state.PC & 0xFF);
        state.push(state.P | 0x30);
        state.PC = state.memory.read16(0xFFFE);
        state.setI(true);
        state.cycl();
    }

    // Execute a single instruction
    static constexpr ConstexprCPUState executeInstruction(ConstexprCPUState state) {
        Byte opcode = state.memory.read(state.PC++);
        state.cycl();
        
        // Decode and execute instruction
        switch (opcode) {
            // LDA
            case 0xA9: LDA_IMM(state); break;
            case 0xA5: LDA_ZP(state); break;
            case 0xB5: LDA_ZPX(state); break;
            case 0xAD: LDA_ABS(state); break;
            case 0xBD: LDA_ABSX(state); break;
            case 0xB9: LDA_ABSY(state); break;
            case 0xA1: LDA_INDX(state); break;
            case 0xB1: LDA_INDY(state); break;
            
            // STA
            case 0x85: STA_ZP(state); break;
            case 0x95: STA_ZPX(state); break;
            case 0x8D: STA_ABS(state); break;
            case 0x9D: STA_ABSX(state); break;
            case 0x99: STA_ABSY(state); break;
            case 0x81: STA_INDX(state); break;
            case 0x91: STA_INDY(state); break;
            
            // JMP
            case 0x4C: JMP_ABS(state); break;
            case 0x6C: JMP_IND(state); break;
            
            // NOP
            case 0xEA: NOP(state); break;
            
            // BRK
            case 0x00: BRK(state); break;
            
            default:
                // Unknown instruction - just consume a cycle
                state.cycl();
                break;
        }
        
        return state;
    }

    // Execute until max cycles or halt
    static constexpr ConstexprCPUState executeUntil(ConstexprCPUState state, __int128 maxCycles) {
        Word prevPC = state.PC;
        int sameCount = 0;
        
        while (state.cycles < maxCycles) {
            Word currentPC = state.PC;
            state = executeInstruction(state);
            
            // Detect infinite loops
            if (state.PC == prevPC) {
                sameCount++;
                if (sameCount > 5) {
                    break;  // Infinite loop detected
                }
            } else {
                sameCount = 0;
                prevPC = currentPC;
            }
        }
        
        return state;
    }

    // Reset the CPU
    static constexpr ConstexprCPUState reset(ConstexprCPUState state) {
        state.A = 0x00;
        state.X = 0x00;
        state.Y = 0x00;
        state.P = 0x00;
        state.SP = 0x00;
        
        state.cycl();
        state.cycl();
        state.cycl();
        state.SP--;
        state.cycl();
        state.SP--;
        state.cycl();
        state.SP--;
        state.cycl();
        
        state.PC = state.memory.read16(0xFFFC);
        state.cycl();
        
        return state;
    }
};
