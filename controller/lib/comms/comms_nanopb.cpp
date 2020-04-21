#include "comms.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include "serdes.h"

#define PACKET_LEN_MAX (32)
static uint8_t tx_buffer[PACKET_LEN_MAX];
static uint16_t tx_data_length;
static bool output_buffer_ready = false;

static uint8_t rx_buffer[PACKET_LEN_MAX];
static uint8_t rx_idx = 0;
static uint64_t last_rx = 0;
static bool rx_in_progress = false;

static constexpr uint16_t RX_TIMEOUT_MS = 1;

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

// command_handler_ptr is a level of indirection to allow tests to redirect
// command_handler to an alternative callback.
//
// TODO(jlebar): This is not the most elegant way to mock a function!
static
#ifndef TEST_MODE
    constexpr
#endif
    void (*command_handler_ptr)(Command &cmd) = &command_handler;

void comms_init() {}

static bool is_time_to_process_packet() {
  return Hal.millis() - last_rx > RX_TIMEOUT_MS;
}

void comms_sendResetState() {
  // TODO
}

void comms_sendControllerStatus(ControllerStatus controller_status) {
  // TODO solve tx overflow
  if (output_buffer_ready) {
    return;
  }

  // TODO(jlebar): Casting tx_data_length to size_t is not safe on systems
  // where size_t is 32 bits!
  bool status = serdes_encode_status_packet(
      controller_status, tx_buffer, PACKET_LEN_MAX, (size_t *)&tx_data_length);
  if (status) {
    output_buffer_ready = true;
  }
}

// NOTE this is work in progress.
// Proper framing incomming. Afproto will be used to encode data to form that
// can be safely sent over wire - with packet start/end markers and CRC

// TODO add frame markers
// TODO add marker escaping in contents
// TODO add CRC to whole packet

// TODO run this via DMA to free up resources for control loops
static void process_tx() {
  if (output_buffer_ready) {
    for (size_t i = 0; i < tx_data_length; i++) {
      uint16_t written = Hal.serialWrite(tx_buffer[i]);
      if (1 != written) {
        // TODO catch on fire
      }
    }
    output_buffer_ready = false;
  }
}

static void process_rx() {
  while (Hal.serialBytesAvailableForRead() > 0) {
    rx_in_progress = true;
    char b;
    uint16_t bytes_read = Hal.serialRead(&b, 1);
    if (bytes_read == 1) {
      rx_buffer[rx_idx++] = (uint8_t)b;
      if (rx_idx >= PACKET_LEN_MAX) {
        rx_idx = 0;
        break;
      }
      last_rx = Hal.millis();
    }
  }

  // TODO do away with timeout-based reception once we have framing inplace,
  // but it will work for Alpha build for now
  if (rx_in_progress && is_time_to_process_packet()) {
    serdes_decode_incomming_packet(rx_buffer, PACKET_LEN_MAX, rx_idx,
                                   gui_ack_handler, command_handler_ptr);
    rx_idx = 0;
    rx_in_progress = false;
  }
}

void comms_handler() {
  process_tx();
  process_rx();
}

#if TEST_MODE
void comms_test_set_command_handler(void (*handler)(Command &cmd)) {
  if (handler) {
    command_handler_ptr = handler;
  } else {
    command_handler_ptr = &command_handler;
  }
}
#endif
