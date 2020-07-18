import serial  # pip install pySerial
import struct
import network_protocol_pb2
import time

p = serial.Serial("/dev/ttyACM0", 115200)

while True:
    last_packet = time.time()
    s = b""
    while True:
        if p.in_waiting > 0:
            b = p.read()
            s += b
            last_packet = time.time()
        if (
            time.time() - last_packet
        ) > 0.001:  # Arbitrary longish time picked up by experiment
            break

    stat = network_protocol_pb2.ControllerStatus()

    try:
        stat.ParseFromString(s)
        print("------")
        print(stat)
    except:
        pass

    while p.in_waiting == 0:
        pass
