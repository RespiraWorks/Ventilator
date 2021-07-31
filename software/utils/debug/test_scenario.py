#!/usr/bin/env python3

import pandas  # pip install pandas
import dataclasses
from typing import Dict, Union
import argparse


@dataclasses.dataclass
class TestScenario:
    """A named list of debug variables and values."""

    id:          str
    description: str
    variables:   Dict[str, Union[int, float, str]]

    def short_description(self):
        return f"{self.id:15} \"{self.description}\""

    def long_description(self):
        return (
                self.short_description() + "\n"
                + "\n".join(f"  - {var:25} = {val}" for var, val in self.variables.items())
        )


def trim_all_columns(df):
    """
    Trim whitespace from ends of each value across all series in dataframe
    """
    trim_strings = lambda x: x.strip() if isinstance(x, str) else x
    return df.applymap(trim_strings)


def from_csv(file_name):
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
        ts = TestScenario(row["id"], row["description"], row)
        ts.variables.pop("id")
        ts.variables.pop("description")
        ret[ts.id] = ts

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
