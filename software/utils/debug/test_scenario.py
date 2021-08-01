#!/usr/bin/env python3

import pandas  # pip install pandas
from typing import Dict, Union
import argparse


class TestScenario:
    """A named list of debug variables and values."""

    name:        str
    description: str
    manual_variables: Dict[str, Union[int, float, str]] = {}
    settable_variables: Dict[str, Union[int, float, str]] = {}

    def short_description(self):
        return f"{self.name:15} \"{self.description}\""

    def long_description(self, list_settable=True):
        ret = self.short_description()
        ret += "\n  Manual variables\n"
        ret += "\n".join(f"    {var:25} = {val}" for var, val in self.manual_variables.items())
        if list_settable:
            ret += "\n  Settable variables\n"
            ret += "\n".join(f"    {var:25} = {val}" for var, val in self.settable_variables.items())
        return ret


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
        # print(row)
        if "id" not in row:
            raise Exception(f"Row does not contain id:\n{row}")
        if "description" not in row:
            raise Exception(f"Row does not contain description:\n{row}")
        ts = TestScenario()
        ts.manual_variables = row
        ts.name = ts.manual_variables.pop("id")
        ts.description = ts.manual_variables.pop("description")
        intersect = set(ts.manual_variables.keys()).intersection(settable_variables)
        ts.settable_variables = {k: ts.manual_variables[k] for k in intersect}
        for k in intersect:
            ts.manual_variables.pop(k)
        ret[ts.name] = ts
    return ret


def main():

    parser = argparse.ArgumentParser()
    parser.add_argument("file", type=str,
                        help="csv file from which to load test scenario configuration")

    args = parser.parse_args()

    if not args.file:
        print(f"No file name provided {args.preset}")
        return

    scenarios = from_csv(args.file)

    for key in scenarios:
        print(scenarios[key].short_description())


if __name__ == "__main__":
    main()
