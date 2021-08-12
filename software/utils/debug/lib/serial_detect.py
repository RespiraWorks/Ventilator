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


def detect_stm32_ports():
    j = json.loads(
        subprocess.check_output(["platformio", "device", "list", "--json-output"])
    )
    ports = [d["port"] for d in j if "STM32" in d.get("description", "")]
    return ports


def print_detected_ports():
    ports = detect_stm32_ports()
    if not ports:
        print("No ports detected")
    else:
        print("STM32 devices detected on the following ports:")
        for p in ports:
            print(f"  {p}")


if __name__ == "__main__":
    print_detected_ports()
