#ifndef SOFT_RX_BUFFER_H_
#define SOFT_RX_BUFFER_H_
#include "serial_listeners.h"
#include <stdint.h>

template <int RX_BYTES_MAX> class SoftRxBuffer {
  uint8_t rx_buf_[RX_BYTES_MAX];
  uint32_t rx_i_ = 0;
  RxListener *rx_listener_;
  uint8_t match_char_ = 0;

public:
  explicit SoftRxBuffer(uint8_t match_char) : match_char_(match_char){};
  void RestartRX(RxListener *listener) {
    rx_i_ = 0;
    rx_listener_ = listener;
  }

  bool Begin(RxListener *listener) {
    RestartRX(listener);
    return true;
  };

  uint32_t ReceivedLength() { return rx_i_; };

  uint8_t *get() { return rx_buf_; }

  void PutByte(uint8_t b) {
    if (rx_i_ < RX_BYTES_MAX) {
      rx_buf_[rx_i_++] = b;
      if (match_char_ == b) {
        if (rx_listener_) {
          rx_listener_->OnCharacterMatch();
        }
      }
    }
    if (rx_i_ >= RX_BYTES_MAX) {
      if (rx_listener_) {
        rx_listener_->OnRxComplete();
      }
    }
  }
};

#endif
