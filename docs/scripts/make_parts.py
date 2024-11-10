#!/usr/bin/env python3

# Generates parts.rst from parts.json

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
import json
from pathlib import Path


def from_json(file_name: Path):
    ret = {}
    j = json.loads(file_name.read_text())
    for tsd in j:
        ret[tsd["pn"]] = tsd
    return ret


def make_table_line(supplier_data, unit) -> str:
    price = supplier_data["price"]
    price_units = supplier_data["price_units"]
    price_string = f"{price}"
    if price_units != 1:
        price_string += f"/{price_units}"
    ret = ""
    ret += "   * - " + supplier_data["supplier"] + "\n"
    ret += "     - `" + supplier_data["spn"] + " <" + supplier_data["url"] + ">`_\n"
    ret += f"     - {price}\n"
    ret += f"     - {price_units} {unit}\n"
    return ret


def make_table(supplier_data, unit) -> str:
    ret = ""
    ret += ".. list-table::\n"
    ret += "   :widths: 20 30 10 10\n"
    ret += "   :header-rows: 1\n"
    ret += "\n"
    ret += "   * - Supplier\n"
    ret += "     - Supplier P/N\n"
    ret += "     - Price (USD)\n"
    if len(unit):
        ret += f"     - / {unit}\n"
    else:
        ret += "     - / units\n"
    for datum in supplier_data:
        ret += make_table_line(datum, unit)
    return ret


def make_rst(part_data) -> str:
    pn = part_data.get("pn", None)
    notes = part_data.get("notes", None)
    manufacturer = part_data.get("manufacturer", None)
    mpn = part_data.get("mpn", None)
    avg_price = part_data.get("avg_price", None)
    price_unit = part_data.get("price_unit", None)
    price_string = f"{avg_price} USD"
    if len(price_unit):
        price_string += "/" + price_unit

    ret = ""
    ret += f"{pn}\n"
    ret += '"' * len(pn) + "\n"
    ret += "\n"
    ret += f"| **{notes}**\n"
    if len(manufacturer) or len(mpn):
        ret += f"| {manufacturer}: {mpn}\n"
    ret += f"| Price: ~{price_string}\n"
    ret += "\n"

    sources = part_data["sources"]
    if len(sources):
        ret += make_table(sources, price_unit) + "\n"
    return ret


def generate_parts_rst(in_file: Path, out_file: Path) -> None:
    parts = from_json(in_file)
    with open(out_file, "w") as f:
        f.write("Parts\n")
        f.write("-----\n")
        f.write("\n")
        for key in sorted(parts.keys()):
            print(f"  {key}")
            f.write(make_rst(parts[key]))


def json_arg(path: str) -> Path:
    ret = Path(path)
    if ret.suffix != ".json":
        raise argparse.ArgumentTypeError(f"Not a json file: {ret}")
    if not ret.is_file():
        raise argparse.ArgumentTypeError(f"File not found: {ret}")
    return ret


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "file",
        type=json_arg,
        help="File to process",
    )

    args = parser.parse_args()

    in_file = args.file
    out_file = in_file.with_suffix(".rst")

    print(f"Generating {in_file} -> {out_file}:")
    generate_parts_rst(in_file, out_file)


if __name__ == "__main__":
    main()
