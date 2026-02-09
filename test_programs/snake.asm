; Simple Snake Game for 6502 CPU Emulator
; Display: 32x32 at $0200-$05FF
; Controls: Automatic movement (demo mode)
;
; Memory Map:
; $0200-$05FF: Video memory (32x32 display)
; $0000-$0001: Snake head position (X, Y)
; $0002-$0003: Apple position (X, Y)
; $0004: Direction (0=right, 1=down, 2=left, 3=up)
; $0005: Snake length
; $0010-$01FF: Snake body positions (X, Y pairs)

; Constants
DISPLAY     = $0200
DISPLAY_W   = 32
DISPLAY_H   = 32

SNAKE_X     = $00
SNAKE_Y     = $01
APPLE_X     = $02
APPLE_Y     = $03
DIRECTION   = $04
SNAKE_LEN   = $05
SNAKE_BODY  = $10

CHAR_EMPTY  = $00
CHAR_SNAKE  = $01
CHAR_APPLE  = $02

; Start of program
        .org $0600

start:
        ; Initialize
        jsr init_game
        
main_loop:
        ; Clear display
        jsr clear_display
        
        ; Update game state
        jsr update_snake
        
        ; Draw snake
        jsr draw_snake
        
        ; Draw apple
        jsr draw_apple
        
        ; Delay
        jsr delay
        
        ; Loop forever
        jmp main_loop

init_game:
        ; Set initial snake position (center of screen)
        lda #16
        sta SNAKE_X
        sta SNAKE_Y
        
        ; Set initial direction (right)
        lda #0
        sta DIRECTION
        
        ; Set initial length
        lda #3
        sta SNAKE_LEN
        
        ; Set initial apple position
        lda #10
        sta APPLE_X
        lda #10
        sta APPLE_Y
        
        rts

clear_display:
        lda #<DISPLAY
        sta $10
        lda #>DISPLAY
        sta $11
        
        ldx #0
        ldy #0
        lda #CHAR_EMPTY
        
clear_loop:
        sta ($10),y
        iny
        bne clear_loop
        inc $11
        inx
        cpx #4          ; 4 pages (1024 bytes)
        bne clear_loop
        
        rts

update_snake:
        ; Move snake based on direction
        lda DIRECTION
        cmp #0
        beq move_right
        cmp #1
        beq move_down
        cmp #2
        beq move_left
        cmp #3
        beq move_up
        rts

move_right:
        inc SNAKE_X
        lda SNAKE_X
        cmp #DISPLAY_W
        bcc check_collision
        lda #0
        sta SNAKE_X
        jmp check_collision

move_down:
        inc SNAKE_Y
        lda SNAKE_Y
        cmp #DISPLAY_H
        bcc check_collision
        lda #0
        sta SNAKE_Y
        jmp check_collision

move_left:
        lda SNAKE_X
        beq wrap_left
        dec SNAKE_X
        jmp check_collision
wrap_left:
        lda #DISPLAY_W-1
        sta SNAKE_X
        jmp check_collision

move_up:
        lda SNAKE_Y
        beq wrap_up
        dec SNAKE_Y
        jmp check_collision
wrap_up:
        lda #DISPLAY_H-1
        sta SNAKE_Y
        ; Fall through to check_collision

check_collision:
        ; Check if snake ate apple
        lda SNAKE_X
        cmp APPLE_X
        bne no_apple
        lda SNAKE_Y
        cmp APPLE_Y
        bne no_apple
        
        ; Grow snake
        inc SNAKE_LEN
        
        ; New apple position (simple pseudo-random)
        lda SNAKE_X
        clc
        adc SNAKE_Y
        and #31
        sta APPLE_X
        
        lda SNAKE_Y
        clc
        adc SNAKE_X
        lsr
        and #31
        sta APPLE_Y
        
no_apple:
        ; Change direction periodically (demo mode)
        lda $0600       ; Use program counter area as pseudo-random
        and #63
        bne no_turn
        
        ; Turn right
        inc DIRECTION
        lda DIRECTION
        and #3
        sta DIRECTION
        
no_turn:
        rts

draw_snake:
        ; Calculate display address for snake head
        lda SNAKE_Y
        
        ; Multiply Y by 32 (shift left 5 times)
        asl
        asl
        asl
        asl
        asl
        
        clc
        adc SNAKE_X
        
        ; Add display base address
        clc
        adc #<DISPLAY
        sta $10
        lda #>DISPLAY
        adc #0
        sta $11
        
        ; Draw snake head
        lda #CHAR_SNAKE
        ldy #0
        sta ($10),y
        
        rts

draw_apple:
        ; Calculate display address for apple
        lda APPLE_Y
        
        ; Multiply Y by 32
        asl
        asl
        asl
        asl
        asl
        
        clc
        adc APPLE_X
        
        ; Add display base address
        clc
        adc #<DISPLAY
        sta $10
        lda #>DISPLAY
        adc #0
        sta $11
        
        ; Draw apple
        lda #CHAR_APPLE
        ldy #0
        sta ($10),y
        
        rts

delay:
        ; Simple delay loop
        ldx #0
        ldy #0
delay_loop:
        nop
        nop
        nop
        nop
        iny
        bne delay_loop
        inx
        cpx #10
        bne delay_loop
        rts

        ; Pad to ensure we have space
        .org $0FFF
        .byte $00
