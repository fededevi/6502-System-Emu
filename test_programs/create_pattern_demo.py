#!/usr/bin/env python3
"""
Simple pattern generator for 6502 CPU graphical display demo.
Creates a binary that generates animated patterns on the display.
"""

import struct

# Create binary program that writes patterns to video memory
program = bytearray()

# Video display is at $0200-$05FF (32x32 = 1024 bytes)
DISPLAY_START = 0x0200
DISPLAY_SIZE = 1024

# Program starts at $0600
ORG = 0x0600

def add_byte(b):
    program.append(b)

def add_word(w):
    program.append(w & 0xFF)
    program.append((w >> 8) & 0xFF)

# Create a simple program that fills the display with a pattern
# and animates it

# Start: Initialize
# LDA #$00 (A9 00) - Load accumulator with 0
add_byte(0xA9)
add_byte(0x00)

# STA $10 (85 10) - Store in zero page $10 (pattern counter)
add_byte(0x85)
add_byte(0x10)

# MAIN_LOOP:
main_loop_addr = len(program) + ORG

# LDA #$00 (A9 00) - Load low byte of display address
add_byte(0xA9)
add_byte(0x00)

# STA $20 (85 20) - Store in zero page $20
add_byte(0x85)
add_byte(0x20)

# LDA #$02 (A9 02) - Load high byte of display address
add_byte(0xA9)
add_byte(0x02)

# STA $21 (85 21) - Store in zero page $21
add_byte(0x85)
add_byte(0x21)

# LDX #$00 (A2 00) - Initialize X to 0
add_byte(0xA2)
add_byte(0x00)

# LDY #$00 (A0 00) - Initialize Y to 0
add_byte(0xA0)
add_byte(0x00)

# FILL_LOOP:
fill_loop_addr = len(program) + ORG

# TYA (98) - Transfer Y to A
add_byte(0x98)

# CLC (18) - Clear carry
add_byte(0x18)

# ADC $10 (65 10) - Add pattern counter
add_byte(0x65)
add_byte(0x10)

# ADC $20 (65 20) - Add X position for pattern
add_byte(0x65)
add_byte(0x20)

# STA ($20),Y (91 20) - Store in display memory
add_byte(0x91)
add_byte(0x20)

# INY (C8) - Increment Y
add_byte(0xC8)

# BNE FILL_LOOP (D0 F0 = -16 relative)
add_byte(0xD0)
rel_offset = fill_loop_addr - (len(program) + ORG + 1)
add_byte(rel_offset & 0xFF)

# INC $21 (E6 21) - Increment high byte of pointer
add_byte(0xE6)
add_byte(0x21)

# INX (E8) - Increment X
add_byte(0xE8)

# CPX #$04 (E0 04) - Compare X with 4 (1024 bytes = 4 pages)
add_byte(0xE0)
add_byte(0x04)

# BNE FILL_LOOP (D0 E3 = -29 relative)
add_byte(0xD0)
rel_offset = fill_loop_addr - (len(program) + ORG + 1)
add_byte(rel_offset & 0xFF)

# DELAY: Simple delay loop
delay_addr = len(program) + ORG

# LDX #$00 (A2 00)
add_byte(0xA2)
add_byte(0x00)

# LDY #$00 (A0 00)
add_byte(0xA0)
add_byte(0x00)

# DELAY_INNER:
delay_inner_addr = len(program) + ORG

# NOP (EA) x 4
add_byte(0xEA)
add_byte(0xEA)
add_byte(0xEA)
add_byte(0xEA)

# INY (C8)
add_byte(0xC8)

# BNE DELAY_INNER (D0 F8 = -8 relative)
add_byte(0xD0)
rel_offset = delay_inner_addr - (len(program) + ORG + 1)
add_byte(rel_offset & 0xFF)

# INX (E8)
add_byte(0xE8)

# CPX #$20 (E0 20) - Delay for 32 iterations
add_byte(0xE0)
add_byte(0x20)

# BNE DELAY_INNER (D0 F2 = -14 relative)
add_byte(0xD0)
rel_offset = delay_inner_addr - (len(program) + ORG + 1)
add_byte(rel_offset & 0xFF)

# INC $10 (E6 10) - Increment pattern counter
add_byte(0xE6)
add_byte(0x10)

# JMP MAIN_LOOP (4C addr_low addr_high)
add_byte(0x4C)
add_word(main_loop_addr)

# Create full binary with padding
binary = bytearray(65536)  # 64KB

# Place program at ORG
binary[ORG:ORG+len(program)] = program

# Write binary to file
with open('test_programs/pattern_demo.bin', 'wb') as f:
    f.write(binary)

print(f"Created pattern_demo.bin ({len(binary)} bytes)")
print(f"Program size: {len(program)} bytes")
print(f"Load at: $0000")
print(f"Start at: ${ORG:04X}")
print()
print("Run with: ./build/6502_emu -f test_programs/pattern_demo.bin -pc 0600 -g -m 10000000")
