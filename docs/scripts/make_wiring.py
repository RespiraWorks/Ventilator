__copyright__ = "Copyright 2024 RespiraWorks"

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

from wireviz import wireviz
from pathlib import Path
import pandas


def yaml_arg(path: str) -> Path:
    ret = Path(path)
    if ret.suffix not in (".yml", ".yaml"):
        raise argparse.ArgumentTypeError(f"Not a YAML file: {ret}")
    if not ret.is_file():
        raise argparse.ArgumentTypeError(f"File not found: {ret}")
    return ret


def dir_arg(path: str) -> Path:
    ret = Path(path)
    if not ret.is_dir():
        raise argparse.ArgumentTypeError(f"Not a directory: {ret}")
    return ret


def decorate_table(file: Path) -> None:
    """
    Decorates tab-separated-table from wireviz, creating RST links
    Make every entry in the P/N column, e.g.:
        pn1234
    look like this:
       :ref:`pn1234`
    """
    df = pandas.read_csv(file, sep="\t", header=0)
    for d in df["P/N"]:
        newd = f":ref:`{d}`"
        df["P/N"] = df["P/N"].replace([d], newd)

    df.to_csv(file, sep="\t", float_format="%g", index=False)


def generate_wiring_diagram(yaml_file: Path) -> None:
    """ """
    print(f"Generating wiring diagram for {yaml_file}")
    wireviz.parse(yaml_file, output_formats=("gv", "html", "png", "svg", "tsv"))
    tsv_file = yaml_file.parent / (yaml_file.stem + ".bom.tsv")
    print(f"Will modify {tsv_file}")
    decorate_table(tsv_file)


def main():
    parser = argparse.ArgumentParser()
    group = parser.add_mutually_exclusive_group(required=True)

    group.add_argument(
        "-f",
        "--file",
        type=yaml_arg,
        help="File to process",
    )
    group.add_argument(
        "-d",
        "--directory",
        type=dir_arg,
        help="Directory to process",
    )

    args = parser.parse_args()

    if args.file:
        generate_wiring_diagram(args.file)
    elif args.directory:
        for file in args.directory.rglob("**/*.yml"):
            generate_wiring_diagram(file)


if __name__ == "__main__":
    main()
