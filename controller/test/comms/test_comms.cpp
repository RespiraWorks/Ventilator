#include "comms.h"

#include "hal.h"
#include "network_protocol.pb.h"
#include "gtest/gtest.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

static constexpr int32_t PACKET_LEN_MAX = 32;

class CommTest {
public:
  void SetUp() {
    // Restore default command_handler callback.
    comms_test_set_command_handler(nullptr);
  }
};

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

  static char tx_buffer[PACKET_LEN_MAX];
  auto len = Hal.test_serialGetOutgoingData(tx_buffer, sizeof(tx_buffer));
  ASSERT_GT(len, 0);
  pb_istream_t stream =
      pb_istream_from_buffer(reinterpret_cast<unsigned char *>(tx_buffer), len);

  Packet packet = Packet_init_zero;
  bool status = pb_decode(&stream, Packet_fields, &packet);
  ASSERT_TRUE(status);
  EXPECT_EQ(packet.which_payload, Packet_data_tag);
  EXPECT_EQ(packet.payload.data.msg_type, ControllerMsgType_DATA);
  EXPECT_EQ(packet.payload.data.which_payload, ControllerData_status_tag);
  ControllerStatus s = packet.payload.data.payload.status;
  EXPECT_FLOAT_EQ(s.pressure, pressure);
  EXPECT_FLOAT_EQ(s.volume, volume);
  EXPECT_FLOAT_EQ(s.flow, flow);
}

TEST(CommTests, CommandRx) {
  static bool command_handler_called = false;
  comms_test_set_command_handler(+[](Command &cmd) {
    command_handler_called = true;
    EXPECT_TRUE(cmd.has_data);
    EXPECT_EQ(cmd.cmd, CommandType_NONE);
    EXPECT_FLOAT_EQ(cmd.data, 42.42);
  });

  Command cmd = Command_init_zero;
  cmd.cmd = CommandType_NONE;
  cmd.has_data = true;
  cmd.data = 42.42;

  Packet packet = Packet_init_zero;
  packet.which_payload = Packet_cmd_tag;
  packet.payload.cmd = cmd;

  static char rx_buffer[PACKET_LEN_MAX];
  pb_ostream_t stream = pb_ostream_from_buffer(
      reinterpret_cast<unsigned char *>(rx_buffer), sizeof(rx_buffer));
  pb_encode(&stream, Packet_fields, &packet);
  EXPECT_GT(stream.bytes_written, 0);
  Hal.test_serialPutIncomingData(rx_buffer, stream.bytes_written);
  EXPECT_GT(Hal.serialBytesAvailableForRead(), 0);

  comms_handler();

  // Some time must pass before we'll deem a packet "done".
  // TODO(jlebar): Remove this once we're no longer using timeouts to frame
  // messages.
  Hal.delay(10);
  comms_handler();

  EXPECT_TRUE(command_handler_called);
}
