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

from util.colors import green, red, gray
import json
import pandas
import subprocess
from typing import List
from pathlib import Path
import urllib.request
import shutil


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


class DeviceManifest:
    device_list: List

    def __init__(self):
        self.device_list = []

    def filter(self, known=False, connected=False):
        filtered = DeviceManifest()
        for m in self.device_list:
            if not len(m.alias) and known:
                continue
            if not len(m.port) and connected:
                continue
            filtered.device_list.append(m)
        return filtered

    def list_devices(self):
        return_string = ""
        for entry in self.device_list:
            return_string += f" {entry.print()}\n"
        return return_string

    def get(self, alias):
        for device in self.device_list:
            if device.alias == alias:
                return device
        return None

    def find(self, port):
        for device in self.device_list:
            if device.port == port:
                return device
        return None

    def auto_select(self):
        if not self.device_list:
            print(red("Could not auto-select: no devices connected."))
            return None
        if len(self.device_list) > 1:
            print(red("Could not auto-select: multiple devices connected."))
            print(self.list_devices())
            return None
        return self.device_list[0]


class DeviceScanner:
    cached_manifest_path: Path

    def __init__(self, cached_path):
        self.cached_manifest_path = cached_path

    def update_manifest(self, url):
        with urllib.request.urlopen(url) as response, open(
            self.cached_manifest_path, "wb"
        ) as out_file:
            shutil.copyfileobj(response, out_file)

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

    def get_devices(self):
        ports = DeviceScanner.detect_stm32_ports()
        ports_inverted = {v: k for k, v in ports.items()}
        loaded_manifest = json.loads(
            pandas.read_csv(self.cached_manifest_path, sep="\t", header=0).to_json(
                orient="records"
            )
        )
        manifest = DeviceManifest()
        for entry in loaded_manifest:
            dev = DeviceInfo(
                alias=entry.get("Alias"),
                hla_serial=entry.get("HLA serial"),
                configuration=entry.get("Configuration"),
                description=entry.get("Description"),
            )
            if dev.hla_serial in ports_inverted:
                dev.port = ports_inverted[dev.hla_serial]
            manifest.device_list.append(dev)
        for hla in ports_inverted:
            if not any(entry.hla_serial == hla for entry in manifest.device_list):
                dev = DeviceInfo(hla_serial=hla, description="unregistered_device")
                dev.port = ports_inverted[hla]
                manifest.device_list.append(dev)
        return manifest

    def rescan(self):
        return


def main():
    MANIFEST_URL = "https://docs.google.com/spreadsheets/d/e/2PACX-1vRduOfterWmAy_xrc356rRhjz4QDLgOScgG1VPx2-KNeH8zYEe29SCw_DKOJG-5hqSO6BXmG1BumUul/pub?gid=0&single=true&output=tsv"
    mod_path = Path(__file__).parent
    rel_path = "../../../../local_data/device_manifest.tsv"
    CACHED_MANIFEST_PATH = (mod_path / rel_path).resolve()

    print(CACHED_MANIFEST_PATH)
    device_finder = DeviceScanner(CACHED_MANIFEST_PATH)
    device_finder.update_manifest(MANIFEST_URL)

    devices = device_finder.get_devices()
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


if __name__ == "__main__":
    main()
