%.hex: %.asm
	avra -l $<.lst -fI $<
	@rm -f *.eep.hex *.obj *.cof

all: $(patsubst %.asm,%.hex,$(wildcard *.asm))

upload: ${program}.hex
	avrdude -c arduino -p m328p -P /dev/ttyACM0 -b 115200 -U flash:w:$<

monitor:
	picocom --send-cmd "ascii_xfr -s -v -l10" --nolock /dev/ttyACM0

clean:
	rm -f *.hex *.obj *.cof *.lst

.PHONY: all upload monitor
