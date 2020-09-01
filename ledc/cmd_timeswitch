 avr - gcc - g - Os - DF_CPU = 16000000UL - mmcu = atmega328p - c - o timeswitch.o timeswitch.c avr - gcc - mmcu = atmega328p timeswitch.o - o timeswitch avr - objcopy - O ihex - R.eeprom timeswitch timeswitch.hex avrdude - D - V - c arduino - p ATMEGA328P - P / dev / ttyACM0 - b 115200 - U flash: w:timeswitch.
    hex
