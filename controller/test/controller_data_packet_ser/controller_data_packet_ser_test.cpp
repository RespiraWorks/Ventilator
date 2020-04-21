#include "gtest/gtest.h"

#include "network_protocol.pb.h"
#include "serdes.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <stdio.h>

#define PACKET_LEN_MAX (32)
uint8_t tx_buffer[PACKET_LEN_MAX];

TEST(SerDesTests, StatusSerialization) {

  size_t encoded_data_length;
  uint64_t time = 1;
  float pressure = 2.2;
  float volume = 3.3;
  float flow = 4.4;

  ControllerStatus controller_status = ControllerStatus_init_zero;
  controller_status.time = time;
  controller_status.flow = flow;
  controller_status.volume = volume;
  controller_status.pressure = pressure;

  bool status = serdes_encode_status_packet(
      controller_status, tx_buffer, PACKET_LEN_MAX, &encoded_data_length);

  ASSERT_TRUE(status);
  ASSERT_TRUE(encoded_data_length > 0);

  pb_istream_t stream = pb_istream_from_buffer(tx_buffer, encoded_data_length);

  Packet packet = Packet_init_zero;

  status = pb_decode(&stream, Packet_fields, &packet);

  ASSERT_TRUE(status);
  ASSERT_EQ(packet.which_payload, Packet_data_tag);
  ASSERT_EQ(packet.payload.data.msg_type, ControllerMsgType_DATA);
  ASSERT_EQ(packet.payload.data.which_payload, ControllerData_status_tag);
  ControllerStatus s = packet.payload.data.payload.status;
  ASSERT_EQ(s.time, time);
  ASSERT_FLOAT_EQ(s.pressure, pressure);
  ASSERT_FLOAT_EQ(s.volume, volume);
  ASSERT_FLOAT_EQ(s.flow, flow);
  // TODO when/if we use alarm flags TEST_ASSERT_EQUAL_INT16(s.alarm_flags,
  // time);
}

bool command_handler_called = false;

void command_handler(Command &cmd) {
  command_handler_called = true;
  ASSERT_EQ(CommandType_NONE, cmd.cmd);
  ASSERT_FLOAT_EQ(cmd.data, 42.42);
}

bool gui_ack_handler_called = false;

void gui_ack_handler(GuiAck &ack) { gui_ack_handler_called = true; }

TEST(SerDesTests, StatusDeserialization) {
  Command cmd = Command_init_zero;
  cmd.cmd = CommandType_NONE;
  cmd.has_data = true;
  cmd.data = 42.42;

  Packet packet = Packet_init_zero;
  packet.which_payload = Packet_cmd_tag;
  packet.payload.cmd = cmd;

  uint8_t rx_buffer[PACKET_LEN_MAX];
  pb_ostream_t stream = pb_ostream_from_buffer(rx_buffer, PACKET_LEN_MAX);
  pb_encode(&stream, Packet_fields, &packet);
  uint8_t encoded_len = stream.bytes_written;

  ASSERT_FALSE(gui_ack_handler_called);
  ASSERT_FALSE(command_handler_called);

  serdes_decode_incomming_packet(rx_buffer, PACKET_LEN_MAX, encoded_len,
                                 gui_ack_handler, command_handler);

  ASSERT_FALSE(gui_ack_handler_called);
  ASSERT_TRUE(command_handler_called);
}
