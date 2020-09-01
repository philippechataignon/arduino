%.o: %.c
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o $@ $<

%elf: %o
	avr-gcc -mmcu=atmega328p -o $@ $<

%.hex: %.elf
	avr-objcopy -O ihex $< $@

%.lst: %.elf
	avr-objdump -d $< > $@

%.s: %.c
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -S $<

TARGETS = $(patsubst %.c,%.hex,$(wildcard *.c))

all: $(TARGETS)

upload: ${program}.hex
	avrdude -D -c arduino -p m328p -P /dev/ttyACM0 -b 115200 -U flash:w:$<

com:
	picocom /dev/ttyACM0

clean:
	rm -f $(TARGETS) *.o *.elf
