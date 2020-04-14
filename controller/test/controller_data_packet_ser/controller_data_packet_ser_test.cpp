#include "unity.h"

#include "network_protocol.pb.h"
#include "serdes.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <stdio.h>

#define PACKET_LEN_MAX (32)
uint8_t tx_buffer[PACKET_LEN_MAX];

void test_status_serialization() {
  size_t encoded_data_length;
  uint64_t time = 1;
  float pressure = 2.2;
  float volume = 3.3;
  float flow = 4.4;

  bool status =
      serdes_encode_status_packet(time, pressure, volume, flow, tx_buffer,
                                  PACKET_LEN_MAX, &encoded_data_length);

  TEST_ASSERT_EQUAL_INT16(true, status);
  TEST_ASSERT_EQUAL_INT16(28, encoded_data_length);

  pb_istream_t stream = pb_istream_from_buffer(tx_buffer, encoded_data_length);

  Packet packet = Packet_init_zero;

  status = pb_decode(&stream, Packet_fields, &packet);

  TEST_ASSERT_EQUAL_INT16(true, status);
  TEST_ASSERT_EQUAL_INT16(Packet_data_tag, packet.which_payload);
  TEST_ASSERT_EQUAL_INT16(ControllerMsgType_DATA, packet.payload.data.msg_type);
  TEST_ASSERT_EQUAL_INT16(ControllerData_status_tag,
                          packet.payload.data.which_payload);
  ControllerStatus s = packet.payload.data.payload.status;
  TEST_ASSERT_EQUAL_INT16(s.time, time);
  TEST_ASSERT_EQUAL_INT16(s.pressure, pressure);
  TEST_ASSERT_EQUAL_INT16(s.volume, volume);
  TEST_ASSERT_EQUAL_INT16(s.flow, flow);
  // TEST_ASSERT_EQUAL_INT16(s.alarm_flags, time);
}

bool command_handler_called = false;

void command_handler(Command cmd) {
  TEST_ASSERT_EQUAL_INT16(CommandType_NONE, cmd.cmd);
  TEST_ASSERT_EQUAL_INT16(42.42, cmd.data);
  command_handler_called = true;
}

bool ack_handler_called = false;
void ack_handler(GuiAck ack) { ack_handler_called = true; }

void test_command_deserialization() {
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

  TEST_ASSERT_FALSE(ack_handler_called);
  TEST_ASSERT_FALSE(command_handler_called);

  serdes_decode_incomming_packet(rx_buffer, PACKET_LEN_MAX, encoded_len,
                                 ack_handler, command_handler);

  TEST_ASSERT_FALSE(ack_handler_called);
  TEST_ASSERT_TRUE(command_handler_called);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_status_serialization);
  RUN_TEST(test_command_deserialization);
  return UNITY_END();
}
