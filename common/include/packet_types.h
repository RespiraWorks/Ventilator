/* Copyright 2020, Edwin Chiu

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef PACKET_TYPES_H
#define PACKET_TYPES_H

/*
 * Stores all command numbers that can sent by the GUI
 */
enum class command {

  /* Medical mode commands */

  set_rr = 0x00,
  get_rr = 0x01,
  set_tv = 0x02,
  get_tv = 0x03,
  set_peep = 0x04,
  get_peep = 0x05,
  set_pip = 0x06,
  get_pip = 0x07,
  set_dwell = 0x08,
  get_dwell = 0x09,
  set_ier = 0x0a, /* Inspiration / Expiration ratio */
  get_ier = 0x0b,

  get_pressure = 0x0c,
  get_flow = 0x0d,
  get_volume = 0x0e,

  /* Engineering mode commands */

  set_kp = 0x20, /* PID constant Kp */
  get_Kp = 0x21,
  set_Ki = 0x22, /* PID constant Ki */
  get_Ki = 0x23,
  set_Kd = 0x24, /* PID constant Kd */
  get_Kd = 0x25,

  set_blower = 0x26,              /* Turn blower ON/OFF */
  reset_vc = 0x27,                /* Reset Ventilation Controller */
  set_solenoidNormalState = 0x28, /* Solenoid normal state (open/closed) */

  /* Mixed Engineering/Medical mode commands */

  set_periodic = 0x40, /* Periodic transmission mode (Pressure, Flow, Volume) */
  get_periodic = 0x41,
  set_mode = 0x42, /* Engineering or medical mode */
  get_mode = 0x43,
  comms_check = 0x44,        /* Communications check command */
  set_ventilatorMode = 0x45, /* Set ventilator mode */
  get_ventilatorMode = 0x46,
  start_ventilator = 0x47, /* Start the ventilator */
  stop_ventilator = 0x48,  /* Stop the ventilator */

  count /* Sentinel */
};

/*
 * Stores all message types that can sent by the Ventilator controller and the
 * GUI
 */
enum class msgType {
  cmd = 0x00,  /* Command */
  ack = 0x01,  /* Ventilator Controller alarm Ack */
  nAck = 0x02, /* Ventilator Controller alarm Fail */

  rAck = 0x10,         /* Response Ack */
  rErrChecksum = 0x11, /* Response checksum error */
  rErrMode = 0x12,     /* Response mode error */
  rErrCmd = 0x13,      /* Response cmd error */

  status = 0x20, /* Status */
  alarm = 0x30,  /* Alarm */
  data = 0x40,   /* Data */

  count /* Sentinel */
};

enum class dataID {
  /* DataID values should start at a value higher than the higest command number
   */

  /* Status */
  vc_boot =
      0x80, /* Status sent when arduino boots (includes software version) */

  /* Alarms */
  alarm_1 = 0xA0,
  alarm_2 = 0xA1,

  /* Data */
  data_1 = 0xC0,

  count /* Sentinel */
};

// The different periodic data transmission modes
enum class periodicMode {
  off = 0x00,
  on = 0x01,

  count /* Sentinel */
};

// The different engineering operating mode types
enum class operatingMode {
  medical = 0x00,
  engineering = 0xF0,

  count /* Sentinel */
};

// Solenoid state definitions
enum class solenoidNormaleState {
  normally_open = 0x00,
  normally_closed = 0x01,

  count /* Sentinel */
};

// Ventilator medical mode types
enum class ventilatorMode {
  PRVC = 0x00,
  ACV = 0x01,

  count /* Sentinel */
};

#endif // PACKET_TYPES_H
