import serial
import sys
import time

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('need input port')
        sys.exit()
    port  = sys.argv[1]
    with serial.Serial(port, 115200) as ser:
        with open('testcase.txt', 'r') as f:
            for line in f:
                ser.write(str(line).encode())
                time.sleep(0.1)
            ser.write(('\n').encode())