#include "serdes.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

bool serdes_decode_incomming_packet(uint8_t *rx_buffer, uint8_t rx_buffer_len,
                                    uint16_t encoded_len,
                                    GuiAckHandler_t gui_ack_handler,
                                    CommandHandler_t command_handler) {
  pb_istream_t stream = pb_istream_from_buffer(rx_buffer, encoded_len);
  Packet packet = Packet_init_zero;
  bool status = pb_decode(&stream, Packet_fields, &packet);
  if (status) {
    if (Packet_cmd_tag == packet.which_payload) {
      command_handler(packet.payload.cmd);
    } else if (Packet_gui_ack_tag == packet.which_payload) {
      gui_ack_handler(packet.payload.gui_ack);
    } else {
      return false;
    }
  }

  return status;
}

bool serdes_encode_status_packet(ControllerStatus controller_status,
                                 uint8_t *tx_buffer, uint8_t tx_buffer_len,
                                 uint16_t *encoded_len) {
  pb_ostream_t stream = pb_ostream_from_buffer(tx_buffer, tx_buffer_len);


  ControllerData cdp = ControllerData_init_zero;
  cdp.msg_type = ControllerMsgType_DATA;
  cdp.response_to_cmd = CommandType_NONE;
  cdp.which_payload = ControllerData_status_tag;
  cdp.payload.status = controller_status;

  Packet packet = Packet_init_zero;
  packet.which_payload = Packet_data_tag;
  packet.payload.data = cdp;

  bool status = pb_encode(&stream, Packet_fields, &packet);

  *encoded_len = stream.bytes_written;

  return status;
}
