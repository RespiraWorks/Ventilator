#include "gtest/gtest.h"

#include "framing_streams.h"
#include "uart_dma_stream.h"

class ObserverStream : public Stream {
  static constexpr uint32_t LEN = 20;
  uint32_t i = 0;
  int32_t saved[LEN];

public:
  StreamResponse Put(int32_t b) {
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
  uint32_t EncodedLength(int32_t b) { return EndOfStream == b ? 0 : 1; }
  // Returns number of bytes that can be written.
  uint32_t BytesAvailableForWrite() { return LEN - i; }
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

  crc_stream.Put(0);
  ASSERT_EQ(observer.get_index(), (uint32_t)1);
  ASSERT_EQ(crc_stream.Put(EndOfStream).count_written, (uint32_t)4);
  ASSERT_EQ(observer.get_index(), (uint32_t)5);
}

TEST(FramingStreamsTests, EscapeStreamTrivial) {
  ObserverStream observer;
  EscapeStream escape_stream(observer);
  EXPECT_EQ(escape_stream.Put(EndOfStream).count_written, (uint32_t)2);
  EXPECT_EQ(escape_stream.Put(0).count_written, (uint32_t)2);
  EXPECT_EQ(escape_stream.Put(0).count_written, (uint32_t)1);
  EXPECT_EQ(escape_stream.Put(FramingMark).count_written, (uint32_t)2);
  EXPECT_EQ(escape_stream.Put(FramingEscape).count_written, (uint32_t)2);
  EXPECT_EQ(escape_stream.Put(EndOfStream).count_written, (uint32_t)1);
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
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)4);

  r = dma_stream.Put(0);
  EXPECT_EQ(r.count_written, (uint32_t)1);
  EXPECT_EQ(r.flags, static_cast<uint32_t>(ResponseFlags::StreamSuccess));
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)3);
  EXPECT_FALSE(is_txing);

  r = dma_stream.Put(0);
  EXPECT_EQ(r.count_written, (uint32_t)1);
  EXPECT_EQ(r.flags, static_cast<uint32_t>(ResponseFlags::StreamSuccess));
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)2);
  EXPECT_TRUE(is_txing);

  r = dma_stream.Put(0);
  EXPECT_EQ(r.count_written, (uint32_t)1);
  EXPECT_EQ(r.flags, static_cast<uint32_t>(ResponseFlags::StreamSuccess));
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)1);

  r = dma_stream.Put(0);
  EXPECT_EQ(r.count_written, (uint32_t)1);
  EXPECT_EQ(r.flags, static_cast<uint32_t>(ResponseFlags::WarningBufferFull));
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)0);

  r = dma_stream.Put(0);
  EXPECT_EQ(r.count_written, (uint32_t)0);
  EXPECT_EQ(r.flags, static_cast<uint32_t>(ResponseFlags::ErrorBufferFull));

  uart_dma.DMA_tx_interrupt_handler();
  EXPECT_TRUE(is_txing);
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)2);
  uart_dma.DMA_tx_interrupt_handler();
  EXPECT_FALSE(is_txing);
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)4);
}

TEST(FramingStreamsTests, FullStackTrivial) {
  DmaStream<20> dma_stream;
  EscapeStream esc_stream(dma_stream);
  CrcStream crc_stream(esc_stream);

  StreamResponse r;

  r = crc_stream.Put(0x41);
  // begin frame marker and the '1'
  ASSERT_EQ(r.count_written, (uint32_t)2);
  r = crc_stream.Put(0x42);
  ASSERT_EQ(r.count_written, (uint32_t)1);
  r = crc_stream.Put(EndOfStream);
  // 4 bytes crc and end marker
  ASSERT_EQ(r.count_written, (uint32_t)5);
}
