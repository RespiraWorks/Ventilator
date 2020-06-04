# pip install pyserial
# can do "python3 -m serial.tools.list_ports" in CLI to get list of available ports
# This is based on some code that someone posted in the Amazon reviews for the project :D

import serial
import time

# Controller=serial.Serial("/dev/ttyS0", timeout=1, write_timeout=1)
Controller = serial.Serial("/dev/ttyUSB0", timeout=1, write_timeout=1)
time.sleep(5.0)
print(Controller.is_open)
print(Controller.get_settings())
# int("01010101",2) converts binary string 01010101 into a integer value where 0 and 1 represents the whether the valve #is on or off
# bytes[()] converts the value above into bytecode (hex format) that can be sent to the serial port.
Controller.write(bytes([int("00000000", 2)]))
time.sleep(2.0)
Controller.write(bytes([int("11111111", 2)]))
