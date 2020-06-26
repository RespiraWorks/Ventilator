from datetime import datetime
import sys
from labjack import ljm
import signal


def exit_gracefully(signum, frame):
    # restore the original signal handler as otherwise evil things will happen
    # in raw_input when CTRL+C is pressed, and our signal handler is not re-entrant
    signal.signal(signal.SIGINT, original_sigint)

    if input("\nReally quit? (y/n)> ").lower().startswith("y"):
        ljm.cleanInterval(interval_handle)
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


# LabJack T7 and other devices configuration

# AIN0 and AIN1:
#   Negative channel = single ended (199)
#   Range: +/-10.0 V (10.0)
#   Resolution index = Default (0)
#   Settling, in microseconds = Auto (0)
names = ["AIN0_NEGATIVE_CH", "AIN0_RANGE", "AIN0_RESOLUTION_INDEX", "AIN0_SETTLING_US"]
aValues = [199, 10, 1, 0]


numFrames = len(names)
ljm.eWriteNames(handle, numFrames, names, aValues)

print("\nSet configuration:")
for i in range(numFrames):
    print("    %s : %f" % (names[i], aValues[i]))

# Read AIN0 and AIN1 from the LabJack with eReadNames in a loop.

read_names = ["AIN0"]
num_read_frames = len(read_names)
print("\nStarting %s read loops.%s\n" % (str(loopAmount), loopMessage))
interval_handle = 1
ljm.startInterval(interval_handle, 100000)  # Delay between readings (in microseconds)

dac_out = 0

while True:
    try:
        results = ljm.eReadNames(handle, num_read_frames, read_names)
        time = datetime.now()
        labjack_value_0 = results[0]
        print("Time: {}  Value: {:.3f} V".format(time, labjack_value_0))
        ljm.waitForNextInterval(interval_handle)

        ljm.eWriteNames(handle, 1, ["DAC0"], [dac_out])

        if dac_out < 5:
            dac_out += 0.05
        else:
            dac_out = 0

    except Exception:
        print(sys.exc_info()[1])
        break

# Close handles
ljm.cleanInterval(interval_handle)
ljm.close(handle)
