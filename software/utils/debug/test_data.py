#!/usr/bin/env python3
import datetime

from test_scenario import TestScenario
from typing import Dict, List
from datetime import datetime
import json
import platform
import git
import colors


class TestData:
    """Test data"""

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
        return self.start_time_utc.strftime("%Y-%m-%d-%H-%M-%S") +\
               "_" + self.tester_email.partition("@")[0] +\
               "_" + self.scenario.name

    def __str__(self):
        ret =  "Start time (UTC): {}\n".format(self.start_time_utc.strftime("%Y-%m-%d %H:%M:%S"))
        ret += "Machine info:     {}\n".format(self.platform_uname)
        ret += "Tester:           {} ({})\n".format(self.tester_name, self.tester_email)
        dirty_string = colors.Colors.RED + " (DIRTY)" + colors.Colors.ENDC
        ret += "Git sha:          {}{}\n".format(self.git_sha,
                                                 dirty_string if self.git_dirty else "")
        ret += "TEST SCENARIO:\n"\
               + self.scenario.long_description()
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
        return {'start_time_utc': self.start_time_utc.isoformat(),
                'platform_uname': self.platform_uname,
                'tester_name': self.tester_name,
                'tester_email': self.tester_email,
                'git_sha': self.git_sha,
                'git_dirty': self.git_dirty,
                'scenario': self.scenario.as_dict(),
                'ventilator_settings': self.ventilator_settings,
                'traces': self.traces}



    @staticmethod
    def from_dict(data):
        ts = TestScenario()
        ts.name = data.get('name', None)
        ts.description = data.get('description', None)
        ts.manual_settings = data.get('manual_settings', None)
        ts.ventilator_settings = data.get('ventilator_settings', None)
        ts.test_criteria = data.get('test_criteria', None)
        ts.capture_duration_secs = data.get('capture_duration_secs', None)
        ts.capture_ignore_secs = data.get('capture_ignore_secs', None)
        return ts

    @staticmethod
    def from_json(file_name):
        ret = {}
        with open(file_name, 'r') as json_file:
            j = json.load(json_file)
            for tsd in j:
                ts = TestScenario.from_dict(tsd)
                ret[ts.name] = ts
        return ret

