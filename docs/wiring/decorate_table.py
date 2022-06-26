#!/usr/bin/env python3

# Decorates tab-separated-table from wireviz, creating RST links
# Make every entry in the P/N column, e.g.:
#   pn1234
# look like this:
#   :ref:`pn1234`

__copyright__ = "Copyright 2022 RespiraWorks"

__license__ = """

    Copyright 2022 RespiraWorks

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

import argparse
import pandas
from pathlib import Path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "file",
        type=str,
        help="File to process",
    )

    args = parser.parse_args()

    if not args.file:
        print(f"No input file name provided {args.preset}")
        return

    in_file = Path(args.file)
    if not in_file.is_file():
        print(f"Input file does not exist {args.file}")
        return
    elif in_file.suffix == ".tsv":
        df = pandas.read_csv(in_file, sep="\t", header=0)
        for d in df["P/N"]:
            newd = f":ref:`{d}`"
            df["P/N"] = df["P/N"].replace([d], newd)

        pn_df = pandas.DataFrame(df, columns=["P/N"])

        out_file = in_file
        df.to_csv(out_file, sep="\t", float_format="%g", index=False)


if __name__ == "__main__":
    main()
