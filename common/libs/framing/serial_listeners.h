#ifndef __SERIAL_LISTENERS_H
#define __SERIAL_LISTENERS_H

enum RxError_t {
  RX_ERROR_UNKNOWN,
  RX_ERROR_OVR,
  RX_ERROR_SERIAL_FRAMING,
  RX_ERROR_TIMEOUT,
  RX_ERROR_DMA
};

// An interface that gets called on serial events.
// NOTE: all callbacks are called from interrupt context!
class RxListener {
public:
  // Called on RX complete
  virtual void onRxComplete() = 0;
  // Called on specified character reception
  virtual void onCharacterMatch() = 0;
  // Called on RX errors
  virtual void onRxError(RxError_t) = 0;
};

class TxListener {
public:
  // Called on TX complete
  virtual void onTxComplete() = 0;
  // Called on TX errors
  virtual void onTxError() = 0;
};

#endif
