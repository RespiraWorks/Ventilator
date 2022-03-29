# Test data abstraction for ventilator debug interface

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

import datetime
from test_scenario import TestScenario
from typing import Dict, List
from datetime import datetime
import matplotlib.pyplot as plt
import dateutil.parser
import json
import platform
import git
from lib.colors import red
from pathlib import Path


class Trace:
    variable_name: str
    variable_units: str
    data: List

    def __init__(self, name, units):
        self.variable_name = name
        self.variable_units = units
        self.data = []

    def as_dict(self):
        return {
            "variable_name": self.variable_name,
            "variable_units": self.variable_units,
            "data": self.data,
        }

    @staticmethod
    def from_dict(data):
        name = data["variable_name"]
        units = data["variable_units"]
        ret = Trace(name, units)
        ret.data = data["data"]
        return ret

    def plot(self, subplt):
        return


class TestData:
    """Abstraction for test data, including metadata and test scenario setup"""

    start_time_utc: datetime = None
    platform_uname = None
    tester_name: str = None
    tester_email: str = None
    git_version: str = None
    git_branch: str = None
    git_dirty: bool = True
    scenario: TestScenario
    ventilator_settings: Dict
    ventilator_readings: Dict
    traces: List

    def __init__(self, test_scenario):
        self.start_time_utc = datetime.utcnow()
        self.platform_uname = platform.uname()
        repo = git.Repo(search_parent_directories=True)
        self.git_version = repo.git.describe("--tags")
        self.git_branch = repo.active_branch.name
        self.git_dirty = repo.is_dirty(untracked_files=True)
        try:
            self.tester_name = repo.config_reader().get_value("user", "name")
        except:
            self.tester_name = "anonymous"
        try:
            self.tester_email = repo.config_reader().get_value("user", "email")
        except:
            self.tester_email = "no@mail.provided"
        self.scenario = test_scenario
        self.traces = []

    def unique_name(self):
        return (
            self.start_time_utc.strftime("%Y-%m-%d_%H-%M-%S")
            + "_"
            + self.tester_name.split()[0].lower()
            + "_"
            + self.scenario.name
        )

    def __str__(self):
        ret = "Start time (UTC): {}\n".format(
            self.start_time_utc.strftime("UTC %Y-%m-%d %H:%M:%S")
        )
        ret += f"Machine info:     {self.platform_uname}\n"
        ret += f"Tester:           {self.tester_name} ({self.tester_email})\n"
        ret += f"Version:          {self.git_version} ({self.git_branch})"
        if self.git_dirty:
            ret += red(" DIRTY")
        ret += "\n"
        ret += "TEST SCENARIO:\n" + self.scenario.long_description()
        if self.ventilator_settings:
            ret += "\nVentilator settings:\n"
            for name, val in sorted(self.ventilator_settings.items()):
                ret += f"  {name:30} = {val}\n"
        if self.ventilator_readings:
            ret += "\nVentilator readings:\n"
            for name, val in sorted(self.ventilator_readings.items()):
                ret += f"  {name:30} = {val}\n"
        if len(self.traces):
            ret += f"Test data contains [{len(self.traces)}] traces"
            for t in self.traces:
                ret += f"\n  {t.variable_name} ({t.variable_units}) = [{len(t.data)}]"
        return ret

    def print_traces(self, separator=" ", line_separator="\n"):
        line = []
        for t in self.traces:
            ll = f"{t.variable_name} ({t.variable_units})"
            line.append(f"{ll:>25}")
        ret = separator.join(line) + line_separator

        for i in range(len(self.traces[0].data)):
            line = []
            for j in range(len(self.traces)):
                line.append("{:>25.3f}".format(self.traces[j].data[i]))
            ret += separator.join(line) + line_separator

        return ret

    def as_dict(self):
        return {
            "start_time_utc": self.start_time_utc.isoformat(),
            "platform_uname": self.platform_uname,
            "tester_name": self.tester_name,
            "tester_email": self.tester_email,
            "git_version": self.git_version,
            "git_branch": self.git_branch,
            "git_dirty": self.git_dirty,
            "scenario": self.scenario.as_dict(),
            "ventilator_settings": self.ventilator_settings,
            "ventilator_readings": self.ventilator_readings,
            "traces": [t.as_dict() for t in self.traces],
        }

    def save_json(self, parent_path: str, print_self=False):
        if print_self:
            print("\nResults:")
            print(self)

        file_name = Path(parent_path) / (self.unique_name() + ".json")
        if print_self:
            print(f"Saving as {file_name}")
        with open(file_name, "w") as json_file:
            json.dump(self.as_dict(), json_file, indent=4)

    def save_csv(self, parent_path: str):
        file_name = Path(parent_path) / (self.unique_name() + ".csv")
        with open(file_name, "w") as csv_file:
            csv_file.write(self.print_traces(separator=", "))

    def plot(self, parent_path: str, save: bool, show: bool):
        title = self.unique_name()

        timestamps_sec = self.traces[0].data
        dat = self.traces[1:]

        unique_units = set([x.variable_units for x in dat])

        figure, axes = plt.subplots(len(unique_units), sharex=True, squeeze=False)
        c = 0
        for i, units in enumerate(unique_units):
            relevant_data = [x for x in dat if x.variable_units == units]
            for d in relevant_data:
                axes[i][0].plot(
                    timestamps_sec, d.data, color="C{}".format(c), label=d.variable_name
                )
                c += 1

            axes[i][0].legend(loc="upper right")
            axes[i][0].set_ylabel(units)
            axes[i][0].grid()
            axes[i][0].axhline(linewidth=1, color="black")
            # Draw a black gridline at y=0 to highlight the x-axis.

        plt.xlabel("Seconds")
        figure.suptitle(title)
        figure.set_figheight(12)
        figure.set_figwidth(16)

        if save:
            file_name = Path(parent_path) / (title + ".png")
            plt.savefig(file_name, format="png", metadata={"Title": title}, dpi=100)

        if show:
            plt.show()
        else:
            plt.close()

    @staticmethod
    def from_dict(data):
        td = TestData(TestScenario.from_dict(data["scenario"]))
        td.start_time_utc = dateutil.parser.isoparse(data["start_time_utc"])
        td.platform_uname = data["platform_uname"]
        td.tester_name = data["tester_name"]
        td.tester_email = data["tester_email"]
        td.git_version = data["git_version"]
        td.git_branch = data["git_branch"]
        td.git_dirty = data["git_dirty"]
        td.ventilator_settings = data["ventilator_settings"]
        td.ventilator_readings = data["ventilator_readings"]
        for t in data["traces"]:
            td.traces.append(Trace.from_dict(t))
        return td

    @staticmethod
    def from_json(file_name):
        with open(file_name, "r") as json_file:
            return TestData.from_dict(json.load(json_file))
