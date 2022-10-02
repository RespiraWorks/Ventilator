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
import gdown
import subprocess
from typing import List
from colors import green, red, gray

class DeviceInfo:
    alias: str
    port: str
    hla_serial: str
    configuration: str
    description: str

    def __init__(self, alias="", hla_serial="", configuration="", description=""):
        self.port = ""
        self.alias = alias
        self.hla_serial = hla_serial
        self.configuration = configuration
        self.description = description

    def print(self):
        output_string = (
            f"{self.alias:<10} {self.port:<15} {self.hla_serial:>25} "
            f"{self.configuration:<12} {self.description}"
        )
        if len(self.port) == 0:
            return gray(output_string)
        elif len(self.alias) == 0:
            return red(output_string)
        else:
            return green(output_string)

class DeviceScanner:
    manifest: List

    def __init__(self, file_name = ""):
        self.manifest = []
        if file_name:
            self.get_devices(file_name)

    @staticmethod
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


    @staticmethod
    def check_value(data, val):
        return any(entry["hla_serial"] == val for entry in data)

    def get_devices(self, file_name):
        ports = DeviceScanner.detect_stm32_ports()
        ports_inverted = {v: k for k, v in ports.items()}
        json_file = open(file_name, "r")
        loaded_manifest = json.load(json_file)
        for entry in loaded_manifest:
            dev = DeviceInfo(alias=entry.get("alias"),
                             hla_serial=entry.get("hla_serial"),
                             configuration=entry.get("configuration"),
                             description=entry.get("description"))
            if dev.hla_serial in ports_inverted:
                dev.port = ports_inverted[dev.hla_serial]
            self.manifest.append(dev)
        for hla in ports_inverted:
            if not any(entry.hla_serial == hla for entry in self.manifest):
                dev = DeviceInfo(hla_serial=hla, description="unregistered_device")
                dev.port = ports_inverted[hla]
                self.manifest.append(dev)

    def filter(self, known=False, connected=False):
        filtered = DeviceScanner()
        for m in self.manifest:
            if not len(m.alias) and known:
                continue
            if not len(m.port) and connected:
                continue
            filtered.manifest.append(m)
        return filtered

    def list_devices(self):
        return_string = ""
        for entry in self.manifest:
            return_string += f" {entry.print()}\n"
        return return_string

    def get(self, alias):
        for device in self.manifest:
            if device.alias == alias:
                return device
        return None

    def find(self, port):
        for device in self.manifest:
            if device.port == port:
                return device
        return None

    def auto_select(self):
        if not self.manifest:
            print(red(
                "Could not auto-detect serial port; "
                "platformio device list did not yield any STM32 devices."
            ))
            return None
        if len(self.manifest) > 1:
            print(red(
                "Could not auto-detect serial port; "
                "platformio device list yielded multiple STM32 devices:\n"
                f"{self.list_devices()}"
            ))
            return None
        return self.manifest[0]
        # "Choose port explicitly with --port."

if __name__ == "__main__":
    url = "https://drive.google.com/file/d/11Hsspy9_VFy6HVnV8iIkUo0Y2vPf67Lm/view?usp=sharing"
    output = "device_list.json"
    gdown.download(url=url, output=output, quiet=True, fuzzy=True)

    devices = DeviceScanner("device_list.json")
    print("ALL: ")
    print(devices.list_devices())
    print("ONLY KNOWN: ")
    print(devices.filter(known=True).list_devices())
    print("ONLY CONNECTED: ")
    print(devices.filter(connected=True).list_devices())
    print("ONLY KNOWN AND CONNECTED: ")
    print(devices.filter(known=True, connected=True).list_devices())
    print("SELECTING FROM ALL: ")
    sel = devices.auto_select()
    if sel:
        print(sel.print())
    else:
        print("None")
    print("SELECTING FROM KNOWN AND CONNECTED: ")
    sel = devices.filter(known=True, connected=True).auto_select()
    if sel:
        print(sel.print())
    else:
        print("None")
