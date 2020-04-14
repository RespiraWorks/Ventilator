#include "serdes.h"
#include <pb_common.h>
#include <pb_decode.h>

#ifdef TEST_MODE
#include "command_handlers-mock.h"
#else
#include "command_handlers.h"
#endif

bool serdes_decode_incomming_packet(uint8_t *rx_buffer, uint8_t rx_buffer_len,
                                    size_t encoded_len,
                                    GuiAckHandler_t ack_handler,
                                    CommandHandler_t command_handler) {
  pb_istream_t stream = pb_istream_from_buffer(rx_buffer, rx_buffer_len);
  Packet packet = Packet_init_zero;
  bool status = pb_decode(&stream, Packet_fields, &packet);
  if (Packet_cmd_tag == packet.which_payload) {
    command_handler(packet.payload.cmd);
  } else if (Packet_gui_ack_tag == packet.which_payload) {
    ack_handler(packet.payload.gui_ack);
  } else {
    return false;
  }

  return status;
}
