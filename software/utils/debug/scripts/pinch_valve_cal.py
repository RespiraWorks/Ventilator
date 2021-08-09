# Script used to capture data for calibrating pinch valves
#
# This script uses two debug variables to control blower power and
# the position of the valve.  You'll need to add those to use it.
# The easiest place is probably in controller.cpp in the vent off state.
#
# Also, the calibration data in the pinch valve code should be
# commented out unless you are trying to run this test on the
# already calibrated pinch valve to see if you get linear results.

import time
import sys

sys.path.append("..")

from controller_debug import ControllerDebugInterface


def pinch_valve_cal(interface: ControllerDebugInterface, cmdline: str = ""):
    print("Shutting off fan and homing pinch valve")
    interface.variable_set("forced_blower_power", 0.75)
    interface.variable_set("forced_blower_valve_pos", 0)

    input("Hit enter when ready to continue")
    print()

    print("Reading minimal flow value from sensor...")
    zero = get_trace_mean(interface)
    print(zero)

    print("Taking a reading fully open")
    interface.variable_set("forced_blower_valve_pos", 1)
    time.sleep(0.3)
    full = get_trace_mean(interface)
    print(full)

    results = [1]

    valve = 1.0
    vinc = 0.05
    prev = full
    for i in range(9):
        pct = (9 - i) * 0.1

        target = (full - zero) * pct + zero

        print(
            "Looking for a valve setting for %d%% flow - target %.1f"
            % (pct * 100, target)
        )

        while valve >= vinc:
            valve -= vinc
            interface.variable_set("forced_blower_valve_pos", valve)
            time.sleep(0.2)
            m = get_trace_mean(interface)

            print("Valve at %.2f, flow %.1f" % (valve, m))
            if m <= target:
                adj = valve + (target - m) / (prev - m) * vinc

                print("Found at %.3f" % adj)
                results.insert(0, adj)
                vinc = 0.01
                break
            prev = m
    results.insert(0, 0)
    interface.variable_set("forced_blower_power", 0)

    for i in range(len(results)):
        print("%.4f" % results[i])


def get_trace_mean(interface: ControllerDebugInterface, samps=300):
    interface.trace_flush()
    interface.trace_select(["flow_inhale"])
    interface.trace_start()

    dat = interface.trace_download()
    return mean(dat[1][0:samps])


def mean(dat):
    return sum(dat) / len(dat)
