#!/usr/bin/env python3
import json
from typing import Dict, List
import pandas  # pip install pandas
import copy
import argparse

# todo can we also have units and/or uncertainties defined for some variables


class TestScenario:
    """A named list of debug variables and values."""

    name:        str = None
    description: str = None
    manual_variables: Dict
    settable_variables: Dict

    def __init__(self):
        self.manual_variables = {}
        self.settable_variables = {}

    def short_description(self):
        return f"{self.name:15} \"{self.description}\""

    def long_description(self, list_settable=True):
        ret = self.short_description()
        if len(self.manual_variables):
            ret += "\n  Manual variables\n"
            ret += "\n".join(f"    {var:25} = {val}" for var, val in self.manual_variables.items())
        if list_settable and len(self.settable_variables):
            ret += "\n  Settable variables\n"
            ret += "\n".join(f"    {var:25} = {val}" for var, val in self.settable_variables.items())
        return ret

    def as_dict(self):
        return {'name': self.name, 'description': self.description,
                'manual_variables': self.manual_variables,
                'settable_variables': self.settable_variables}

    def __iter__(self):
        yield 'name', self.name
        yield 'description', self.description
        yield 'manual_variables', self.manual_variables
        yield 'settable_variables', self.settable_variables

    @staticmethod
    def from_dict(data):
        ts = TestScenario()
        ts.name = data.get('name', None)
        ts.description = data.get('description', None)
        ts.manual_variables = data.get('manual_variables', None)
        ts.settable_variables = data.get('settable_variables', None)
        return ts

    # def to_json(self):
    #     return json.dumps(self.manual_variables)
    #
    # def toJSON(self):
    #     return json.dumps(self, default=lambda o: o.__dict__, sort_keys=True, indent=4)


def trim_all_columns(df):
    """
    Trim whitespace from ends of each value across all series in dataframe
    """
    trim_strings = lambda x: x.strip() if isinstance(x, str) else x
    return df.applymap(trim_strings)


def from_csv(file_name, settable_variables):
    df = pandas.read_csv(file_name)
    df.columns = df.columns.str.replace(' ', '')
    df = trim_all_columns(df)

    ret = {}
    for index, row in df.iterrows():
        if index > 1:
            break
        # print(row)
        if "id" not in row:
            raise Exception(f"Row does not contain id:\n{row}")
        if "description" not in row:
            raise Exception(f"Row does not contain description:\n{row}")
        ts = TestScenario()
        row_copy = copy.deepcopy(row)
        ts.name = row_copy.pop("id")
        ts.description = row_copy.pop("description")
        for k in row_copy.keys():
            if k in settable_variables:
                ts.settable_variables[k] = row_copy[k]
            else:
                ts.manual_variables[k] = row_copy[k]
        ret[ts.name] = ts
    # todo more checks, such as no duplicate labeled columns
    return ret


def main():

    parser = argparse.ArgumentParser()
    parser.add_argument("file", type=str,
                        help="csv file from which to load test scenario configuration")
    parser.add_argument("out_file", type=str,
                        help="json file for saving")

    args = parser.parse_args()

    if not args.file:
        print(f"No file name provided {args.preset}")
        return

    scenarios = from_csv(args.file, {"gui_mode", "gui_pip", "gui_peep",
                                     "gui_bpm", "gui_ie_ratio", "gui_fio2"})

    for key in scenarios:
        print(scenarios[key].short_description())

    # jsonpickle.set_encoder_options('simplejson', compactly=False, indent=4)

    for key in scenarios:
        j = json.dumps(dict(scenarios[key]))
        print(j)
        ts = TestScenario.from_dict(json.loads(j))
        print(ts.long_description())

    if args.out_file:
        tsl = []
        for key in scenarios:
            tsl.append(dict(scenarios[key]))
        with open(args.out_file, 'w') as json_file:
            json.dump(tsl, json_file)


if __name__ == "__main__":
    main()
