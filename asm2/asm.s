    .section    .text
    
    ;The global directive declares AsmSubroutine as global for linker.
    ;The AsmSubroutine label must follow the global directive.
    .global     AsmSubroutine
AsmSubroutine:
    add        R25,R23    
    adc        R24,R22
    ret
    .end
