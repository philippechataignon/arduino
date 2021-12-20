%.hex: %.s
	avra -l $<.lst -fI $<
	@rm -f *.eep.hex *.obj *.cof

all: $(patsubst %.s,%.hex,$(wildcard *.s))

upload_%: %.hex
	avrdude -D -c arduino -p m328p -P /dev/ttyACM0 -b 115200 -U flash:w:$<

com:
	picocom /dev/ttyACM0

clean:
	rm -f *.hex *.obj *.cof *.lst
