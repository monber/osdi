#! /usr/bin/python3

import os
from socket import timeout
import time
import sys
import serial
from time import sleep

BAUD_RATE = 115200

def send_img(tty,kernel):
    print("Please sent the kernel image size:")
    tty.write(("loadimg\n").encode())
    kernel_size = os.stat(kernel).st_size
    # print(kernel_size)
    print(tty.read_until(b"Rpi3 is ready. Please send kernel img size\n").decode(), end = "")
    tty.write((str(kernel_size) + "\n").encode())
    print(tty.read_until(b"Rpi3 starts to load img\n").decode(), end = "")

    with open(kernel, "rb") as image:
        while kernel_size > 0:
            kernel_size -= tty.write(image.read(1))
            tty.read_until(b".")
    
    print(tty.read_until(b"load finish\n").decode(), end="")
    return

if __name__ == "__main__":
    PORT = sys.argv[1]
    tty = serial.Serial(PORT, BAUD_RATE, timeout=5)
    send_img(tty,"../kernel/kernel8.img")

    