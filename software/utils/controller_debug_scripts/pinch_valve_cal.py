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

def main():
    print("Shutting off fan and homing pinch valve")
    interface.SetVar("forced_blower_power", 0.75)
    interface.SetVar("forced_blower_valve_pos", 0)

    input("Hit enter when ready to continue")
    print()

    print("Reading minimal flow value from sensor...")
    zero = GetTraceMean()
    print(zero)

    print("Taking a reading fully open")
    interface.SetVar("forced_blower_valve_pos", 1)
    time.sleep(0.3)
    full = GetTraceMean()
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
            interface.SetVar("forced_blower_valve_pos", valve)
            time.sleep(0.2)
            m = GetTraceMean()

            print("Valve at %.2f, flow %.1f" % (valve, m))
            if m <= target:
                adj = valve + (target - m) / (prev - m) * vinc

                print("Found at %.3f" % adj)
                results.insert(0, adj)
                vinc = 0.01
                break
            prev = m
    results.insert(0, 0)
    interface.SetVar("forced_blower_power", 0)

    for i in range(len(results)):
        print("%.4f" % results[i])


def GetTraceMean(samps=300):
    interpreter.do_trace("flush")
    interpreter.do_trace("start flow_inhale")

    dat = interface.SendCmd(controller_low.OP_TRACE, [controller_low.SUBCMD_TRACE_GET_NUM_SAMPLES])
    ct = controller_types.Build32(dat)[0]
    while ct < samps:
        dat = interface.SendCmd(controller_low.OP_TRACE, [controller_low.SUBCMD_TRACE_GET_NUM_SAMPLES])
        ct = controller_types.Build32(dat)[0]

    dat = interface.TraceDownload()
    return mean(dat[1])


def mean(dat):
    return sum(dat) / len(dat)


main()
