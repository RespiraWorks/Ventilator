#!/usr/bin/env python3

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

import serial  # pip install pySerial
import network_protocol_pb2


class FrameDecoder:
    def __init__(self, port):
        self.p = port
        self.state = self.LOST
        self.s = b""
        self.frame = b""
        self.frame_available = False

    FRAMING_MARK = "\xE2"
    FRAMING_ESC = "\x27"

    LOST = 0
    WAIT_START = 1
    RX_FRAME = 2

    def on_char_match(self):
        if self.LOST == self.state:
            if len(self.s) > 1:
                self.state = self.WAIT_START
            elif len(self.s) == 1:
                self.state = self.RX_FRAME
            else:
                # panic!
                pass
        elif self.WAIT_START == self.state:
            if len(self.s) == 1:
                self.state = self.RX_FRAME
            else:
                self.state = LOST
        elif self.RX_FRAME == self.state:
            if len(self.s) > 1:
                self.frame = self.s
                self.frame_available = True
                pass  # GOT FRAME
            else:
                pass  # repeated MARK
        else:
            pass  # unknown state
        self.s = b""

    def unescape(self, s):
        ret = b""
        esc = False
        for b in s:
            if self.FRAMING_MARK == b:
                pass
            elif self.FRAMING_ESC == b:
                esc = True
            else:
                if esc:
                    esc = False
                    ret += chr(ord(b) ^ 0x20)
                else:
                    ret += b
        return ret

    def get_frame(self):
        while True:
            if self.p.in_waiting > 0:
                b = self.p.read()
                print(b)
                self.s += b
                if self.FRAMING_MARK == b:
                    self.on_char_match()
                if self.frame_available:
                    self.frame_available = False
                    return self.unescape(self.frame)


p = serial.Serial("/dev/ttyUSB0", 115200)
decoder = FrameDecoder(p)

while True:
    frame = decoder.get_frame()
    frame = frame[:-4]  # drop CRC for now
    try:
        stat = network_protocol_pb2.ControllerStatus()
        stat.ParseFromString(frame)
        print("------")
        print(stat)
    except:
        pass
