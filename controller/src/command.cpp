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

#include "command.h"

// Converts 4 bytes of big-endian data into a float.
static float convIntTofloat(char *data);

// Converts a float into 4 bytes of big-endian data.
static uint32_t convfloatToInt(float float_value);

void command_execute(enum command cmd, char *dataTx, uint8_t lenTx,
                     char *dataRx, uint8_t *lenRx, uint8_t lenRxMax) {

  float rr, tv, peep, dwell, ier, pip, Ki, Kd, Kp;
  *lenRx = 0; // Initialise the value to zero
  switch (cmd) {
    /* Medical mode commands */
  case command::set_rr:
    rr = convIntTofloat(dataTx);
    parameters_setRR(rr);
    break;
  case command::get_rr:
    *lenRx = 4;
    rr = convfloatToInt(parameters_getRR());
    memcpy(dataRx, &rr, sizeof(uint32_t));
    break;
  case command::set_tv:
    tv = convIntTofloat(dataTx);
    parameters_setTV(tv);
    break;
  case command::get_tv:
    *lenRx = 4;
    tv = convfloatToInt(parameters_getTV());
    memcpy(dataRx, &tv, sizeof(uint32_t));
    break;
  case command::set_peep:
    peep = convIntTofloat(dataTx);
    parameters_setPEEP(peep);
    break;
  case command::get_peep:
    *lenRx = 4;
    peep = convfloatToInt(parameters_getPEEP());
    memcpy(dataRx, &peep, sizeof(uint32_t));
    break;
  case command::set_pip:
    pip = convIntTofloat(dataTx);
    parameters_setPIP(pip);
    break;
  case command::get_pip:
    *lenRx = 4;
    pip = convfloatToInt(parameters_getPIP());
    memcpy(dataRx, &pip, sizeof(uint32_t));
    break;
  case command::set_dwell:
    dwell = convIntTofloat(dataTx);
    parameters_setDwell(dwell);
    break;
  case command::get_dwell:
    *lenRx = 4;
    dwell = convfloatToInt(parameters_getDwell());
    memcpy(dataRx, &dwell, sizeof(uint32_t));
    break;
  case command::set_ier:
    ier = convIntTofloat(dataTx);
    parameters_setInspireExpireRatio(ier);
    break;
  case command::get_ier:
    *lenRx = 4;
    ier = convfloatToInt(parameters_getInspireExpireRatio());
    memcpy(dataRx, &ier, sizeof(uint32_t));
    break;
  case command::get_pressure:
    break;
  case command::get_flow:
    break;
  case command::get_volume:
    break;
    /* Engineering mode commands */
  case command::set_kp:
    Kp = convIntTofloat(dataTx);
    parameters_setKp(Kp);
    break;
  case command::get_Kp:
    *lenRx = 4;
    Kp = convfloatToInt(parameters_getKp());
    memcpy(dataRx, &Kp, sizeof(uint32_t));
    break;
  case command::set_Ki:
    Ki = convIntTofloat(dataTx);
    parameters_setKi(Ki);
    break;
  case command::get_Ki:
    *lenRx = 4;
    Ki = convfloatToInt(parameters_getKi());
    memcpy(dataRx, &Ki, sizeof(uint32_t));
    break;
  case command::set_Kd:
    Kd = convIntTofloat(dataTx);
    parameters_setKi(Kd);
    break;
  case command::get_Kd:
    *lenRx = 4;
    Kd = convfloatToInt(parameters_getKd());
    memcpy(dataRx, &Kd, sizeof(uint32_t));
    break;
  case command::set_blower:
    break;
  case command::reset_vc:
    // TODO Do any necessary cleaning up before reset
    reset_device();
    break;

    /* Mixed mode commands */
  case command::set_periodic:
    parameters_setPeriodicMode((enum periodicMode)dataTx[0]);
    break;
  case command::get_periodic:
    *lenRx = 1;
    dataRx[0] = (char)parameters_getPeriodicMode();
    break;
  case command::set_mode:
    parameters_setOperatingMode((enum operatingMode)dataTx[0]);
    break;
  case command::get_mode:
    *lenRx = 1;
    dataTx[0] = (char)parameters_getOperatingMode();
    break;
  case command::comms_check:
    break;
  case command::set_ventilatorMode:
    parameters_setVentilatorMode((enum ventilatorMode)dataTx[0]);
    break;
  case command::get_ventilatorMode:
    *lenRx = 1;
    dataTx[0] = (char)parameters_getVentilatorMode();
    break;
  case command::start_ventilator:
    break;
  case command::stop_ventilator:
    break;
  case command::set_solenoidNormalState:
    parameters_setSolenoidNormalState((enum solenoidNormaleState)dataTx[0]);
    break;

  default:
    // TODO: Remove this `default` clause so the compiler can check that we
    // handle all commands.
    break;
  }
}

void command_responseSend(uint8_t cmd, char *packet, uint8_t len) {
  serialIO_send(msgType::rAck, (enum dataID)cmd, packet, len);
}

static float convIntTofloat(char *data) {
    float f;

#ifndef __FLOAT_WORD_ORDER__
#error "Your compiler isn't defining the __FLOAT_WORD_ORDER__ macro?"
#elif __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__
    /* Convert big endian integer to little endian float */
    uint32_t data_bEndian;
    memcpy(&data_bEndian, data, sizeof(data_bEndian));
    uint32_t data_lEndian = __builtin_bswap32(data_bEndian);
    memcpy(&f, &data_lEndian, sizeof(f));
#else
    /* Convert big endian to big endian float */
    memcpy(&f, &data, sizeof(f));
#endif

    return f;
}

static uint32_t convfloatToInt(float float_value) {
#ifndef __FLOAT_WORD_ORDER__
#error "Your compiler isn't defining the __FLOAT_WORD_ORDER__ macro?"
#elif __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__
  /* Convert float into big endian format for transmission */
  uint32_t data_lEndian;
  memcpy(&data_lEndian, &float_value, sizeof(data_lEndian));
  return __builtin_bswap32(data_lEndian);
#else
  /* Keep float in big endian format */
  uint32_t data_bEndian;
  memcpy(&data_bEndian, &float_value, sizeof(data_bEndian));
  return data_bEndian;
#endif
}
