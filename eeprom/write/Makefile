BOARD_TAG = uno
include /usr/share/arduino/Arduino.mk

up: ${program}.hex
	avrdude -D -c arduino -p m328p -P /dev/ttyACM0 -b 115200 -U flash:w:$<

ee: ${program}.eep.hex
	avrdude -e -c arduino -p m328p -P /dev/ttyACM0 -b 115200 -U eeprom:w:$<

com:
	picocom /dev/ttyACM0
