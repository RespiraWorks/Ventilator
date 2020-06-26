# SOURCES OF TRUTH
#
# Using this datasheet for Sensor: https://media.ncd.io/sites/2/20170721134812/ams5915-5.pdf
#
# Reusing some Labjack sample Python code from here: https://labjack.com/support/software/examples/ljm/python
#
# Some of Riley's code modified by Martin
# It does not crash. But data does not seem meaningful.
# Help!

from datetime import datetime
import sys
from labjack import ljm
import signal


def exit_gracefully(signum, frame):
    # restore the original signal handler as otherwise evil things will happen
    # in raw_input when CTRL+C is pressed, and our signal handler is not re-entrant
    signal.signal(signal.SIGINT, original_sigint)

    if input("\nReally quit? (y/n)> ").lower().startswith("y"):
        # ljm.cleanInterval(interval_handle)
        ljm.close(handle)

        sys.exit(0)

    # restore the exit gracefully handler here
    signal.signal(signal.SIGINT, exit_gracefully)


original_sigint = signal.getsignal(signal.SIGINT)
signal.signal(signal.SIGINT, exit_gracefully)


# An argument was not passed. Loop an infinite amount of times.
loopAmount = "infinite"
loopMessage = " Press Ctrl+C to stop."

handle = ljm.openS("T7", "ANY", "ANY")  # T7 device, Any connection, Any identifier
info = ljm.getHandleInfo(handle)
print(
    "Opened a LabJack with Device type: %i, Connection type: %i,\n"
    "Serial number: %i, IP address: %s, Port: %i,\nMax bytes per MB: %i"
    % (info[0], info[1], info[2], ljm.numberToIP(info[3]), info[4], info[5])
)

deviceType = info[0]

# interval_handle = 1

# LabJack T7 and other devices configuration

# ============================================================
# Configure the I2C communication.
ljm.eWriteName(handle, "I2C_SDA_DIONUM", 17)  # SDA pin number = 1 (FIO1)
ljm.eWriteName(handle, "I2C_SCL_DIONUM", 14)  # SCA pin number = 0 (FIO0)

# Speed throttle is inversely proportional to clock frequency. 0 = max.
#   Default=0 corresponds to 65536 internally which results in ~450 kHz. 1 results in ~40 Hz, 65516 is ~100 kHz.
# ljm.eWriteName(handle, "I2C_SPEED_THROTTLE", 65516)  # 100kHz
ljm.eWriteName(handle, "I2C_SPEED_THROTTLE", 65536)  # 100kHz

# Options bits:
#   bit0: Reset the I2C bus.
#   bit1: Restart w/o stop
#   bit2: Disable clock stretching.
ljm.eWriteName(handle, "I2C_OPTIONS", 0)  # Options = 0

# 7 bit adres shited to an 8 bit adres r/w bit in the 8th position is taken care of by labjack
# 0x20 < 1 bit = 0x40 = 64, 0x21 < 1 = 0x 42  = 66, 0x22 = 0x44  = 68, 0x28 = 0x50 = 80
ljm.eWriteName(
    handle, "I2C_SLAVE_ADDRESS", 40
)  # Slave Address of the I2C chip = 40 (0x28)

# Initial read of 4 bytes. We need a single I2C
# transmission that the sensor docs apparently say should happen from master?
ljm.eWriteName(handle, "I2C_NUM_BYTES_TX", 1)  # Set the number of bytes to transmit
ljm.eWriteName(handle, "I2C_NUM_BYTES_RX", 4)  # Set the number of bytes to receive

# Set the TX bytes. We are sending 1 byte for the address.
numBytes = 1
aBytes = [1]  # Byte 0: Memory pointer = 0
ljm.eWriteNameByteArray(handle, "I2C_DATA_TX", numBytes, aBytes)

ljm.eWriteName(handle, "I2C_GO", 1)  # Do the I2C communications.

ack = ljm.eReadName(handle, "I2C_ACKS")
print("Ack %i\n" % ack)

# Read the RX bytes.
numBytes = 4
# aBytes[0] to aBytes[3] will contain the data
aBytes = [0] * 4
aBytes = ljm.eReadNameByteArray(handle, "I2C_DATA_RX", numBytes)

print("\nRead User Memory [0-3] = %s" % " ".join([("%.0f" % val) for val in aBytes]))

ack = ljm.eReadName(handle, "I2C_ACKS")
print("Ack %i\n" % ack)

# Close handles
# ljm.cleanInterval(interval_handle)
ljm.close(handle)
