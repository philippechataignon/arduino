.nolist                
.include "m328pdef.inc"      
.list              

.org 0x000
    rjmp MAIN

.org INT0addr
    rjmp INT0HANDLE                

.org 0x0100
   
.def temp = r16

MAIN:          
    cli                
    ldi temp, LOW(RAMEND) ; setup stack
    out SPL, temp
    ldi temp, HIGH(RAMEND)
    out SPH, temp
    
    sbi DDRB, 5          ; set PB5 as output (pin 13)
    cbi PORTB, 5         ; set PB5 HIGH
    cbi DDRD, 2          ; set PD2 as input (pin 2)
    sbi PORTD, 2         ; set PD2 (cbi=FLOAT, sbi=PULLUP)
    
    ldi temp, 0b01  ; enable INT0
    out EIMSK, temp
    
    ldi temp, 0b10  ; mode change (falling = 10, rising = 11)
    sts EICRA, temp
    sei
       
LOOP:          
    rjmp LOOP
       
INT0HANDLE:    
	push temp
	in temp, SREG
	push temp

    ; inverse port B
	in temp, PORTB
	com temp
	out PORTB, temp

	; restore conflict registers
	pop temp
	out SREG, temp
	pop temp
	reti
