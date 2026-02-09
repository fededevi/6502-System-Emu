#!/usr/bin/env python3
"""
Simple animated pattern for 6502 CPU graphical display demo.
Creates moving colored blocks on the display.
"""

import struct

program = bytearray()
ORG = 0x0600
DISPLAY_START = 0x0200

def add_byte(b):
    program.append(b)

def add_word(w):
    program.append(w & 0xFF)
    program.append((w >> 8) & 0xFF)

# Initialize frame counter at $10
# LDA #$00
add_byte(0xA9)
add_byte(0x00)
# STA $10
add_byte(0x85)
add_byte(0x10)

# MAIN_LOOP:
main_loop_addr = len(program) + ORG

# Fill display with animated pattern
# Set pointer to display start
# LDA #$00
add_byte(0xA9)
add_byte(0x00)
# STA $20
add_byte(0x85)
add_byte(0x20)
# LDA #$02
add_byte(0xA9)
add_byte(0x02)
# STA $21
add_byte(0x85)
add_byte(0x21)

# Initialize Y register for rows
# LDY #$00
add_byte(0xA0)
add_byte(0x00)

# ROW_LOOP:
row_loop_addr = len(program) + ORG

# Initialize X register for columns
# LDX #$00
add_byte(0xA2)
add_byte(0x00)

# COL_LOOP:
col_loop_addr = len(program) + ORG

# Calculate pattern: (X + Y + frame) & 15
# TXA
add_byte(0x8A)
# CLC
add_byte(0x18)
# ADC $10 (add frame counter)
add_byte(0x65)
add_byte(0x10)
# Store in temp
# STA $11
add_byte(0x85)
add_byte(0x11)
# TYA
add_byte(0x98)
# CLC
add_byte(0x18)
# ADC $11
add_byte(0x65)
add_byte(0x11)
# AND #$0F (limit to 16 colors)
add_byte(0x29)
add_byte(0x0F)
# ORA #$10 (add brightness bit)
add_byte(0x09)
add_byte(0x10)

# Calculate offset: Y * 32 + X
# Save pattern value
# PHA
add_byte(0x48)
# Calculate Y * 32
# TYA
add_byte(0x98)
# ASL (x2)
add_byte(0x0A)
# ASL (x4)
add_byte(0x0A)
# ASL (x8)
add_byte(0x0A)
# ASL (x16)
add_byte(0x0A)
# ASL (x32)
add_byte(0x0A)
# CLC
add_byte(0x18)
# ADC $20 (add to base pointer low)
add_byte(0x65)
add_byte(0x20)
# STA $22 (store in temp pointer)
add_byte(0x85)
add_byte(0x22)
# LDA $21 (get high byte)
add_byte(0xA5)
add_byte(0x21)
# ADC #$00 (add carry)
add_byte(0x69)
add_byte(0x00)
# STA $23
add_byte(0x85)
add_byte(0x23)

# Add X offset
# TXA
add_byte(0x8A)
# CLC
add_byte(0x18)
# ADC $22
add_byte(0x65)
add_byte(0x22)
# STA $22
add_byte(0x85)
add_byte(0x22)
# LDA $23
add_byte(0xA5)
add_byte(0x23)
# ADC #$00
add_byte(0x69)
add_byte(0x00)
# STA $23
add_byte(0x85)
add_byte(0x23)

# Store pattern value
# PLA (get pattern value back)
add_byte(0x68)
# LDY #$00 (offset 0)
add_byte(0xA0)
add_byte(0x00)
# STA ($22),Y
add_byte(0x91)
add_byte(0x22)

# Restore Y (row counter) from stack
# We need to preserve Y differently
# Let's use a zero page location
# Actually, let's restructure to avoid this

# Increment X (column)
# INX
add_byte(0xE8)
# CPX #$20 (32 columns)
add_byte(0xE0)
add_byte(0x20)
# BNE COL_LOOP
add_byte(0xD0)
rel_offset = col_loop_addr - (len(program) + ORG + 1)
add_byte(rel_offset & 0xFF)

# Move to next row
# CLC
add_byte(0x18)
# LDA $20
add_byte(0xA5)
add_byte(0x20)
# ADC #$20 (add 32)
add_byte(0x69)
add_byte(0x20)
# STA $20
add_byte(0x85)
add_byte(0x20)
# LDA $21
add_byte(0xA5)
add_byte(0x21)
# ADC #$00
add_byte(0x69)
add_byte(0x00)
# STA $21
add_byte(0x85)
add_byte(0x21)

# Check if we're done with all rows (4 pages = 32 rows)
# LDA $21
add_byte(0xA5)
add_byte(0x21)
# CMP #$06 (display ends at $0600)
add_byte(0xC9)
add_byte(0x06)
# BCC ROW_LOOP
add_byte(0x90)
rel_offset = row_loop_addr - (len(program) + ORG + 1)
add_byte(rel_offset & 0xFF)

# Delay loop
# LDX #$00
add_byte(0xA2)
add_byte(0x00)
# DELAY1:
delay1_addr = len(program) + ORG
# LDY #$00
add_byte(0xA0)
add_byte(0x00)
# DELAY2:
delay2_addr = len(program) + ORG
# NOP
add_byte(0xEA)
# INY
add_byte(0xC8)
# BNE DELAY2
add_byte(0xD0)
rel_offset = delay2_addr - (len(program) + ORG + 1)
add_byte(rel_offset & 0xFF)
# INX
add_byte(0xE8)
# CPX #$08
add_byte(0xE0)
add_byte(0x08)
# BNE DELAY1
add_byte(0xD0)
rel_offset = delay1_addr - (len(program) + ORG + 1)
add_byte(rel_offset & 0xFF)

# Increment frame counter
# INC $10
add_byte(0xE6)
add_byte(0x10)

# JMP MAIN_LOOP
add_byte(0x4C)
add_word(main_loop_addr)

# Create full binary
binary = bytearray(65536)
binary[ORG:ORG+len(program)] = program

with open('test_programs/animated_pattern.bin', 'wb') as f:
    f.write(binary)

print(f"Created animated_pattern.bin ({len(binary)} bytes)")
print(f"Program size: {len(program)} bytes")
print(f"Load at: $0000")
print(f"Start at: ${ORG:04X}")
print()
print("Run with: ./build/6502_emu -f test_programs/animated_pattern.bin -pc 0600 -g -r 10 -m 100000000")
