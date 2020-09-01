%.o: %.c
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o $@ $<

%avr: %o
	avr-gcc -mmcu=atmega328p $< -o $@

%.hex: %.avr
	avr-objcopy -O ihex $< $@

TARGETS = led.hex timeswitch.hex

all: $(TARGETS)

upload: ${program}.hex
	avrdude -D -c arduino -p m328p -P /dev/ttyACM0 -b 115200 -U flash:w:$<

com:
	picocom /dev/ttyACM0

clean:
	rm -f $(TARGETS) *.o *.avr
