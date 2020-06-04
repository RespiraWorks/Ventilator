#include "gtest/gtest.h"

#include "framing_streams.h"
#include "uart_dma_stream.h"

class ObserverStream : public Stream {
  static constexpr uint32_t LEN = 20;
  uint32_t i = 0;
  int32_t saved[LEN];

public:
  StreamResponse Put(int32_t b) {
    if (END_OF_STREAM == b) {
      return {0, 0};
    }

    if (i < LEN) {
      saved[i++] = b;
      return {1, 0};
    } else {
      return {0, ERROR_BUFFER_FULL};
    }
  }
  // Returns number of bytes of encoded frame after processing the given byte.
  uint32_t EncodedLength(int32_t b) { return END_OF_STREAM == b ? 0 : 1; }
  // Returns number of bytes that can be written.
  uint32_t BytesAvailableForWrite() { return LEN - i; }
  int32_t *get_saved() { return saved; }
  uint32_t get_index() { return i; }
};

TEST(StreamResponse, StreamResponseSum) {
  StreamResponse r1 = {42, 0};
  StreamResponse r2 = {52, 0};
  r2 += r1;
  ASSERT_EQ(r2.count_written, (uint32_t)(42 + 52));
}

TEST(FramingStreamsTests, CrcStreamTrivial) {
  ObserverStream observer;
  CrcStream crc_stream(observer);

  crc_stream.Put(0);
  ASSERT_EQ(observer.get_index(), (uint32_t)1);
  ASSERT_EQ(crc_stream.Put(END_OF_STREAM).count_written, (uint32_t)4);
  ASSERT_EQ(observer.get_index(), (uint32_t)5);
}

TEST(FramingStreamsTests, EscapeStreamTrivial) {
  ObserverStream observer;
  EscapeStream escape_stream(observer);
  EXPECT_EQ(escape_stream.Put(END_OF_STREAM).count_written, (uint32_t)2);
  EXPECT_EQ(escape_stream.Put(0).count_written, (uint32_t)2);
  EXPECT_EQ(escape_stream.Put(0).count_written, (uint32_t)1);
  EXPECT_EQ(escape_stream.Put(FRAMING_MARK).count_written, (uint32_t)2);
  EXPECT_EQ(escape_stream.Put(FRAMING_ESC).count_written, (uint32_t)2);
  EXPECT_EQ(escape_stream.Put(END_OF_STREAM).count_written, (uint32_t)1);
}

uint8_t tx_buffer[20];
uint32_t tx_length = 0;
bool is_txing = false;
TxListener *tx_listener;

bool UART_DMA::isTxInProgress() { return is_txing; }
bool UART_DMA::startTX(const uint8_t *buf, uint32_t length, TxListener *txl) {
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

void UART_DMA::DMA_TX_ISR() {
  is_txing = false;
  tx_listener->onTxComplete();
}

UART_DMA uart_dma;

TEST(FramingStreamsTests, DmaStreamTrivial) {
  DmaStream<2> dma_stream;
  StreamResponse r;
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)4);

  r = dma_stream.Put(0);
  EXPECT_EQ(r.count_written, (uint32_t)1);
  EXPECT_EQ(r.flags, STREAM_SUCCESS);
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)3);
  EXPECT_FALSE(is_txing);

  r = dma_stream.Put(0);
  EXPECT_EQ(r.count_written, (uint32_t)1);
  EXPECT_EQ(r.flags, STREAM_SUCCESS);
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)2);
  EXPECT_TRUE(is_txing);

  r = dma_stream.Put(0);
  EXPECT_EQ(r.count_written, (uint32_t)1);
  EXPECT_EQ(r.flags, STREAM_SUCCESS);
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)1);

  r = dma_stream.Put(0);
  EXPECT_EQ(r.count_written, (uint32_t)1);
  EXPECT_EQ(r.flags, WARNING_BUFFER_FULL);
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)0);

  r = dma_stream.Put(0);
  EXPECT_EQ(r.count_written, (uint32_t)0);
  EXPECT_EQ(r.flags, ERROR_BUFFER_FULL);

  uart_dma.DMA_TX_ISR();
  EXPECT_TRUE(is_txing);
  EXPECT_EQ(dma_stream.BytesAvailableForWrite(), (uint32_t)2);
  uart_dma.DMA_TX_ISR();
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
  r = crc_stream.Put(END_OF_STREAM);
  // 4 bytes crc and end marker
  ASSERT_EQ(r.count_written, (uint32_t)5);
}
