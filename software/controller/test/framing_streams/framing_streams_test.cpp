/* Copyright 2020-2021, RespiraWorks

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

#include "framing_streams.h"

#include "gtest/gtest.h"
#include "uart_dma_stream.h"

class ObserverStream : public OutputStream {
  static constexpr uint32_t LEN = 20;
  uint32_t i = 0;
  int32_t saved[LEN];

 public:
  StreamResponse put(int32_t b) override {
    if (EndOfStream == b) {
      return {};
    }

    if (i < LEN) {
      saved[i++] = b;
      return StreamResponse(1, ResponseFlags::StreamSuccess);
    } else {
      return StreamResponse(0, ResponseFlags::ErrorBufferFull);
    }
  }
  // Returns number of bytes of encoded frame after processing the given byte.
  uint32_t encoded_length(int32_t b) { return EndOfStream == b ? 0 : 1; }
  // Returns number of bytes that can be written.
  uint32_t bytes_available_for_write() override { return LEN - i; }
  int32_t *get_saved() { return saved; }
  uint32_t get_index() { return i; }
};

TEST(StreamResponse, StreamResponseSum) {
  StreamResponse r1 = StreamResponse(42, ResponseFlags::StreamSuccess);
  StreamResponse r2 = StreamResponse(52, ResponseFlags::StreamSuccess);
  r2 += r1;
  ASSERT_EQ(r2.count_written, (uint32_t)(42 + 52));
}

TEST(FramingStreamsTests, CrcStreamTrivial) {
  ObserverStream observer;
  CrcStream crc_stream(observer);

  crc_stream.put(0);
  ASSERT_EQ(observer.get_index(), (uint32_t)1);
  ASSERT_EQ(crc_stream.put(EndOfStream).count_written, (uint32_t)4);
  ASSERT_EQ(observer.get_index(), (uint32_t)5);
}

TEST(FramingStreamsTests, EscapeStreamTrivial) {
  ObserverStream observer;
  EscapeStream escape_stream(observer);
  EXPECT_EQ(escape_stream.put(EndOfStream).count_written, (uint32_t)2);
  EXPECT_EQ(escape_stream.put(0).count_written, (uint32_t)2);
  EXPECT_EQ(escape_stream.put(0).count_written, (uint32_t)1);
  EXPECT_EQ(escape_stream.put(FramingMark).count_written, (uint32_t)2);
  EXPECT_EQ(escape_stream.put(FramingEscape).count_written, (uint32_t)2);
  EXPECT_EQ(escape_stream.put(EndOfStream).count_written, (uint32_t)1);
}

uint8_t tx_buffer[20];
uint32_t tx_length = 0;
bool is_txing = false;
TxListener *tx_listener;

bool UartDma::tx_in_progress() const { return is_txing; }
bool UartDma::start_tx(uint8_t *buf, uint32_t length, TxListener *txl) {
  printf("TX event, length: %d\n ", length);
  for (uint32_t i = 0; i < length; i++) {
    printf("%X ", buf[i]);
  }
  printf("\n");
  memcpy(tx_buffer, buf, length);
  tx_length = length;
  is_txing = true;
  tx_listener = txl;
  return true;
};

void UartDma::DMA_tx_interrupt_handler() {
  is_txing = false;
  tx_listener->on_tx_complete();
}

UartDma uart_dma;

TEST(FramingStreamsTests, DmaStreamTrivial) {
  DmaStream<2> dma_stream;
  StreamResponse r;
  EXPECT_EQ(dma_stream.bytes_available_for_write(), (uint32_t)4);

  r = dma_stream.put(0);
  EXPECT_EQ(r.count_written, (uint32_t)1);
  EXPECT_EQ(r.flags, static_cast<uint32_t>(ResponseFlags::StreamSuccess));
  EXPECT_EQ(dma_stream.bytes_available_for_write(), (uint32_t)3);
  EXPECT_FALSE(is_txing);

  r = dma_stream.put(0);
  EXPECT_EQ(r.count_written, (uint32_t)1);
  EXPECT_EQ(r.flags, static_cast<uint32_t>(ResponseFlags::StreamSuccess));
  EXPECT_EQ(dma_stream.bytes_available_for_write(), (uint32_t)2);
  EXPECT_TRUE(is_txing);

  r = dma_stream.put(0);
  EXPECT_EQ(r.count_written, (uint32_t)1);
  EXPECT_EQ(r.flags, static_cast<uint32_t>(ResponseFlags::StreamSuccess));
  EXPECT_EQ(dma_stream.bytes_available_for_write(), (uint32_t)1);

  r = dma_stream.put(0);
  EXPECT_EQ(r.count_written, (uint32_t)1);
  EXPECT_EQ(r.flags, static_cast<uint32_t>(ResponseFlags::WarningBufferFull));
  EXPECT_EQ(dma_stream.bytes_available_for_write(), (uint32_t)0);

  r = dma_stream.put(0);
  EXPECT_EQ(r.count_written, (uint32_t)0);
  EXPECT_EQ(r.flags, static_cast<uint32_t>(ResponseFlags::ErrorBufferFull));

  uart_dma.DMA_tx_interrupt_handler();
  EXPECT_TRUE(is_txing);
  EXPECT_EQ(dma_stream.bytes_available_for_write(), (uint32_t)2);
  uart_dma.DMA_tx_interrupt_handler();
  EXPECT_FALSE(is_txing);
  EXPECT_EQ(dma_stream.bytes_available_for_write(), (uint32_t)4);
}

TEST(FramingStreamsTests, FullStackTrivial) {
  DmaStream<20> dma_stream;
  EscapeStream esc_stream(dma_stream);
  CrcStream crc_stream(esc_stream);

  StreamResponse r;

  r = crc_stream.put(0x41);
  // begin frame marker and the '1'
  ASSERT_EQ(r.count_written, (uint32_t)2);
  r = crc_stream.put(0x42);
  ASSERT_EQ(r.count_written, (uint32_t)1);
  r = crc_stream.put(EndOfStream);
  // 4 bytes crc and end marker
  ASSERT_EQ(r.count_written, (uint32_t)5);
}
