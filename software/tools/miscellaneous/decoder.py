import serial
from protocols import network_protocol_pb2
import time
import argparse


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("serialport", metavar="SERIAL", type=str, help="Serial port")
    args = parser.parse_args()

    port = serial.Serial(args.serialport, 115200)

    while True:
        last_packet = time.time()
        s = b""
        while True:
            if port.in_waiting > 0:
                b = port.read()
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

        while port.in_waiting == 0:
            pass
