# Test scenario abstraction for ventilator debug interface

__copyright__ = "Copyright 2021 RespiraWorks"

__license__ = """

    Copyright 2021 RespiraWorks

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

"""

from typing import Dict
import pandas  # pip install pandas
import json
import copy
import argparse
from pathlib import Path
import colors


def trim_all_columns(df):
    """
    Trim whitespace from ends of each value across all series in dataframe
    """
    trim_strings = lambda x: x.strip() if isinstance(x, str) else x
    return df.applymap(trim_strings)


# todo can we also have units and/or uncertainties defined for some variables
class TestScenario:
    """Configuration for semi-automated tests"""

    name: str = None
    description: str = None
    manual_settings: Dict
    ventilator_settings: Dict
    test_criteria: Dict
    capture_duration_secs: int
    capture_ignore_secs: int
    trace_period: int
    trace_variable_names: [str]

    def __init__(self):
        self.manual_settings = {}
        self.ventilator_settings = {}
        self.test_criteria = {}
        self.capture_duration_secs = 25
        self.capture_ignore_secs = 2
        self.trace_period = 1
        self.trace_variable_names = ["pc_setpoint", "pressure", "volume", "net_flow"]

    def short_description(self):
        return f'{self.name:15} "{self.description}"'

    def long_description(self, highlight_manual=False):
        ret = colors.blue(f"[[{self.name}]]\n")
        ret += f'  "{self.description}"'
        if len(self.manual_settings):
            if highlight_manual:
                ret += colors.Color.ORANGE
            ret += "\n  Manual settings:\n"
            ret += "\n".join(
                f"    {var:25} = {val}" for var, val in self.manual_settings.items()
            )
            if highlight_manual:
                ret += colors.Color.ENDC
        if len(self.ventilator_settings):
            ret += "\n  Ventilator settings:\n"
            ret += "\n".join(
                f"    {var:25} = {val}" for var, val in self.ventilator_settings.items()
            )
        ret += f"\n  Capture duration (secs)     = {self.capture_duration_secs:3}"
        ret += f"\n  Capture ignore (secs)       = {self.capture_ignore_secs:3}"
        ret += f"\n  Trace period (loop cycles)  = {self.trace_period:3}"
        ret += f"\n  Trace variables:"
        for trace_name in self.trace_variable_names:
            ret += f"\n     {trace_name}"
        if len(self.test_criteria):
            ret += "\n  Test criteria:\n"
            ret += "\n".join(
                f"    {var:25} = {val}" for var, val in self.test_criteria.items()
            )
        return ret

    def as_dict(self):
        return {
            "name": self.name,
            "description": self.description,
            "manual_settings": self.manual_settings,
            "ventilator_settings": self.ventilator_settings,
            "capture_duration_secs": self.capture_duration_secs,
            "capture_ignore_secs": self.capture_ignore_secs,
            "trace_period": self.trace_period,
            "trace_variable_names": self.trace_variable_names,
            "test_criteria": self.test_criteria,
        }

    @staticmethod
    def from_dict(data):
        ts = TestScenario()
        ts.name = data.get("name", None)
        ts.description = data.get("description", None)
        ts.manual_settings = data.get("manual_settings", None)
        ts.ventilator_settings = data.get("ventilator_settings", None)
        ts.test_criteria = data.get("test_criteria", None)
        ts.capture_duration_secs = data.get("capture_duration_secs", None)
        ts.capture_ignore_secs = data.get("capture_ignore_secs", None)
        ts.trace_period = data.get("trace_period", None)
        ts.trace_variable_names = data.get("trace_variable_names", None)
        return ts

    @staticmethod
    def from_csv(file_name, ventilator_settings):
        df = pandas.read_csv(file_name)
        df.columns = df.columns.str.replace(" ", "")
        df = trim_all_columns(df)

        ret = {}
        for index, row in df.iterrows():
            if "id" not in row:
                raise Exception(f"Row does not contain id:\n{row}")
            if "description" not in row:
                raise Exception(f"Row does not contain description:\n{row}")
            ts = TestScenario()
            row_copy = copy.deepcopy(row)
            ts.name = row_copy.pop("id")
            ts.description = row_copy.pop("description")
            for k in row_copy.keys():
                if k in ventilator_settings:
                    ts.ventilator_settings[k] = row_copy[k]
                elif k.startswith("test_"):
                    ts.test_criteria[k] = row_copy[k]
                else:
                    ts.manual_settings[k] = row_copy[k]
            ret[ts.name] = ts
        # todo more checks, such as no duplicate labeled columns
        return ret

    @staticmethod
    def from_json(file_name):
        ret = {}
        with open(file_name, "r") as json_file:
            j = json.load(json_file)
            for tsd in j:
                ts = TestScenario.from_dict(tsd)
                ret[ts.name] = ts
        return ret


def main():

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "file",
        type=str,
        help="File from which to load test scenario configurations (csv or json)",
    )
    parser.add_argument(
        "--verbose",
        "-v",
        default=False,
        action="store_true",
        help="Print out verbose scenarios",
    )
    parser.add_argument(
        "--out_file",
        "-o",
        type=str,
        help="output file for writing full json configuration",
    )

    args = parser.parse_args()

    if not args.file:
        print(f"No input file name provided {args.preset}")
        return

    in_file = Path(args.file)
    if not in_file.is_file():
        print(f"Input file does not exist {args.file}")
        return
    elif in_file.suffix == ".csv":
        scenarios = TestScenario.from_csv(
            in_file,
            {
                "gui_mode",
                "gui_pip",
                "gui_peep",
                "gui_bpm",
                "gui_ie_ratio",
                "gui_fio2",
                "gui_tv",
            },
        )
    elif in_file.suffix == ".json":
        scenarios = TestScenario.from_json(in_file)
    else:
        print(f"Unknown file format `{in_file.suffix}`")
        return

    if len(scenarios) == 0:
        return

    print("Imported the following test scenarios:")
    for key in scenarios:
        if args.verbose:
            print(scenarios[key].long_description())
        else:
            print(scenarios[key].short_description())

    if args.out_file:
        tsl = []
        for key in scenarios:
            tsl.append(scenarios[key].as_dict())
        with open(args.out_file, "w") as json_file:
            json.dump(tsl, json_file, indent=4)


if __name__ == "__main__":
    main()
