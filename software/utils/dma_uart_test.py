# /bin/python -u
import serial  # pip install pySerial
import struct
import time

debug = serial.Serial("/dev/ttyACM0", 115200, timeout=0.1)
dma_port = serial.Serial("/dev/ttyUSB0", 115200, timeout=0.1)

greet = "ping ping ping ping ping ping ping ping ping ping ping ping\n"


def reset():
    debug.flush()
    debug.reset_input_buffer()
    dma_port.flush()
    dma_port.reset_input_buffer()
    debug.write("x")  # write any char to debug port to reset the device
    time.sleep(0.1)
    debug.read(1)


reset()


def testStarup():
    inp = debug.read(3)
    if inp != "*!!":
        print("Unexpected startup message:" + inp)
        exit(-1)
    # print (inp)


testStarup()


def testTxEnd():
    # wait for tx end
    inp = debug.read_until("$")
    if inp != "$":
        print("Unexpected TX end:" + inp)
        exit(-1)
    # print (inp)
    inp = dma_port.read_until("\n")
    if inp != greet:
        print("Unexpected dma port output: " + inp)
        exit(-1)
    # print(inp)


testTxEnd()


def testRx():
    # Firmware wants 10 chars from us
    t = "asdfasdfas"
    dma_port.write(t)

    inp = debug.read(1)
    if "&" != inp:
        print("End of rx not detected: " + inp)
        exit(-1)
    inp = debug.read(10)
    if t != inp:
        print("Unexpected echo: " + inp)
    # print inp


testRx()


def testCharMatch():
    reset()
    testStarup()
    testTxEnd()
    dma_port.write(".")  # '.' is matched char
    c = debug.read(1)
    if "@" != c:
        print("No char match: " + c)
    # print(c)
    dma_port.write("111111111")
    inp = debug.read(1)
    if "&" != inp:
        print("End of rx not detected: " + inp)
        exit(-1)
    inp = debug.read(10)
    if ".111111111" != inp:
        print("Unexpected echo: " + inp)
    # print inp


testCharMatch()


def testTimeout():
    reset()
    testStarup()
    testTxEnd()
    dma_port.write("1")
    time.sleep(2)
    c = debug.read(1)
    if "T" != c:
        print("Timeout not detected: " + c)
        exit(-1)
    # print(c)


testTimeout()


debug.close()
dma_port.close()

exit(0)
