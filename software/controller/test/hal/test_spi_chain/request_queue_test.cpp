/* Copyright 2022, RespiraWorks

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

#include "gtest/gtest.h"
#include "spi_chain.h"

using namespace SPI;

// This class adds public accessors to RequestQueue for testing purposes.
template <size_t Q>
class ExposedRequestQueue : public RequestQueue<Q> {
 public:
  ExposedRequestQueue() = default;

  // getters
  bool end_of_request() { return this->end_of_request_; }
  bool save_response() { return this->save_response_; }
  size_t response_count() { return this->response_count_; }
  size_t queue_count() { return this->queue_count_; }
  size_t command_index() { return this->command_index_; }

  // setters
  void save_response(bool b) { this->save_response_ = b; }
  void end_of_request(bool b) { this->end_of_request_ = b; }
};

TEST(RequestQueue, ConstructorCreatesEmptyQueue) {
  ExposedRequestQueue<1> queue;

  EXPECT_EQ(queue.queue_count(), 0);
  EXPECT_EQ(queue.response_count(), 0);
  EXPECT_EQ(queue.command_index(), 0);
  EXPECT_FALSE(queue.end_of_request());
  EXPECT_FALSE(queue.save_response());

  EXPECT_FALSE(queue.GetNextCommandByte().has_value());
  EXPECT_FALSE(queue.WriteNextResponseByte(1));
  EXPECT_FALSE(queue.GetCurrentRequest());
}

TEST(RequestQueue, QueueingOnlyChangesCommandPointer) {
  ExposedRequestQueue<2> queue;

  uint8_t command{1};
  // deliberately put a different value in command_buffer
  uint8_t command_buffer[1] = {4};
  uint8_t response{0};
  bool processed{false};
  Request request{
      .command{&command}, .length{sizeof(command)}, .response{&response}, .processed{&processed}};

  EXPECT_TRUE(queue.AddRequest(request, command_buffer));
  EXPECT_EQ(queue.queue_count(), 1);

  EXPECT_EQ(queue.GetCurrentRequest()->length, request.length);
  EXPECT_EQ(queue.GetCurrentRequest()->response, request.response);
  EXPECT_EQ(queue.GetCurrentRequest()->processed, request.processed);
  EXPECT_EQ(queue.GetCurrentRequest()->command[0], command_buffer[0]);
}

TEST(RequestQueue, GetSecondCommandByteTriggersSaveResponse) {
  ExposedRequestQueue<2> queue;

  uint8_t command[4] = {1, 2, 3, 4};
  Request request{.command{command}, .length{sizeof(command)}};

  queue.AddRequest(request, command);

  queue.GetNextCommandByte().value();
  EXPECT_FALSE(queue.save_response());
  queue.GetNextCommandByte().value();
  EXPECT_TRUE(queue.save_response());
}

TEST(RequestQueue, GetFullCommandTriggersEndOfRequest) {
  ExposedRequestQueue<2> queue;

  uint8_t command[4] = {1, 2, 3, 4};
  Request request{.command{command}, .length{sizeof(command)}};

  queue.AddRequest(request, command);

  // testing that GetNextCommandByte returns the proper value while we are at it.
  for (size_t i = 0; i < request.length; i++) {
    EXPECT_EQ(queue.GetCurrentRequest()->command[i], queue.GetNextCommandByte().value());
  }
  EXPECT_TRUE(queue.end_of_request());
}

TEST(RequestQueue, WriteResponseByteSavesOnlyWhenAsked) {
  ExposedRequestQueue<1> queue;

  uint8_t command{0};
  uint8_t response{0};
  Request request{.command{&command}, .length{sizeof(command)}, &response};

  queue.AddRequest(request, &command);

  // save_response was initialized at false, therefore we don't write anything
  EXPECT_FALSE(queue.WriteNextResponseByte(42));
  EXPECT_EQ(response, 0);
  EXPECT_EQ(queue.response_count(), 0);

  // set save_response, but force request.response=nullptr, we still don't write anything
  queue.save_response(true);
  queue.GetCurrentRequest()->response = nullptr;
  EXPECT_FALSE(queue.WriteNextResponseByte(42));
  EXPECT_EQ(queue.response_count(), 0);

  // revert back to a valid pointer
  queue.GetCurrentRequest()->response = &response;
  EXPECT_TRUE(queue.WriteNextResponseByte(42));
  EXPECT_EQ(response, 42);
  EXPECT_EQ(queue.response_count(), 1);
}

TEST(RequestQueue, WriteResponseByteAtEndOfRequestAdvancesQueue) {
  ExposedRequestQueue<2> queue;

  uint8_t commands[2]{1, 2};
  bool processed[2] = {false};
  Request request{.command{&commands[0]},
                  .length{sizeof(commands[0])},
                  .response{nullptr},
                  .processed{&processed[0]}};

  queue.AddRequest(request, &commands[0]);
  request.command = &commands[1];
  request.processed = &processed[1];
  queue.AddRequest(request, &commands[1]);

  // advance command index
  queue.GetNextCommandByte();
  EXPECT_EQ(queue.command_index(), 1);

  queue.save_response(true);
  queue.end_of_request(true);
  queue.WriteNextResponseByte(0);

  EXPECT_EQ(queue.GetCurrentRequest()->command[0], commands[1]);
  EXPECT_FALSE(queue.end_of_request());
  EXPECT_FALSE(queue.save_response());
  EXPECT_EQ(queue.response_count(), 0);
  EXPECT_EQ(queue.command_index(), 0);
  EXPECT_EQ(queue.queue_count(), 2);
  EXPECT_TRUE(processed[0]);
}

TEST(RequestQueue, WriteResponseByteAtEndOfQueueResetsQueue) {
  ExposedRequestQueue<1> queue;

  uint8_t command[2]{1, 2};
  uint8_t response{0};
  bool processed{false};
  Request request{
      .command{command}, .length{sizeof(command)}, .response{&response}, .processed{&processed}};

  queue.AddRequest(request, command);
  // advance command index
  queue.GetNextCommandByte();
  EXPECT_EQ(queue.command_index(), 1);

  // advance response_count
  queue.save_response(true);
  queue.WriteNextResponseByte(0);
  EXPECT_EQ(queue.response_count(), 1);

  // trigger end of request
  queue.end_of_request(true);
  queue.WriteNextResponseByte(0);
  EXPECT_FALSE(queue.GetCurrentRequest());
  EXPECT_FALSE(queue.end_of_request());
  EXPECT_FALSE(queue.save_response());
  EXPECT_EQ(queue.response_count(), 0);
  EXPECT_EQ(queue.command_index(), 0);
  EXPECT_EQ(queue.queue_count(), 0);
  EXPECT_TRUE(processed);
}

TEST(RequestQueue, CannotQueueInvalidRequests) {
  RequestQueue<1> queue;
  // Cannot queue a request with nullptr command
  uint8_t command{0};
  Request request{.command{nullptr}, .length{sizeof(command)}};
  EXPECT_FALSE(queue.AddRequest(request, &command));

  // Cannot queue a request with length 0
  request.command = &command;
  request.length = 0;
  EXPECT_FALSE(queue.AddRequest(request, &command));
}

TEST(RequestQueue, CannotQueueWhenFull) {
  RequestQueue<1> queue;

  uint8_t command{0};
  Request request{.command{&command}, .length{sizeof(command)}};

  EXPECT_TRUE(queue.AddRequest(request, &command));
  // Queue is now full, we cannot send any more stuff
  EXPECT_FALSE(queue.AddRequest(request, &command));
}

TEST(RequestQueue, CannotQueueWithNullBuffer) {
  RequestQueue<1> queue;

  uint8_t command{0};
  Request request{.command{&command}, .length{sizeof(command)}};

  EXPECT_FALSE(queue.AddRequest(request, nullptr));
}
