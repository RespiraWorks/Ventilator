#ifndef __QSERIAL_OUTPUT_STREAM__
#define __QSERIAL_OUTPUT_STREAM__

#include "../common/libs/framing/framing_streams.h"
#include <QSerialPort>

class QSerialOutputStream : public OutputStream {
  std::unique_ptr<QSerialPort> &port_;

public:
  QSerialOutputStream(std::unique_ptr<QSerialPort> &port) : port_(port) {}

  // We can write virtually unlimited number of bytes
  uint32_t BytesAvailableForWrite() override { return 9001; }

  StreamResponse Put(int32_t b) override {
    if (END_OF_STREAM == b) {
      return {.count_written = 0, .flags = STREAM_SUCCESS};
    }
    port_->putChar(static_cast<uint8_t>(b));
    return {.count_written = 0, .flags = STREAM_SUCCESS};
  }
};
#endif
