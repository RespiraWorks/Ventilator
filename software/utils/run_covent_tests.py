#!/usr/bin/env python3

"""
Wrapper around controller_debug.py that runs all of the covent tests in a row
and packages them into a standard, sharable format.
"""

from datetime import date
from pathlib import Path
import argparse
import atexit
import math
import os
import subprocess
import time
import sys
import shlex

PRESETS = [f"covent_pc_{i}" for i in range(1, 9)]

# Covent tests come in two flavors, ones which are meant to run at 30%
# fio2, and ones which are meant to run at 90%.
HIGH_OX_TESTS = {f"covent_pc_{i}" for i in [2, 3, 6, 7]}

DBG_SCRIPT = Path(__file__).absolute().parent.joinpath("controller_debug.py")


def run_test(test, cmd_args):
    def run(args):
        subprocess.check_call(
            [sys.executable, DBG_SCRIPT, "--port", cmd_args.port, "-c", args]
        )

    outfile = cmd_args.dest.joinpath(f"{test}.dat")

    # Tell the controller whether to generate pressure using the fan or the
    # high-pressure oxygen source.
    #
    # TODO: Although these tests are meant to run at 30%/90% fio2, we currently
    # can't mix air and oxygen, so we run them at 21% and 100%.
    if cmd_args.oxygen == "on" or (cmd_args.oxygen == "auto" and test in HIGH_OX_TESTS):
        run("set gui_fio2 100")
    else:
        run("set gui_fio2 21")

    # Ensure the ventilator fan is on, and disconnect the fan from the system
    # so it's not inflating the test lung.  Edwin's request is that the fan
    # doesn't have to spin up during these tests.
    run("set forced_exhale_valve_pos 1")
    run("set forced_blower_valve_pos 0")
    run("set forced_blower_power 1")
    run("set gui_mode 0")

    # Switch to this preset.
    run(f"preset {test}")

    # Give the user a chance to adjust the test lung.
    if not cmd_args.nointeractive:
        input(
            "Adjust test lung per above, then press enter.  "
            "(Skip this with --nointeractive.)"
        )
    # Unforce parameters we set above so they can be controlled by the
    # controller.  Note that we unforce the blower power *after* setting the
    # gui_mode to 1 because if we unforced it while we were still in mode 0
    # (i.e. "ventilator off"), the fan would momentarily spin down.
    run("set forced_exhale_valve_pos -1")
    run("set forced_blower_valve_pos -1")
    run("set gui_mode 1")
    run("set forced_blower_power -1")
    time.sleep(cmd_args.ignore_secs)

    run(f"trace start pc_setpoint pressure volume net_flow")
    time.sleep(cmd_args.capture_secs)

    # Graphing flow at any scale is noisy and overwhelms the rest of the chart.
    # trace graph can't currently exclude fields, so as a hack, graph flow
    # divided by a large number, essentially zeroing it out.
    run(
        f"trace graph --dest={shlex.quote(str(outfile))} --title={test} "
        "--nointeractive --scale=volume/10 --scale=net_flow/1e9"
    )


def concat_images(cmd_args):
    # It's more compact to show two images per row, but then visually you are
    # tempted to assume the Y axes are the same in each row, which is not true.
    IMAGES_PER_ROW = 1

    try:
        from PIL import Image
    except ImportError:
        print("pip install pillow to get a single image of all the test results.")
        return

    images = [Image.open(p) for p in sorted(cmd_args.dest.glob("*.png"))]
    max_width = max(img.width for img in images)
    max_height = max(img.height for img in images)

    combined_width = min(IMAGES_PER_ROW, len(images)) * max_width
    combined_height = math.ceil(len(images) / IMAGES_PER_ROW) * max_height

    combined = Image.new(
        "RGB", (combined_width, combined_height), color=(255, 255, 255)
    )
    for i, img in enumerate(images):
        xidx = i % IMAGES_PER_ROW
        yidx = i // IMAGES_PER_ROW
        combined.paste(img, (xidx * max_width, yidx * max_height))
    combined.save(cmd_args.dest.joinpath("combined.png"))


def choose_dest_dir():
    prefix = f"covent-tests-{date.today().isoformat()}-run-"
    dirs = sorted(Path(".").glob(prefix + "[0-9][0-9][0-9]"))
    if not dirs:
        n = 0
    else:
        n = int(str(dirs[-1])[-3:]) + 1
    return f"{prefix}{n:03d}"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--port",
        help="port device is connected to (default: try to auto-detect the port)",
    )
    parser.add_argument(
        "--tests",
        help="comma-separated list of test numbers to run "
        "(default: run all covent tests)",
    )
    parser.add_argument(
        "--oxygen",
        choices=["off", "auto", "on"],
        default="off",
        help="Deliver air from the blower (off), from the pressurized oxygen "
        "source (on), or according to the particular test (auto).",
    )
    parser.add_argument(
        "--ignore-secs",
        help="Ignore this many seconds of data before starting to capture.",
        default=10,
        type=int,
    )
    parser.add_argument(
        "--capture-secs",
        help="Capture this many seconds of data from each test.",
        default=20,
        type=int,
    )
    parser.add_argument("--dest", help="Output directory (must not exist)")
    parser.add_argument(
        "--nointeractive",
        help="Run without prompting the user to e.g. change lung compliance",
    )
    args = parser.parse_args()

    if not args.port:
        args.port = (
            subprocess.check_output(
                [sys.executable, DBG_SCRIPT, "--detect-port-and-quit"]
            )
            .decode("utf-8")
            .strip()
        )

    if not args.dest:
        args.dest = choose_dest_dir()
        print(f"Writing output to {args.dest}")

    args.dest = Path(args.dest)

    if args.tests:
        tests = [f"covent_pc_{i}" for i in args.tests.split(",")]
    else:
        tests = PRESETS

    # Stop ventilating if this script exits abnormally, e.g. due to ctrl+c.
    def stop_ventilation():
        subprocess.check_call(
            [sys.executable, DBG_SCRIPT, "--port", args.port, "-c", "set gui_mode 0"]
        )

    atexit.register(stop_ventilation)

    args.dest.mkdir(exist_ok=False)

    for i, t in enumerate(tests):
        run_test(t, args)

    concat_images(args)


if __name__ == "__main__":
    main()
