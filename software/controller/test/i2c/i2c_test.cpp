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
#include "gtest/gtest.h"

TEST(I2C, RequestQueue) {
  constexpr uint kQueueLength{80};

  I2CChannel i2c;

  // Queue kQueueLength+5 read requests
  uint8_t read_data[kQueueLength + 5];
  bool processed[kQueueLength + 5];
  for (int req = 0; req < kQueueLength + 5; ++req) {
    I2CRequest read_byte{
        .slave_address = 0,
        .read_write = I2CExchangeDir::kRead,
        .size = 1,
        .data = &read_data[req],
        .processed = &processed[req],
    };
    // Note that the first request is only queued temporarily: because there is
    // no transfer in progress yet, it is dequeued immediately by a call to
    // StartTransfer in SendRequest.
    ASSERT_EQ(i2c.SendRequest(read_byte), req <= kQueueLength);
    ASSERT_FALSE(processed[req]);
  }

  // Process the first request and check that this is done properly
  i2c.TEST_QueueReceiveData(10);
  i2c.I2CEventHandler();
  ASSERT_TRUE(processed[0]);
  ASSERT_FALSE(processed[1]);
  ASSERT_EQ(read_data[0], 10);

  // Attempt at queueing one new request should succeed, not the following ones
  for (int req = kQueueLength + 1; req < kQueueLength + 5; ++req) {
    I2CRequest read_byte{
        .slave_address = 0,
        .read_write = I2CExchangeDir::kRead,
        .size = 1,
        .data = &read_data[req],
        .processed = &processed[req],
    };
    ASSERT_EQ(i2c.SendRequest(read_byte), req <= kQueueLength + 1);
  }

  // Process all requests in the queue and check they are processed in order
  for (int req = 1; req < kQueueLength + 2; ++req) {
    i2c.TEST_QueueReceiveData(static_cast<uint8_t>(req + 10));
    i2c.I2CEventHandler();
    ASSERT_TRUE(processed[req]);
    ASSERT_FALSE(processed[req + 1]);
    ASSERT_EQ(read_data[req], req + 10);
  }
}

TEST(I2C, WriteBuffer) {
  constexpr uint16_t kWriteBufferLength{4096};
  constexpr uint16_t kRequestLength{400};
  uint buffer_head{0};
  I2CChannel i2c;

  // Initialize a long array to be sent over I2C
  uint8_t test_array[kRequestLength + 50];
  for (int i = 0; i < kRequestLength + 50; ++i) {
    test_array[i] = static_cast<uint8_t>(i % 256);
  }

  constexpr int kNumReq{kWriteBufferLength / kRequestLength};
  bool processed[kNumReq + 5];

  // Fill the buffer with requests of 400 bytes each
  for (int req = 0; req < kNumReq; ++req) {
    I2CRequest write_array{
        .slave_address = 0,
        .read_write = I2CExchangeDir::kWrite,
        .size = kRequestLength,
        .data = &test_array[req],
        .processed = &processed[req],
    };
    ASSERT_TRUE(i2c.SendRequest(write_array));
    buffer_head = buffer_head + write_array.size;
  }
  // Check that a request of "remaining + 1" bytes request is rejected
  I2CRequest write_array{
      .slave_address = 0,
      .read_write = I2CExchangeDir::kWrite,
      .size = static_cast<uint16_t>(kWriteBufferLength - buffer_head + 1),
      .data = &test_array[kNumReq],
      .processed = &processed[kNumReq],
  };
  ASSERT_FALSE(i2c.SendRequest(write_array));

  // Process the first bytes of the first request, we should still reject
  // previous request (freeing is only done when a request is processed)
  for (int byte = 0; byte < kWriteBufferLength - buffer_head + 1; ++byte) {
    i2c.I2CEventHandler();
    std::optional<uint8_t> data = i2c.TEST_GetSentData();
    if (data != std::nullopt) {
      ASSERT_EQ(data, byte % 256);
    } else {
      ADD_FAILURE();
    }
  }
  ASSERT_FALSE(i2c.SendRequest(write_array));

  // Free up to 256 bytes (one transfer, but not the full request), repeat
  for (int byte = kWriteBufferLength - buffer_head + 1; byte < 256; ++byte) {
    i2c.I2CEventHandler();
    std::optional<uint8_t> data = i2c.TEST_GetSentData();
    if (data != std::nullopt) {
      ASSERT_EQ(data, byte % 256);
    } else {
      ADD_FAILURE();
    }
  }
  ASSERT_FALSE(i2c.SendRequest(write_array));

  // Finish processing the first request and this time it should work
  for (int byte = 256; byte < kRequestLength; ++byte) {
    i2c.I2CEventHandler();
    std::optional<uint8_t> data = i2c.TEST_GetSentData();
    if (data != std::nullopt) {
      ASSERT_EQ(data, byte % 256);
    } else {
      ADD_FAILURE();
    }
  }

  ASSERT_TRUE(processed[0]);
  ASSERT_FALSE(processed[1]);

  ASSERT_TRUE(i2c.SendRequest(write_array));
  // we wrapped around the buffer
  buffer_head = write_array.size;

  // process all of the initial requests (and check that it is done right)
  for (int req = 1; req < kNumReq; ++req) {
    for (int byte = 0; byte < kRequestLength; ++byte) {
      i2c.I2CEventHandler();
      std::optional<uint8_t> data = i2c.TEST_GetSentData();
      if (data != std::nullopt) {
        ASSERT_EQ(data, (byte + req) % 256);
      } else {
        ADD_FAILURE();
      }
    }
    ASSERT_TRUE(processed[req]);
    ASSERT_FALSE(processed[req + 1]);
  }

  // queue another request (we should then have 2 requests queued)
  write_array.data = &test_array[kNumReq + 1];
  write_array.processed = &processed[kNumReq + 1];
  ASSERT_TRUE(i2c.SendRequest(write_array));
  buffer_head = buffer_head + write_array.size;

  // process the first -> frees the first write_array.size bytes of the buffer
  for (int byte = 0; byte < write_array.size; ++byte) {
    i2c.I2CEventHandler();
    std::optional<uint8_t> data = i2c.TEST_GetSentData();
    if (data != std::nullopt) {
      ASSERT_EQ(data, (byte + kNumReq) % 256);
    } else {
      ADD_FAILURE();
    }
  }
  ASSERT_TRUE(processed[kNumReq]);
  ASSERT_FALSE(processed[kNumReq + 1]);

  uint buffer_tail{write_array.size};

  // Check that we still can't send a request that would wrap around the buffer
  uint8_t data[kWriteBufferLength - buffer_head + 1];
  write_array.size =
      static_cast<uint16_t>(kWriteBufferLength - buffer_head + 1);
  write_array.data = &data;
  ASSERT_FALSE(i2c.SendRequest(write_array));

  // But we can send a request that fills the whole end of the buffer (ranging
  // from buffer_head to kWriteBufferLength - 1)
  write_array.size = static_cast<uint16_t>(kWriteBufferLength - buffer_head);
  ASSERT_TRUE(i2c.SendRequest(write_array));

  // now check that the first buffer_tail -1 bytes are also usable
  // though not 1 more...
  write_array.size = static_cast<uint16_t>(buffer_tail);
  ASSERT_FALSE(i2c.SendRequest(write_array));
  write_array.size = static_cast<uint16_t>(buffer_tail - 1);
  ASSERT_TRUE(i2c.SendRequest(write_array));
}

TEST(I2C, RetryOnError) {
  constexpr uint kMaxRetries{5};
  constexpr uint16_t kRequestLength{400};

  I2CChannel i2c;

  // Queue a long read request
  uint8_t read_data[kRequestLength];
  bool processed{false};
  I2CRequest read_byte{
      .slave_address = 0,
      .read_write = I2CExchangeDir::kRead,
      .size = kRequestLength,
      .data = &read_data[0],
      .processed = &processed,
  };
  i2c.SendRequest(read_byte);

  // Process the first transfer (255 bytes) + a few bytes of the next one
  for (int byte = 0; byte < 265; ++byte) {
    i2c.TEST_QueueReceiveData(static_cast<uint8_t>(byte % 256));
    i2c.I2CEventHandler();
    ASSERT_EQ(read_data[byte], byte % 256);
  }

  // Simulate a bus error and check the request is restarted from its first byte
  i2c.I2CErrorHandler();
  i2c.TEST_QueueReceiveData(static_cast<uint8_t>(45));
  i2c.I2CEventHandler();
  ASSERT_EQ(read_data[0], 45);

  // Simulate another kMaxRetries - 1 bus errors to the same effect
  for (int error = 1; error < kMaxRetries - 1; ++error) {
    i2c.I2CErrorHandler();
    i2c.TEST_QueueReceiveData(static_cast<uint8_t>(error + 45));
    i2c.I2CEventHandler();
    ASSERT_EQ(read_data[0], error + 45);
  }

  // Simulate a NACK, and check this has the same effect except it doesn't check
  // the retry count
  i2c.TEST_QueueReceiveData(10);
  i2c.TEST_SimulateNack();
  i2c.I2CEventHandler();
  i2c.TEST_QueueReceiveData(20);
  i2c.I2CEventHandler();
  ASSERT_EQ(read_data[0], 20);
  // This is where I get stuck... the I2C design is currently not robust passed
  // this point... if I simulate another error, it will just carry-on with the
  // request, which is incorrect outside of test mode.
  i2c.I2CErrorHandler();
  i2c.TEST_QueueReceiveData(40);
  i2c.I2CEventHandler();
  ASSERT_EQ(read_data[0], 20);
  ASSERT_EQ(read_data[1], 40);
}
