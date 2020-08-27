; These are all constant definitions taken from m328Pdef.inc
; and we will go in more detail of what they are, when they
; are used
;.equ PORTB = 0x05
;.equ PORTD = 0x0b
;.equ PIND  = 0x09
;.equ DDRB  = 0x04
;.equ DDRD  = 0x0a

.include "m328pdef.inc"

.ORG 0x0000             ; the next instruction has to be written to
                        ; address 0x0000
    rjmp START          ; the reset vector: jump to "main"

START:
    ldi r16, low(RAMEND)   ; set up the stack
    out SPL, r16
    ldi r16, high(RAMEND)
    out SPH, r16
    ldi r16, 0xFF          ; load register 16 with 0xFF (all bits 1)
    out DDRB, r16          ; write the value in r16 (0xFF) to Data
                           ; Direction Register B (DDR B) = all output
LOOP:
    sbi PORTB, 5           ; switch on the LED PORTB = 8 - 13
    rcall DELAY_05         ; wait for half a secon
    cbi PORTB, 5           ; switch it off
    rcall DELAY_20         ; wait for 2 seconds
    rjmp LOOP              ; jump to loop

DELAY_05:                  ; the subroutine:
    ldi r16, 31            ; load r16 with 31

LOOPA1:                ; outer loop label
    ldi r24, low(1021)     ; load registers r24:r25 with 1021, our new
                           ; init value  
    ldi r25, high(1021)    ; the loop label

LOOPA2:                     ; "add immediate to word": r24:r25 are
    adiw r24, 1            ; if no overflow go back to "delay_loop"  
    brne LOOPA2             ; branch if not equal 0 
    dec r16                ; decrement r16  
    brne LOOPA1             ; and loop if outer loop not finished  
    ret                    ; return from subroutine

DELAY_20:                  ; the subroutine:
    ldi r17, 4
LOOPB1:
    rcall DELAY_05         ; wait for half a secon
    dec r17
    brne LOOPB1
    ret


