.nolist
.include "m328pdef.inc"
.list

;==============
; Declarations:

.def temp = r16
.def overflows = r17
.def milliseconds = r20

.macro delay
   clr overflows
   ldi milliseconds,@0
sec_count:
   cpse overflows, milliseconds
   rjmp sec_count
.endmacro

.org 0x0000              ; memory (PC) location of reset handler
rjmp Reset               ; jmp costs 2 cpu cycles and rjmp costs only 1
                         ; so unless you need to jump more than 8k bytes
                         ; you only need rjmp. Some microcontrollers therefore only 
                         ; have rjmp and not jmp
.org 0x0020              ; memory location of Timer0 overflow handler
rjmp overflow_handler    ; go here if a timer0 overflow interrupt occurs 

.org 0x0100              ; start of program
;============

Reset: 
   ldi temp,0b00000011
   out TCCR0B,temp     ; TCNT0 in FCPU/64 mode, 2500000 cnts/sec
   ldi temp,249
   out OCR0A, temp     ; top of counter so 250 counts/overflow
                       ;   so overflow occurs every 1/1000 sec
                       ;   this means an overflow every 1ms
   ldi temp,0b00000010
   out TCCR0A,temp     ; reset TCNT0 at top of OCR0A
   sts TIMSK0, temp    ; Enable Timer Overflow Interrupts
   sei                 ; enable global interrupts

   sbi DDRB, 5           ; set PB5 to output

;======================
; Main body of program:

blink:
   sbi PORTB, 5          ; turn on LED on PB5
   delay 250             ; delay will be 1/2 second
   cbi PORTB, 5          ; turn off LED on PB5
   delay 250             ; delay will be 1/2 second
   rjmp blink            ; loop back to the start
  
overflow_handler: 
   inc overflows         ; add 1 to the overflows variable
   reti                  ; return from interrupt
