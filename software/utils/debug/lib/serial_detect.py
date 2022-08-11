#!/usr/bin/env python3

# Utility to find serial port to which an STM controller is connected to

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

import json
import subprocess
from .colors import green, red, gray


def detect_stm32_ports():
    scanned_devices = json.loads(
        subprocess.check_output(["platformio", "device", "list", "--json-output"])
    )
    ports = {}
    for entry in scanned_devices:
        if "STM32" in entry.get("description", ""):
            port = entry["port"]
            hla_serial = entry["hwid"].split()[2].split("=")[1]
            ports[port] = hla_serial
    return ports


def check_value(data, val):
    return any(entry["hla_serial"] == val for entry in data)


def list_devices(file_name, unknown=True, disconnected=True):
    ports = detect_stm32_ports()
    ports_inv = {v: k for k, v in ports.items()}
    json_file = open(file_name, "r")
    manifest = json.load(json_file)
    for entry in manifest:
        hla = entry.get("hla_serial")
        if hla in ports_inv:
            entry["port"] = ports_inv[hla]
        else:
            entry["port"] = ""
    for hla in ports_inv:
        if not any(entry["hla_serial"] == hla for entry in manifest):
            manifest.append(
                {
                    "hla_serial": hla,
                    "port": ports_inv[hla],
                    "alias": "",
                    "description": "",
                    "configuration": "",
                }
            )
    for entry in manifest:
        outstr = (
            f" {entry.get('alias'):<10} {entry.get('port'):<15} {entry.get('hla_serial'):>24} "
            f"{entry.get('configuration'):<12} {entry.get('description')} "
        )
        if len(entry.get("port")) == 0:
            print(gray(outstr))
        elif len(entry.get("alias")) == 0:
            print(red(outstr))
        else:
            print(green(outstr))


def print_detected_ports():
    ports = detect_stm32_ports()
    if not ports:
        print("No ports detected")
    else:
        print("STM32 devices detected on the following ports:")
        for p in ports:
            print(f"  {p} = {ports[p]}")


if __name__ == "__main__":
    print_detected_ports()
