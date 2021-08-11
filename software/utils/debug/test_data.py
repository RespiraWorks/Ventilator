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


class TestData:
    """Abstraction for test data, including metadata and test scenario setup"""

    start_time_utc: datetime = None
    platform_uname = None
    tester_name: str = None
    tester_email: str = None
    git_sha: str = None
    git_dirty: bool = True
    scenario: TestScenario
    ventilator_settings: Dict
    traces: List

    def __init__(self, test_scenario):
        self.start_time_utc = datetime.utcnow()
        self.platform_uname = platform.uname()
        repo = git.Repo(search_parent_directories=True)
        self.git_sha = repo.head.object.hexsha
        self.git_dirty = repo.is_dirty()
        # todo: uncommitted changes? unpushed changes?
        self.tester_name = repo.config_reader().get_value("user", "name")
        self.tester_email = repo.config_reader().get_value("user", "email")
        self.scenario = test_scenario

    def unique_name(self):
        return (
            self.start_time_utc.strftime("%Y-%m-%d-%H-%M-%S")
            + "_"
            + self.tester_email.partition("@")[0]
            + "_"
            + self.scenario.name
        )

    def __str__(self):
        ret = "Start time (UTC): {}\n".format(
            self.start_time_utc.strftime("%Y-%m-%d %H:%M:%S")
        )
        ret += f"Machine info:     {self.platform_uname}\n"
        ret += f"Tester:           {self.tester_name} ({self.tester_email})\n"
        dirty_string = red(" (DIRTY)")
        ret += "Git sha:          {}{}\n".format(
            self.git_sha, dirty_string if self.git_dirty else ""
        )
        ret += "TEST SCENARIO:\n" + self.scenario.long_description()
        if self.ventilator_settings:
            ret += "\nVentilator settings:\n"
            for name, val in self.ventilator_settings.items():
                ret += f"  {name:25} = {val}\n"
        if self.traces:
            cols = len(self.traces)
            rows = len(self.traces[0])
            ret += f"Test data contains [{cols}][{rows}] data points"
        return ret

    def print_trace(self, separator=" ", line_separator="\n"):
        line = ["{:>15}".format("time(sec)")]
        for v in self.scenario.trace_variable_names:
            line.append(f"{v:>15}")
        ret = separator.join(line) + line_separator

        for i in range(len(self.traces[0])):
            # First column is time in seconds
            line = [f"{self.traces[0][i]:>15.3f}"]
            for j in range(len(self.scenario.trace_variable_names)):
                line.append("{:>15.3f}".format(self.traces[j + 1][i]))
            ret += separator.join(line) + line_separator

        return ret

    def as_dict(self):
        return {
            "start_time_utc": self.start_time_utc.isoformat(),
            "platform_uname": self.platform_uname,
            "tester_name": self.tester_name,
            "tester_email": self.tester_email,
            "git_sha": self.git_sha,
            "git_dirty": self.git_dirty,
            "scenario": self.scenario.as_dict(),
            "ventilator_settings": self.ventilator_settings,
            "traces": self.traces,
        }

    def save_json(self, print_self=False):
        if print_self:
            print("\nResults:")
            print(self)

        file_name = self.unique_name() + ".json"
        if print_self:
            print(f"Saving as {file_name}")
        with open(file_name, "w") as json_file:
            json.dump(self.as_dict(), json_file, indent=4)

    def plot(self, save, show):
        title = self.unique_name()

        timestamps_sec = self.traces[0]
        dat = self.traces[1:]
        figure, axes = plt.subplots(len(dat), sharex=True, squeeze=False)
        for i, d in enumerate(dat):
            var = self.scenario.trace_variable_names[i]
            axes[i][0].plot(timestamps_sec, d, color="C{}".format(i))
            axes[i][0].set_title(var)
            axes[i][0].set_ylabel(var)  # todo should be units
            axes[i][0].grid()
            axes[i][0].axhline(linewidth=1, color="black")
            # Draw a black gridline at y=0 to highlight the x-axis.

        plt.xlabel("Seconds")
        figure.suptitle(title)
        figure.set_figheight(12)
        figure.set_figwidth(16)

        if save:
            plt.savefig(
                title + ".png", format="png", metadata={"Title": title}, dpi=100
            )

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
        td.git_sha = data["git_sha"]
        td.git_dirty = data["git_dirty"]
        td.ventilator_settings = data["ventilator_settings"]
        td.traces = data["traces"]
        return td

    @staticmethod
    def from_json(file_name):
        with open(file_name, "r") as json_file:
            return TestData.from_dict(json.load(json_file))
