#include "unity.h"

#include "comms.h"

#include "network_protocol.pb.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

static uint64_t time = 0x4242;

#define PACKET_LEN_MAX (32)
static uint8_t tx_buffer[PACKET_LEN_MAX];
static uint8_t tx_idx = 0;
static uint8_t rx_buffer[PACKET_LEN_MAX];
static uint8_t rx_idx = 0;
static uint8_t rx_data_len = 0;
uint64_t millis() { return time++; }

void serialIO_readByte(char *buffer) {
  if (rx_idx >= PACKET_LEN_MAX || rx_idx > rx_data_len) {
    TEST_FAIL_MESSAGE("rx buffer underflow");
  }
  *buffer++ = rx_buffer[rx_idx++];
}

void serialIO_init() {
  tx_idx = 0;
  rx_idx = 0;
}

bool serialIO_dataAvailable() { return rx_idx < rx_data_len; }

void serialIO_send(uint8_t b) {
  tx_buffer[tx_idx++] = b;
  if (tx_idx >= PACKET_LEN_MAX) {
    TEST_FAIL_MESSAGE("tx buffer overflow");
  }
}

void test_sendPeriodicReadings() {
  float pressure = 1.1;
  float volume = 2.2;
  float flow = 3.3;
  tx_idx = 0;

  comms_sendPeriodicReadings(pressure, volume, flow);
  comms_handler();

  TEST_ASSERT_TRUE(tx_idx != 0);
  pb_istream_t stream = pb_istream_from_buffer(tx_buffer, tx_idx);
  Packet packet = Packet_init_zero;
  bool status = pb_decode(&stream, Packet_fields, &packet);
  TEST_ASSERT_EQUAL_INT16(true, status);
  TEST_ASSERT_EQUAL_INT16(Packet_data_tag, packet.which_payload);
  TEST_ASSERT_EQUAL_INT16(ControllerMsgType_DATA, packet.payload.data.msg_type);
  TEST_ASSERT_EQUAL_INT16(ControllerData_status_tag,
                          packet.payload.data.which_payload);
  ControllerStatus s = packet.payload.data.payload.status;
  TEST_ASSERT_EQUAL_FLOAT(s.pressure, pressure);
  TEST_ASSERT_EQUAL_FLOAT(s.volume, volume);
  TEST_ASSERT_EQUAL_FLOAT(s.flow, flow);
}

bool command_handler_called = false;
void command_handler(Command cmd) {
  command_handler_called = true;
  TEST_ASSERT_TRUE(cmd.has_data);
  TEST_ASSERT_EQUAL_INT32(CommandType_NONE, cmd.cmd);
  TEST_ASSERT_EQUAL_FLOAT(42.42, cmd.data);
}

void ack_handler(GuiAck ack) {}

void test_command_rx() {
  Command cmd = Command_init_zero;
  cmd.cmd = CommandType_NONE;
  cmd.has_data = true;
  cmd.data = 42.42;

  Packet packet = Packet_init_zero;
  packet.which_payload = Packet_cmd_tag;
  packet.payload.cmd = cmd;

  pb_ostream_t stream = pb_ostream_from_buffer(rx_buffer, PACKET_LEN_MAX);
  pb_encode(&stream, Packet_fields, &packet);
  rx_data_len = stream.bytes_written;
  rx_idx = 0;

  TEST_ASSERT_TRUE(serialIO_dataAvailable());

  comms_handler();
  comms_handler();

  TEST_ASSERT_TRUE(command_handler_called);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_sendPeriodicReadings);
  RUN_TEST(test_command_rx);
  return UNITY_END();
}
