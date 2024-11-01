# use socat -d -d pty,raw,echo=0 pty,raw,echo=0
# you will get something like this:

# 2020/05/02 22:27:09 socat[23367] N PTY is /dev/pts/1
# 2020/05/02 22:27:09 socat[23367] N PTY is /dev/pts/6
# 2020/05/02 22:27:09 socat[23367] N starting data transfer loop with FDs [5,5] and [7,7]

# note the /dev/pts/*,
# poetry run mock-controller /dev/pts/1
# and specify the other one for ventilator GUI

import serial
from protocols import network_protocol_pb2
import time
import argparse
import math


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("serialport", metavar="SERIAL", type=str, help="Serial port")
    parser.add_argument(
        "interframe_interval_ms",
        metavar="INTERVAL",
        type=int,
        help="Milliseconds between frames",
    )
    args = parser.parse_args()

    port = serial.Serial(args.serialport, 115200)

    stat = network_protocol_pb2.ControllerStatus()
    stat.uptime_ms = int(time.time())
    stat.pressure_setpoint_cm_h2o = 1
    stat.fan_power = 0.8

    stat.sensor_readings.breath_id = 0
    stat.sensor_readings.patient_pressure_cm_h2o = 0.9
    stat.sensor_readings.volume_ml = -0.5
    stat.sensor_readings.flow_ml_per_min = 10
    stat.sensor_readings.inflow_pressure_diff_cm_h2o = 0.5
    stat.sensor_readings.outflow_pressure_diff_cm_h2o = 0.5
    stat.sensor_readings.flow_correction_ml_per_min = 0.5
    stat.sensor_readings.fio2 = 0.5

    stat.active_params.fio2 = 0.5
    stat.active_params.mode = network_protocol_pb2.OFF
    stat.active_params.peep_cm_h2o = 2
    stat.active_params.breaths_per_min = 8
    stat.active_params.pip_cm_h2o = 1
    stat.active_params.inspiratory_expiratory_ratio = 0.5
    stat.active_params.inspiratory_trigger_cm_h2o = 2
    stat.active_params.expiratory_trigger_ml_per_min = 900

    i = 0
    while True:
        stat.sensor_readings.breath_id += 1
        stat.sensor_readings.patient_pressure_cm_h2o = math.sin(i)
        port.write(stat.SerializeToString())
        port.flush()
        while port.in_waiting > 0:
            port.read()
        time.sleep(0.001 * args.interframe_interval_ms)
        i += 0.01
