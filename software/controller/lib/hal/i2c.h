#if defined(BARE_STM32)

#ifndef I2C_H
#define I2C_H

#include "circular_buffer.h"
#include "hal_stm32.h"

enum class I2CSpeed {
  kSlow,     // 10 kb/s
  kStandard, // 100 kb/s
  kFast,     // 400 kb/s
  kFastPlus, // 1 Mb/s
};

// Structure that represents an I2C request. It is up to the caller to ensure
// that size is consistent with data limits: Because we are queueing requests,
// we cannot allocate a buffer in the I2CChannel class: we would need several
// separate buffers of maximum size... with our architecture, each caller can
// (must) tailor memory allocation to its needs
struct I2CRequest {
  uint8_t slave_address{0}; // for now we only support 7 bits addresses
  I2CExchangeDir read_write{I2CExchangeDir::kRead};
  uint16_t size{1};         // size of the data to be sent or recieved
                            // limited to 16 bytes because of DMA limitation
  void *data{nullptr};      // pointer to the data to be sent or recieved.
  bool *processed{nullptr}; // pointer to a boolean that informs the caller that
                            // his request has been processed
};

// Class that represents an I2C channel (we have 4 of those on the STM32)
// Said channel may or may not use DMA (spoiler alert: the one we do use does).
//
// I2C requests are queued in the class and processed in order.
// A request consists of one or several transfers of up to 255 bytes.
//
// When we use DMA, a transfer is performed directly in hardware and the end of
// transfer triggers a DMA interrupt which we use to start the next transfer.
//
// When we don't, we listen to the i2c interrupts to know when to read/write
// the next byte in a transfer, and when a transfer is complete.
//
// We don't necessarily plan to use the second mode in production, but it should
// come in handy during integration testing, when we want to closely monitor
// what is sent on the bus.
class I2CChannel {
public:
  // Init I2C channel, setting up registers DMA_Reg and I2C_Reg to enable the
  // channel using DMA if possible.
  // If DMA_Reg is invalid (or that DMA cannot be linked to this I2C), dma is
  // disabled and all transfers are handled in software.
  void Init(I2C_Regs *i2c, DMA_Regs *dma, I2CSpeed speed);

  // Method that ultimately sends a request over the I2C channel, note that it
  // may take some time to be processed: if the line is already busy, the
  // request ends up in a queue.
  // Once the request has been completed, request.processed is set to True and
  // (if the request is a Read) request.data contains desired data.
  // Return value is false in case the request cannot be processed (this can
  // only happen if queue is full).
  bool SendRequest(const I2CRequest request);

  // Interrupt handlers
  void I2CEventHandler();
  void I2CErrorHandler();
  void DMAIntHandler(DMA_Chan chan);

private:
  I2C_Regs *i2c_{nullptr};
  DMA_Regs *dma_{nullptr};
  DMA_Chan rx_channel_;
  DMA_Chan tx_channel_;

  static constexpr int8_t kMaxRetries{5};
  int8_t error_retry_{kMaxRetries};

  bool dma_enable_{false};

  bool transfer_in_progress_{false};

  void StartTransfer(); // initiate a transfer
  void ByteTransfer();  // transfer a single byte (for non-DMA transfer)

  // store the last request in order to be able to resume in case of errors
  I2CRequest last_request_;
  // for non-DMA transfers, store pointer to the next data to be sent/recieved
  uint8_t *next_data_{nullptr};
  // for transfers longer than 255 bytes and non-DMA transfers, store size of
  // data that is still expected to be recieved/sent
  uint16_t remaining_size_{0};

  // Queue of a few requests. the number of requests is arbitrary but should be
  // enough for all intents and purposes.
  static uint8_t constexpr kQueueLength{40};
  // Because I2Crequest cannot be std::moved (and is therefore not compatible
  // with circular buffer), we use a circular buffer of indexes to know the
  // queue state and let the tested template worry about buffer management but
  // we also use our own I2Crequest table (to which the circular buffer
  // elements lead)
  CircularBuffer<uint8_t, kQueueLength> buffer_;
  I2CRequest queue_[kQueueLength];
  uint8_t ind_queue_{0};
};

extern I2CChannel i2c1;

#endif // I2C_H
#endif // BARE_STM32
