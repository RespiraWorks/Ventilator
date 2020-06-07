#ifndef __SERIAL_LISTENERS_H
#define __SERIAL_LISTENERS_H

enum class RxError { UNKNOWN, OVERRUN, SERIAL_FRAMING, TIMEOUT, DMA };

// An interface that gets called on serial events.
// NOTE: all callbacks are called from interrupt context!
class RxListener {
public:
  // Called when the ordered number of bytes is transfered from UART to memory
  // over DMA
  virtual void OnRxComplete() = 0;
  // Called on specified character reception
  virtual void OnCharacterMatch() = 0;
  // Called on RX errors
  virtual void OnRxError(RxError) = 0;
};

class TxListener {
public:
  // Called when the ordered nuber of bytes is transfered from memory to UART
  // over DMA
  virtual void OnTxComplete() = 0;
  // Called on TX errors
  virtual void OnTxError() = 0;
};

#endif
