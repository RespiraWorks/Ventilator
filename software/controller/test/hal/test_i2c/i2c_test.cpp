/* Copyright 2020, RespiraWorks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "i2c.h"

#include "gtest_main.h"

using namespace I2C;

class TestChannel : public Channel {
 public:
  TestChannel() : Channel(Base::I2C1){};
  // in test mode, setters and getters for faked sent/received data
  std::optional<uint8_t> TESTGetSentData() { return sent_buffer_.Get(); };
  bool TESTQueueReceiveData(uint8_t data) { return rx_buffer_.Put(data); };
  // setter to simulate Nack received
  void TESTSimulateNack() { nack_ = true; };

 private:
  // fake sending and receiving data through circular buffers.
  CircularBuffer<uint8_t, WriteBufferSize> sent_buffer_;
  // note it is up to the tester to put data in the rx_buffer before
  // calling I2CEventHandler during a read request
  CircularBuffer<uint8_t, WriteBufferSize> rx_buffer_;
  // fake a NACK condition on next handler call
  bool nack_{false};

  // mock the sending and receiving of bytes from internal buffers
  void SendByte() override {
    bool ok = sent_buffer_.Put(*next_data_);
    if (ok) return;
  };
  void ReceiveByte() override {
    std::optional<uint8_t> data = rx_buffer_.Get();
    if (data != std::nullopt) {
      *next_data_ = *data;
    }
  };

  // Override functions that write to registers
  void SetupI2CTransfer() override{};
  void StopTransfer() override{};
  void ClearNack() override { nack_ = false; };
  void WriteTransferSize() override{};
  void ClearErrors() override{};

  // Override I²C interrupt getters for Mock
  bool NextByteNeeded() const override { return remaining_size_ > 0; };
  bool TransferReload() const override {
    return remaining_size_ % 255 == 0 && remaining_size_ > 0;
  };
  bool TransferComplete() const override { return remaining_size_ == 0; };
  bool NackDetected() const override { return nack_; };
};

TEST(I2C, RequestQueue) {
  constexpr uint32_t QueueLength{I2C::Channel::QueueLength};

  TestChannel i2c;

  // Queue QueueLength+5 read requests
  uint8_t read_data[QueueLength + 5];
  bool processed[QueueLength + 5];
  for (int req = 0; req < QueueLength + 5; ++req) {
    Request read_byte{
        .slave_address = 0,
        .direction = ExchangeDirection::Read,
        .size = 1,
        .data = &read_data[req],
        .processed = &processed[req],
    };
    // Note that the first request is only queued temporarily: because there is
    // no transfer in progress yet, it is dequeued immediately by a call to
    // StartTransfer in SendRequest.
    ASSERT_EQ(i2c.SendRequest(read_byte), req <= QueueLength);
    ASSERT_FALSE(processed[req]);
  }

  // Process the first request and check that this is done properly
  i2c.TESTQueueReceiveData(10);
  i2c.I2CEventHandler();
  ASSERT_TRUE(processed[0]);
  ASSERT_FALSE(processed[1]);
  ASSERT_EQ(read_data[0], 10);

  // Attempt at queueing one new request should succeed, not the following ones
  for (int req = QueueLength + 1; req < QueueLength + 5; ++req) {
    Request read_byte{
        .slave_address = 0,
        .direction = ExchangeDirection::Read,
        .size = 1,
        .data = &read_data[req],
        .processed = &processed[req],
    };
    ASSERT_EQ(i2c.SendRequest(read_byte), req <= QueueLength + 1);
  }

  // Process all requests in the queue and check they are processed in order
  for (int req = 1; req < QueueLength + 2; ++req) {
    i2c.TESTQueueReceiveData(static_cast<uint8_t>(req + 10));
    i2c.I2CEventHandler();
    ASSERT_TRUE(processed[req]);
    ASSERT_FALSE(processed[req + 1]);
    ASSERT_EQ(read_data[req], req + 10);
  }
}

TEST(I2C, WriteBuffer) {
  constexpr uint16_t WriteBufferLength{4096};
  constexpr uint16_t RequestLength{400};
  uint buffer_head{0};
  TestChannel i2c;

  // Initialize a long array to be sent over I2C
  uint8_t test_array[RequestLength + 50];
  for (int i = 0; i < RequestLength + 50; ++i) {
    test_array[i] = static_cast<uint8_t>(i % 256);
  }

  constexpr int NumReq{WriteBufferLength / RequestLength};
  bool processed[NumReq + 5];

  // Fill the buffer with requests of 400 bytes each
  for (int req = 0; req < NumReq; ++req) {
    Request write_array{
        .slave_address = 0,
        .direction = ExchangeDirection::Write,
        .size = RequestLength,
        .data = &test_array[req],
        .processed = &processed[req],
    };
    ASSERT_TRUE(i2c.SendRequest(write_array));
    buffer_head = buffer_head + write_array.size;
  }
  // Check that a request of "remaining + 1" bytes request is rejected
  Request write_array{
      .slave_address = 0,
      .direction = ExchangeDirection::Write,
      .size = static_cast<uint16_t>(WriteBufferLength - buffer_head + 1),
      .data = &test_array[NumReq],
      .processed = &processed[NumReq],
  };
  ASSERT_FALSE(i2c.SendRequest(write_array));

  // Process the first bytes of the first request, we should still reject
  // previous request (freeing is only done when a request is processed)
  for (int byte = 0; byte < WriteBufferLength - buffer_head + 1; ++byte) {
    i2c.I2CEventHandler();
    std::optional<uint8_t> data = i2c.TESTGetSentData();
    ASSERT_TRUE(data != std::nullopt);
    ASSERT_EQ(data, byte % 256);
  }
  ASSERT_FALSE(i2c.SendRequest(write_array));

  // Free up to 256 bytes (one transfer, but not the full request), repeat
  for (int byte = WriteBufferLength - buffer_head + 1; byte < 256; ++byte) {
    i2c.I2CEventHandler();
    std::optional<uint8_t> data = i2c.TESTGetSentData();
    ASSERT_TRUE(data != std::nullopt);
    ASSERT_EQ(data, byte % 256);
  }
  ASSERT_FALSE(i2c.SendRequest(write_array));

  // Finish processing the first request and this time it should work
  for (int byte = 256; byte < RequestLength; ++byte) {
    i2c.I2CEventHandler();
    std::optional<uint8_t> data = i2c.TESTGetSentData();
    ASSERT_TRUE(data != std::nullopt);
    ASSERT_EQ(data, byte % 256);
  }

  ASSERT_TRUE(processed[0]);
  ASSERT_FALSE(processed[1]);

  ASSERT_TRUE(i2c.SendRequest(write_array));
  // we wrapped around the buffer
  buffer_head = write_array.size;

  // process all of the initial requests (and check that it is done right)
  for (int req = 1; req < NumReq; ++req) {
    for (int byte = 0; byte < RequestLength; ++byte) {
      i2c.I2CEventHandler();
      std::optional<uint8_t> data = i2c.TESTGetSentData();
      ASSERT_TRUE(data != std::nullopt);
      ASSERT_EQ(data, (byte + req) % 256);
    }
    ASSERT_TRUE(processed[req]);
    ASSERT_FALSE(processed[req + 1]);
  }

  // queue another request (we should then have 2 requests queued)
  write_array.data = &test_array[NumReq + 1];
  write_array.processed = &processed[NumReq + 1];
  ASSERT_TRUE(i2c.SendRequest(write_array));
  buffer_head = buffer_head + write_array.size;

  // process the first -> frees the first write_array.size bytes of the buffer
  for (int byte = 0; byte < write_array.size; ++byte) {
    i2c.I2CEventHandler();
    std::optional<uint8_t> data = i2c.TESTGetSentData();
    ASSERT_TRUE(data != std::nullopt);
    ASSERT_EQ(data, (byte + NumReq) % 256);
  }
  ASSERT_TRUE(processed[NumReq]);
  ASSERT_FALSE(processed[NumReq + 1]);

  uint buffer_tail{write_array.size};

  // Check that we still can't send a request that would wrap around the buffer
  uint8_t data[WriteBufferLength - buffer_head + 1];
  write_array.size = static_cast<uint16_t>(WriteBufferLength - buffer_head + 1);
  write_array.data = &data;
  ASSERT_FALSE(i2c.SendRequest(write_array));

  // But we can send a request that fills the whole end of the buffer (ranging
  // from buffer_head to WriteBufferLength - 1)
  write_array.size = static_cast<uint16_t>(WriteBufferLength - buffer_head);
  ASSERT_TRUE(i2c.SendRequest(write_array));

  // now check that the first buffer_tail -1 bytes are also usable
  // though not 1 more...
  write_array.size = static_cast<uint16_t>(buffer_tail);
  ASSERT_FALSE(i2c.SendRequest(write_array));
  write_array.size = static_cast<uint16_t>(buffer_tail - 1);
  ASSERT_TRUE(i2c.SendRequest(write_array));
}

TEST(I2C, RetryOnError) {
  constexpr uint MaxRetries{5};
  constexpr uint16_t RequestLength{400};

  TestChannel i2c;

  // Queue a long read request
  uint8_t read_data[RequestLength];
  bool processed{false};
  Request long_read{
      .slave_address = 0,
      .direction = ExchangeDirection::Read,
      .size = RequestLength,
      .data = &read_data[0],
      .processed = &processed,
  };
  i2c.SendRequest(long_read);

  uint8_t read2{0};
  bool processed2{false};
  Request read_byte{
      .slave_address = 0,
      .direction = ExchangeDirection::Read,
      .size = 1,
      .data = &read2,
      .processed = &processed2,
  };
  i2c.SendRequest(read_byte);

  // Process the first transfer (255 bytes) + a few bytes of the next one
  for (int byte = 0; byte < 265; ++byte) {
    i2c.TESTQueueReceiveData(static_cast<uint8_t>(byte % 256));
    i2c.I2CEventHandler();
    ASSERT_EQ(read_data[byte], byte % 256);
  }

  // Simulate a bus error and check the request is restarted from its first byte
  i2c.I2CErrorHandler();
  i2c.TESTQueueReceiveData(static_cast<uint8_t>(45));
  i2c.I2CEventHandler();
  ASSERT_EQ(read_data[0], 45);

  // Simulate another kMaxRetries - 1 bus errors to the same effect
  for (int error = 1; error < MaxRetries - 1; ++error) {
    i2c.I2CErrorHandler();
    i2c.TESTQueueReceiveData(static_cast<uint8_t>(error + 45));
    i2c.I2CEventHandler();
    ASSERT_EQ(read_data[0], error + 45);
  }

  // Simulate a NACK, and check this has the same effect except it doesn't check
  // the retry count
  i2c.TESTSimulateNack();
  i2c.I2CEventHandler();
  i2c.I2CEventHandler();
  ASSERT_EQ(read_data[0], MaxRetries - 2 + 45);
  // This is where I get stuck... the I2C design is currently not robust passed
  // this point... if I simulate another error, it will just start the next
  // request, which is not really satisfactory.
  i2c.I2CErrorHandler();
  i2c.TESTQueueReceiveData(20);
  i2c.I2CEventHandler();
  ASSERT_EQ(read_data[0], MaxRetries - 2 + 45);
  ASSERT_FALSE(processed);
  ASSERT_EQ(read2, 20);
  ASSERT_TRUE(processed2);
}
