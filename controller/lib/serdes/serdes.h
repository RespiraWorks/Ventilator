#ifndef __SERDES_H
#define __SERDES_H
#include "network_protocol.pb.h"

typedef void (*CommandHandler_t)(Command &);
typedef void (*GuiAckHandler_t)(GuiAck &);

bool serdes_encode_status_packet(ControllerStatus controller_status,
                                 uint8_t *tx_buffer, uint8_t tx_buffer_len,
                                 size_t *encoded_len);
bool serdes_decode_incomming_packet(uint8_t *rx_buffer, uint8_t rx_buffer_len,
                                    size_t encoded_len,
                                    GuiAckHandler_t gui_ack_handler,
                                    CommandHandler_t command_handler);

#endif
