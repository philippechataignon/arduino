#! /usr/bin/env python3
import argparse
import serial
import sys
import time
import unicodedata

def test_xonxoff(ser):
    read = ser.read()
    # XOFF
    if read == b'\x13':
        # Wait XON
        while True:
            read = ser.read()
            if read == b'\x11':
                break

def main():
    parser = argparse.ArgumentParser(description="Print on Panasonic R191")
    parser.add_argument("-p", "--port", type=str, help="COM port", default="/dev/ttyACM0")
    parser.add_argument("-v", "--verbose", help="increase output verbosity", action="store_true")
    parser.add_argument("-l", "--latin1", help="input is encoded in latin1 (default UTF8)", action="store_true")
    args = parser.parse_args()
    with serial.Serial(args.port, 9600, timeout=1) as ser:
        for line in sys.stdin:
            if not args.latin1:
                line = unicodedata.normalize("NFC", line).encode("latin1", "ignore")
            test_xonxoff(ser)
            if args.verbose:
                print(line)
            ser.write(line)

if __name__ == "__main__":
    main()
