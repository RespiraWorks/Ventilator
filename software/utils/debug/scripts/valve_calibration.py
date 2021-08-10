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


def valve_calibration(interface: ControllerDebugInterface, cmdline: str = ""):

    cl = cmdline.split()
    if len(cl) < 2:
        print("Not enough parameters for valve calibration")

    if len(cl) < 2 or cl[0] == "help":
        print("Syntax:")
        print("  run valve_calibration <forced_valve_variable> <flow_variable>")
        print("    forced_valve_variable - one of these:")
        print("         forced_exhale_valve_pos")
        print("         forced_blower_valve_pos")
        print("         forced_psol_pos")
        print("    flow_variable - one of these:")
        print("         flow_inhale")
        print("         flow_exhale")
        return

    valve_variable = cl[0]  # "forced_blower_valve_pos"
    flow_variable = cl[1]  # "flow_inhale"

    if valve_variable not in interface.variable_metadata.keys():
        print(f"Error: `{valve_variable}` is not a valid variable")
        return

    if flow_variable not in interface.variable_metadata.keys():
        print(f"Error: `{flow_variable}` is not a valid variable")
        return

    sample_duration = 3
    sample_size = int(sample_duration * 1e6 / interface.trace_get_period_us())

    print("Valve calibration routine:")
    print(f"  valve_variable = {valve_variable}")
    print(f"  flow_variable  = {flow_variable}")
    print(f"  samples        = {sample_size}")

    print("Shutting off fan and homing pinch valve")
    interface.variables_force_open()
    interface.variable_set("forced_blower_power", 0.75)

    input("Hit enter when ready to continue")
    print()

    interface.variable_set(valve_variable, 0)
    time.sleep(0.2)
    zero_flow = get_flow_mean(interface, flow_variable, sample_duration)
    print(f"zero flow = {zero_flow}")

    interface.variable_set(valve_variable, 1)
    time.sleep(0.2)
    full_flow = get_flow_mean(interface, flow_variable, sample_duration)
    print(f"full flow = {full_flow}")

    if zero_flow > full_flow:
        print("Error: zero_flow > full_flow")
        print("     Cannot continue with calibration routine.")
        print("     Please check pneumatic configuration and try again.")
        return

    results = [1]

    valve_position = 1.0
    step_size = 0.05
    previous_flow = full_flow
    for i in range(9):
        percent = (9 - i) * 0.1

        target_flow = zero_flow + percent * (full_flow - zero_flow)

        print(
            f"Looking for a valve setting for {percent:.1%} flow - target `{flow_variable}`={target_flow:.1f}"
        )

        while valve_position >= step_size:
            valve_position -= step_size
            interface.variable_set(valve_variable, valve_position)
            time.sleep(0.2)
            mean_flow = get_flow_mean(interface, flow_variable, sample_duration)

            print(
                f"Valve at `valve_variable`={valve_position:.2f}, flow\u2248{mean_flow:.2f}"
            )
            if mean_flow <= target_flow:
                adj = (
                    valve_position
                    + (target_flow - mean_flow)
                    / (previous_flow - mean_flow)
                    * step_size
                )

                print(f"Found at {adj:.4f}")
                results.insert(0, adj)
                step_size = 0.01
                break
            previous_flow = mean_flow
    results.insert(0, 0)
    interface.variable_set("forced_blower_power", 0)

    formatted_list = [f"{elem:.2f}" for elem in results]
    print(f"Results: {formatted_list}")


def get_flow_mean(interface: ControllerDebugInterface, flow_variable, sample_duration):
    interface.trace_stop()
    interface.trace_select([flow_variable])
    interface.trace_start()

    time.sleep(sample_duration)

    dat = interface.trace_download()[1]
    return mean(dat)


def mean(dat):
    return sum(dat) / len(dat)
