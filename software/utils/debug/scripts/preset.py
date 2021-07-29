import dataclasses
from typing import Dict, Union

# Copied from network_protocol.proto.
# TODO: Import the proto instead!
VentMode_OFF = 0
VentMode_PRESSURE_CONTROL = 1
VentMode_PRESSURE_ASSIST = 2

@dataclasses.dataclass
class Preset:
    """A named list of DebugVars + values."""

    name: str
    desc: str
    vars: Dict[str, Union[int, float]]

    def short_description(self):
        return f"{self.name} - {self.vars}"

    def long_description(self):
        return (
                self.desc
                + "\n"
                + "\n".join(f"  - {var} = {val}" for var, val in self.vars.items())
        )


def pressure_control_preset(
        test_num,
        intended_tv,
        lung_compliance,
        lung_resistance,
        rr,
        inspiratory_time,
        delta_inspiratory_pressure,
        fio2,
        bap,
):
    """Constructs a Preset object for a particular covent pressure-control test.

    Copied from CoVent-19 Ventilator Testing Procedure, table 201.105:
    https://drive.google.com/file/d/1FJOs6pdwHqV-Ygm5gMwIRBAmqH6Xxby8
    """
    desc = f"""\
CoVent-19 pressure-control test #{test_num}

If you have a calibrated test lung, configure it as follows:

 - Compliance: {lung_compliance} ml/hPa +/- 10%
 - Linear resistance: {lung_resistance} hPa/l/s +/- 10%

Intended TV: {intended_tv} ml
"""

    sec_per_breath = 60 / rr
    ie_ratio = inspiratory_time / (sec_per_breath - inspiratory_time)
    # TODO: For now fio2 is intentionally ignored.  Most of us don't have
    # access to pressurized gas, so all tests have to run with the blower.
    # run_covent_tests.py is able to respect the fio2 settings if you have
    # pressurized gas available.
    vars = {
        "gui_bpm": rr,
        "gui_ie_ratio": round(ie_ratio, 2),
        "gui_pip": bap + delta_inspiratory_pressure,
        "gui_peep": bap,
        # It's important to set gui_mode last.  Otherwise we'll start breathing
        # immediately, with whatever the old parameters happen to be.  (Python3
        # dicts have consistent ordering.)
        "gui_mode": VentMode_PRESSURE_CONTROL,
    }
    return Preset(f"covent_pc_{test_num}", desc, vars)


# DebugVar presets recognized by the `preset` command.
PRESETS = [
    pressure_control_preset(1, 500, 50, 5, 20, 1, 10, 30, 5),
    pressure_control_preset(2, 500, 50, 20, 12, 1, 15, 90, 10),
    pressure_control_preset(3, 500, 20, 5, 20, 1, 25, 90, 5),
    pressure_control_preset(4, 500, 20, 20, 20, 1, 25, 30, 10),
    pressure_control_preset(5, 300, 20, 20, 20, 1, 15, 30, 5),
    pressure_control_preset(6, 300, 20, 50, 12, 1, 25, 90, 10),
    pressure_control_preset(7, 300, 10, 50, 20, 1, 30, 90, 5),
    pressure_control_preset(8, 200, 10, 10, 20, 1, 25, 30, 10),
]


def print_help():
    print(
        """\
Apply a preset list of settings, all at once.

Usage:

preset PRESET
Apply the given preset.

Available PRESETs:
"""
    )
    for p in PRESETS:
        print(f"  - {p.short_description()}")


def main():

    presets_manifest = "Available PRESETs:\n"

    for p in PRESETS:
        presets_manifest += "  " + p.short_description() + "\n"

    parser = CmdArgumentParser("preset.py",
                               formatter_class=argparse.RawDescriptionHelpFormatter,
                               epilog=presets_manifest)
    parser.add_argument("preset", metavar="PRESET", type=str)

    cl = cmdline.split()

    if len(cl) < 2:
        print(Colors.RED + "Not enough args\n" + Colors.ENDC)
        parser.print_help()
        return

    args = parser.parse_args(cl[1:])

    candidates = [p for p in PRESETS if p.name.lower() == args.preset.lower()]
    if not candidates:
        print(f"No preset named {args.preset}")
        return
    if len(candidates) > 1:
        print(
            f"Two or more presets named {args.preset} (case-insensitive)!  "
            "Fix the PRESETS variable in the code."
        )
        return

    preset = candidates[0]
    print(f"Applying preset {preset.name}:\n")
    print(textwrap.indent(preset.long_description(), "    "))

    for var, val in preset.vars.items():
        interface.set_variable(var, val)

    print(f"\nPreset {preset.name} successfully applied!")

def complete_preset(self, text, line, begidx, endidx):
    return [k for k in PRESETS.keys() if k.lower().startswith(text.lower())]

main()
