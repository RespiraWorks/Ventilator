#include "gtest/gtest.h"

#include "comms.h"

#include "hal.h"
#include "network_protocol.pb.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

#define PACKET_LEN_MAX 64

TEST(CommTests, SendControllerStatus) {
  float pressure = 1.1;
  float volume = 2.2;
  float flow = 3.3;

  ControllerStatus controller_status = ControllerStatus_init_zero;
  controller_status.time = 100;
  controller_status.flow = flow;
  controller_status.volume = volume;
  controller_status.pressure = pressure;

  comms_sendControllerStatus(controller_status);
  comms_handler();

  uint32_t len = Hal.test_serialGetOutgoingDataSize();
  ASSERT_GE(len, (uint32_t)0);
  uint8_t tx_buffer[PACKET_LEN_MAX];
  Hal.test_serialGetOutgoingData((char *)tx_buffer, len);
  pb_istream_t stream = pb_istream_from_buffer(tx_buffer, len);

  Packet packet = Packet_init_zero;
  bool status = pb_decode(&stream, Packet_fields, &packet);
  ASSERT_TRUE(status);
  ASSERT_EQ(packet.which_payload, Packet_data_tag);
  ASSERT_EQ(packet.payload.data.msg_type, ControllerMsgType_DATA);
  ASSERT_EQ(packet.payload.data.which_payload, ControllerData_status_tag);
  ControllerStatus s = packet.payload.data.payload.status;
  ASSERT_FLOAT_EQ(s.pressure, pressure);
  ASSERT_FLOAT_EQ(s.volume, volume);
  ASSERT_FLOAT_EQ(s.flow, flow);
}

bool command_handler_called = false;

void command_handler(Command &cmd) {
  command_handler_called = true;
  ASSERT_TRUE(cmd.has_data);
  ASSERT_EQ(cmd.cmd, CommandType_NONE);
  ASSERT_FLOAT_EQ(cmd.data, 42.42);
}

void gui_ack_handler(const GuiAck &ack) {}

TEST(CommTests, CommandRx) {
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
  uint32_t rx_data_len = stream.bytes_written;
  Hal.test_serialPutIncomingData((char *)rx_buffer, rx_data_len);

  ASSERT_TRUE(Hal.serialBytesAvailableForRead() > 0);

  comms_handler();
  Hal.delay(2);
  comms_handler();

  ASSERT_TRUE(command_handler_called);
}
