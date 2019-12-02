#!/usr/bin/env python
import serial
import random
import time
import warnings

ser = serial.Serial('/dev/ttyUSB0')
ser.baudrate = 230400
ser.timeout = 10.0
ser.rtscts=1

if ser.name:
	print("Serial Connection Success")

def send_signal(signal):
    """
    2-dimensional signal
    [x,y]
    """
    x=signal[0]
    y=signal[1]
    msg=str(x)+'\n'+str(y)+'\n'
    ser.write(msg.encode())
    print(ser.readline())

def fake_stream(fs=1):
    
    while True:
        now=time.time()
        x=random.randint(-5,5)
        y=random.randint(-5,5)
        send_signal([x,y])
        
        elapsed=time.time()-now
        try:
            time.sleep(period-elapsed)
        except:
            warnings.warn('System cannot handle such high frame rate, lower the desired frequency or simplify your callback fucntion')
            continue



