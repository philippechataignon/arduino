%.hex: %.s
	avra -fI -l $(<:.s=.lst) -o "$@" "$<"
	@rm -f $(<:.s=.eep.hex) $(<:.s=.obj) $(<:.s=.cof) 

all: $(patsubst %.s,%.hex,$(wildcard *.s))

upload_%: %.hex
	avrdude -D -c arduino -p m328p -P /dev/ttyACM0 -b 115200 -U flash:w:$<

com:
	picocom /dev/ttyACM0

clean:
	rm -f *.hex *.obj *.cof *.lst
