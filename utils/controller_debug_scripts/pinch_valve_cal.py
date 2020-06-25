# Script used to capture data for calibrating pinch valves
#
# This script uses two debug variables to control blower power and
# the position of the valve.  You'll need to add those to use it.
# The easiest place is probably in controller.cpp in the vent off state.
#
# Also, the calibration data in the pinch valve code should be
# commented out unless you are trying to run this test on the
# already calibrated pinch valve to see if you get linear results.


def main():
    print("Shutting off fan and homing pinch valve")
    SetVar("blower_power", 0.75)
    SetVar("blower_valve", 0)

    input("Hit enter when ready to continue")
    print()

    print("Reading minimal flow value from sensor...")
    SetVar("trace_var1", "flow_inhale")
    SetVar("trace_var2", "none")
    SetVar("trace_var3", "none")
    SetVar("trace_var4", "none")
    SetVar("trace_ctrl", 0)
    SetVar("trace_period", 1)
    zero = GetTraceMean()
    print(zero)

    print("Taking a reading fully open")
    SetVar("blower_valve", 1)
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
            SetVar("blower_valve", valve)
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
    SetVar("blower_power", 0)

    for i in range(len(results)):
        print("%.4f" % results[i])


def GetTraceMean(samps=300):
    SetVar("trace_ctrl", 0)
    SetVar("trace_ctrl", 1)
    while int(GetVar("trace_samples")) < samps:
        pass

    dat = TraceDownload()
    return mean(dat[1])


def mean(dat):
    return sum(dat) / len(dat)


main()
