.nolist
.include "m328pdef.inc"
.list

;==============
; Declarations:

.def temp = r16
.def overflows = r17

.org 0x0000              ; memory (PC) location of reset handler
rjmp Reset               ; jmp costs 2 cpu cycles and rjmp costs only 1
                         ; so unless you need to jump more than 8k bytes
                         ; you only need rjmp. Some microcontrollers therefore only 
                         ; have rjmp and not jmp
.org OVF0addr            ; memory location of Timer0 overflow handler
rjmp overflow_handler    ; go here if a timer0 overflow interrupt occurs 

.org 0x0100     ; start of program
;============

Reset: 
   ldi temp,  0b00000101
   out TCCR0B, temp      ; set the Clock Selector Bits CS00, CS01, CS02 to 101
                         ; this puts Timer Counter0, TCNT0 in to FCPU/1024 mode
                         ; so it ticks at the CPU freq/1024
   ldi temp, 0b00000001
   sts TIMSK0, temp      ; set the Timer Overflow Interrupt Enable (TOIE0) bit 
                         ; of the Timer Interrupt Mask Register (TIMSK0)

   sei                   ; enable global interrupts -- equivalent to "sbi SREG, I"

   clr temp
   out TCNT0, temp       ; initialize the Timer/Counter to 0

   sbi DDRB, 5           ; set PB5 to output

;======================
; Main body of program:

blink:
   sbi PORTB, 5          ; turn on LED on PB5
   rcall delay           ; delay will be 1/2 second
   cbi PORTB, 5          ; turn off LED on PB5
   rcall delay           ; delay will be 1/2 second
   rjmp blink            ; loop back to the start
  
delay:
   clr overflows         ; set overflows to 0 
   sec_count:
     cpi overflows,30    ; compare number of overflows and 30
   brne sec_count        ; branch to back to sec_count if not equal 
   ret                   ; if 30 overflows have occured return to blink

overflow_handler: 
   inc overflows         ; add 1 to the overflows variable
   cpi overflows, 61     ; compare with 61
   brne exit             ; Program Counter + 2 (skip next line) if not equal
   clr overflows         ; if 61 overflows occured reset the counter to zero
exit:
   reti                  ; return from interrupt
