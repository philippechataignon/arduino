;;;;
.nolist
.include "m328pdef.inc"
.list

.def temp = r16
.def char = r19
.def temp2 = r18
.def overflows = r17
.def milliseconds = r20
.def ndelay = r21

.equ fcpu = 16000000
.equ bitrate = 9600 ; OK -> 57600 ; KO 115200
.equ prescale = fcpu / 16 / bitrate - 1

;;;;
.macro sdelay
   clr overflows
   ldi milliseconds,@0
loop_sdelay:
   cpse overflows, milliseconds
   rjmp loop_sdelay
.endmacro

.macro delay
   ldi ndelay,@0
loop_delay:
   sdelay 250
   dec ndelay
   brne loop_delay
.endmacro

.cseg

;;;;
.org 0x0000             ; the next instruction has to be written to
                        ; address 0x0000
    rjmp    START          ; the reset vector: jump to "main"

.org 0x0020              ; memory location of Timer0 overflow handler
rjmp overflow_handler    ; go here if a timer0 overflow interrupt occurs 

.org INT_VECTORS_SIZE   ; the next instruction has to be written to


overflow_handler:
   inc overflows         ; add 1 to the overflows variable
   reti                  ; return from interrupt

puts:
    lpm     char,Z+              ; load character from pmem
    cpi     char,0               ; check if null
    breq    puts_end             ; branch if null
puts_wait:
    ; UCSR = USART Control and Status Register 
    ; UDRE = USART Data Register Empty
    lds     temp2,UCSR0A         ; load UCSR0A into temp2
    sbrs    temp2,UDRE0          ; wait for empty transmit buffer
                                 ; sbrs = skip next instruction 
                                 ; if bit UDRE0 of UCSR0A is set
    rjmp    puts_wait            ; repeat loop
    ; UDR = USART I/O Data Register 
    sts     UDR0,char            ; transmit character
    rjmp    puts                 ; repeat loop
puts_end:
    ret    

;;;;
START:
    ; put led13 off
    ser r16                ; load register 16 with 0xFF (all bits 1)
    out DDRB, r16          ; write the value in r16 (0xFF) to Data
    cbi PORTB, 5           ; switch on the LED PORTB = 8 - 13

    ; init timer
    ldi temp,0b00000011
    out TCCR0B,temp     ; TCNT0 in FCPU/64 mode, 2500000 cnts/sec
    ldi temp,249
    out OCR0A, temp     ; top of counter so 250 counts/overflow
                        ;   so overflow occurs every 1/1000 sec
                        ;   this means an overflow every 1ms

    ; prescale from bitrate
    ; UBRR = USART Baud Rate Registers
    ldi temp, low(prescale)
    sts UBRR0L, temp
    ldi temp, high(prescale)
    sts UBRR0H, temp
    ; active tx/rx
    ldi temp,0b00011000
    sts UCSR0B, temp
    ; 8 bits
    ldi temp,0b00000110
    sts UCSR0C, temp
    
    ldi temp,0b00000010
    out TCCR0A,temp     ; reset TCNT0 at top of OCR0A
    sts TIMSK0, temp    ; Enable Timer Overflow Interrupts
    sei                 ; enable global interrupts
loop:
    ldi     ZL,low(2*msg)     ; load Z pointer with
    ldi     ZH,high(2*msg)    ; myStr address
    rcall   puts                ; transmit string
    delay   4
    rjmp    loop 

msg:    
.db    "Hello world!",13,10,0,0
