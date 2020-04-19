#include "serdes.h"
void command_handler(Command &) {
  // TODO move to nanopb Command type

  // // TODO(lee-matthews): We need to validate that we don't go past dataTx
  // (void)lenTx;
  // // TODO(lee-matthews): We need to ensure that we don't set lenRx >
  // lenRxMax. (void)lenRxMax;

  // float rr, tv, peep, dwell, ier, pip, Ki, Kd, Kp;
  // *lenRx = 0; // Initialise the value to zero
  // switch (cmd) {
  //   /* Medical mode commands */
  // case command::set_rr:
  //   rr = convIntTofloat(dataTx);
  //   parameters_setRR(rr);
  //   break;
  // case command::get_rr:
  //   *lenRx = 4;
  //   rr = convfloatToInt(parameters_getRR());
  //   memcpy(dataRx, &rr, sizeof(uint32_t));
  //   break;
  // case command::set_tv:
  //   tv = convIntTofloat(dataTx);
  //   parameters_setTV(tv);
  //   break;
  // case command::get_tv:
  //   *lenRx = 4;
  //   tv = convfloatToInt(parameters_getTV());
  //   memcpy(dataRx, &tv, sizeof(uint32_t));
  //   break;
  // case command::set_peep:
  //   peep = convIntTofloat(dataTx);
  //   parameters_setPEEP(peep);
  //   break;
  // case command::get_peep:
  //   *lenRx = 4;
  //   peep = convfloatToInt(parameters_getPEEP());
  //   memcpy(dataRx, &peep, sizeof(uint32_t));
  //   break;
  // case command::set_pip:
  //   pip = convIntTofloat(dataTx);
  //   parameters_setPIP(pip);
  //   break;
  // case command::get_pip:
  //   *lenRx = 4;
  //   pip = convfloatToInt(parameters_getPIP());
  //   memcpy(dataRx, &pip, sizeof(uint32_t));
  //   break;
  // case command::set_dwell:
  //   dwell = convIntTofloat(dataTx);
  //   parameters_setDwell(dwell);
  //   break;
  // case command::get_dwell:
  //   *lenRx = 4;
  //   dwell = convfloatToInt(parameters_getDwell());
  //   memcpy(dataRx, &dwell, sizeof(uint32_t));
  //   break;
  // case command::set_ier:
  //   ier = convIntTofloat(dataTx);
  //   parameters_setInspireExpireRatio(ier);
  //   break;
  // case command::get_ier:
  //   *lenRx = 4;
  //   ier = convfloatToInt(parameters_getInspireExpireRatio());
  //   memcpy(dataRx, &ier, sizeof(uint32_t));
  //   break;
  // case command::get_pressure:
  //   break;
  // case command::get_flow:
  //   break;
  // case command::get_volume:
  //   break;
  //   /* Engineering mode commands */
  // case command::set_kp:
  //   Kp = convIntTofloat(dataTx);
  //   parameters_setKp(Kp);
  //   break;
  // case command::get_Kp:
  //   *lenRx = 4;
  //   Kp = convfloatToInt(parameters_getKp());
  //   memcpy(dataRx, &Kp, sizeof(uint32_t));
  //   break;
  // case command::set_Ki:
  //   Ki = convIntTofloat(dataTx);
  //   parameters_setKi(Ki);
  //   break;
  // case command::get_Ki:
  //   *lenRx = 4;
  //   Ki = convfloatToInt(parameters_getKi());
  //   memcpy(dataRx, &Ki, sizeof(uint32_t));
  //   break;
  // case command::set_Kd:
  //   Kd = convIntTofloat(dataTx);
  //   parameters_setKi(Kd);
  //   break;
  // case command::get_Kd:
  //   *lenRx = 4;
  //   Kd = convfloatToInt(parameters_getKd());
  //   memcpy(dataRx, &Kd, sizeof(uint32_t));
  //   break;
  // case command::set_blower:
  //   break;
  // case command::reset_vc:
  //   // TODO Do any necessary cleaning up before reset
  //   reset_device();
  //   break;

  //   /* Mixed mode commands */
  // case command::set_periodic:
  //   parameters_setPeriodicMode((enum periodicMode)dataTx[0]);
  //   break;
  // case command::get_periodic:
  //   *lenRx = 1;
  //   dataRx[0] = (char)parameters_getPeriodicMode();
  //   break;
  // case command::set_mode:
  //   parameters_setOperatingMode((enum operatingMode)dataTx[0]);
  //   break;
  // case command::get_mode:
  //   *lenRx = 1;
  //   dataTx[0] = (char)parameters_getOperatingMode();
  //   break;
  // case command::comms_check:
  //   break;
  // case command::set_ventilatorMode:
  //   parameters_setVentilatorMode((enum ventilatorMode)dataTx[0]);
  //   break;
  // case command::get_ventilatorMode:
  //   *lenRx = 1;
  //   dataTx[0] = (char)parameters_getVentilatorMode();
  //   break;
  // case command::start_ventilator:
  //   break;
  // case command::stop_ventilator:
  //   break;
  // case command::set_solenoidNormalState:
  //   parameters_setSolenoidNormalState((enum solenoidNormaleState)dataTx[0]);
  //   break;

  // default:
  //   // TODO: Remove this `default` clause so the compiler can check that we
  //   // handle all commands.
  //   break;
  // }
}

void gui_ack_handler(GuiAck &) {}
