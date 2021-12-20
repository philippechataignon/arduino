.nolist
.include "m328pdef.inc"
.list

.def temp1 = r17
.def temp2 = r18
.equ LCDrcve0 = 0x200

ReadEE:
    sbic    EECR,EEPE   ;busy writing EEPROM?
    rjmp    ReadEE      ;yes, so wait
    out     EEARH,YH    ;set up address reg.
    out     EEARL,YL
    sbi     EECR,EERE   ;strobe the read bit
    in      temp1,EEDR  ;get the dataret

WriteEE:
    sbic    EECR,EEPE   ;busy writing EEPROM?
    rjmp    WriteEE     ;yes, so wait
    out     EEARH,YH    ;set up address reg.
    out     EEARL,YL
    out     EEDR,temp1  ;put data in data reg.
    cli                 ;dont interrupt the write
    sbi     EECR,EEMPE  ;master write enable
    sbi     EECR,EEPE   ;strobe the write bit
    sei                 ;interrupts OK now
    ret

Read8E:                 ;read 8 bytes from EE
    ldi     temp2,8     ;counter=8
r81:
    rcall   ReadEE      ;get byte from EE
    st      Z+,temp1    ;move byte to destination
    adiw    YL,1         ;go to next EE addr
    dec     temp2
    brne    r81
    ret

LoadEEMem:              ;specify the preset# in temp1
                        ;will return the EE memory into LCDrcve0
    clr     YH
    ldi     YL,32       ;start of presets
    ldi     ZH,high(LCDrcve0)
    ldi     ZL,low(LCDrcve0)
ge0:
    cpi     temp1,0
    breq    ge1
    adiw    YL,8
    dec     temp1
    brne    ge0
ge1:
    rcall   Read8E
    ret
