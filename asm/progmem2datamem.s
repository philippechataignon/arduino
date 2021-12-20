.nolist
.include "m328pdef.inc"
.list

.equ	numB 	= 20		; number of bytes in array

.def	tmp	= r16		; define temp register
.def	loopCt	= r17		; define loop count register

.dseg
.org	SRAM_START
sArr:	.BYTE	numB			; allocate bytes in SRAM for array

.cseg
.org	0x00
	ldi	XL,LOW(sArr)		; initialize X pointer
	ldi	XH,HIGH(sArr)		; to SRAM array address

	ldi	ZL,LOW(2*pArr)		; initialize Z pointer
	ldi	ZH,HIGH(2*pArr)		; to pmem array address

	ldi	loopCt,numB		; initialize loop count to number of bytes

arrLp:	lpm	tmp,Z+			; load value from pmem array
	st	X+,tmp			; store value to SRAM array
	dec	loopCt			; decrement loop count
	brne	arrLp			; repeat loop for all bytes in array

loop:	rjmp	loop			; infinite loop

pArr:	.db	0,1,2,3,4,5,6,7,8,9,\
		10,11,12,13,14,15,16,\
		17,18,19		; program memory array
