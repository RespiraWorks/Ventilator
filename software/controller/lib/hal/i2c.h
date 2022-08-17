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

// This file defines the interface to the I²C peripherals in the STM32 HAL.
// See [RM] chapter 37 for details on their inner workings.
// Our design uses the STM32 as an I²C master

#pragma once

#include "circular_buffer.h"
#include "dma.h"
#include "gpio.h"
#include "vars.h"

namespace I2C {

enum class Base { I2C1, I2C2, I2C3, I2C4 };

// Speed values correspond to the timing register value from [RM] table 182
enum class Speed {
  Slow,      // 10 kb/s
  Standard,  // 100 kb/s
  Fast,      // 400 kb/s
  FastPlus,  // 1 Mb/s
};

enum class ExchangeDirection {
  Write = 0,
  Read = 1,
};

// Structure that represents an I²C request. It is up to the caller to
// ensure that size is consistent with data limits. For read requests, the
// caller must use a variable with the appropriate scope (ideally a static
// variable) to ensure the result is put at a safe memory.
struct Request {
  uint8_t slave_address{0};  // for now we only support 7 bits addresses
                             // and NOT 10 bits addresses (I²C bus is one
                             // or the other)
  ExchangeDirection direction{ExchangeDirection::Read};
  uint16_t size{1};          // size (in bytes) of the data to be sent or
                             // received - limited to 16 bits because of DMA
                             // limitation
  void* data{nullptr};       // pointer to the data to be sent or received.
  bool* processed{nullptr};  // pointer to a boolean that informs the
                             // caller that his request has been
                             // processed
};

// Class that represents an I²C channel (we have 4 of those on the STM32)
// Said channel may or may not use DMA (spoiler alert: ours does).
//
// I²C requests are queued in the class and processed in order.
// On the STM32, a request consists of one or several transfers of up to 255
// bytes.
//
// When we use DMA, a transfer is performed directly in hardware and the end
// of transfer triggers a DMA interrupt which we use to start the next
// transfer. Note that a current limitation is that DMA cannot be used for
// transfers that are longer than 255 bytes AND include a functional header
// within their data because our implementation doesn't support reload mode
// with DMA.
//
// When we don't, we listen to the I²C interrupts to know when to read/write
// the next byte in a transfer, and when a transfer is complete.
//
// We don't necessarily plan to use the second mode in production, but it
// should come in handy during integration testing, when we want to closely
// monitor what is sent on the bus.
class Channel {
 public:
  Channel(Base i2c, const char* name, const char* help_supplement) : i2c_(i2c) {
    UpdateDebugVariables(name, help_supplement);
  };
  Channel(Base i2c, DMA::Base dma, const char* name, const char* help_supplement);

  // Init I²C channel, setting up registers to enable the channel and use
  // DMA if a valid dma channel was provided during constuction.
  // If no DMA was given (or it cannot be linked to this I²C), DMA is disabled
  // and all transfers are handled in software.
  void Initialize(Speed speed, GPIO::Port port, uint8_t scl_pin, uint8_t sda_pin,
                  GPIO::AlternativeFunction alt_function);

  // Method that ultimately sends a request over the I²C channel, note
  // that it may take some time to be processed: if the line is already
  // busy, the request ends up in a queue. Once the request has been
  // completed, *request.processed is set to True and (if the request is a
  // Read) *request.data contains desired data. Return value is false in
  // case the request cannot be processed (this can only happen if queue
  // is full).
  bool SendRequest(const Request& request);

  // Interrupt handlers
  void I2CEventHandler();
  void I2CErrorHandler();
  // Interrupt handler for DMA, which only makes sense on the STM32
  void DMAInterruptHandler(ExchangeDirection direction);

  // Queue of a few requests. The number of requests is arbitrary but
  // should be enough for all intents and purposes.
  static constexpr size_t QueueLength{80};

 protected:
  // We copy the write data into a buffer to make sure nothing can be lost
  // due to the scope of the caller's variable. This is the buffer size.
  static constexpr size_t WriteBufferSize{4096};

  // Max retry-after-error allowed for a single request.
  static constexpr int8_t MaxRetries{5};

  // retry countdown - set to MaxRetries when starting a request
  // If it hits zero, we abort the request.
  int8_t error_retry_{MaxRetries};

  bool transfer_in_progress_{false};

  // For non-DMA transfers, store pointer to the next data to be
  // sent/received
  uint8_t* next_data_{nullptr};
  // For transfers longer than 255 bytes and non-DMA transfers, store size
  // of data that is still expected to be received/sent
  uint16_t remaining_size_{0};

 private:
  Base i2c_;
  std::optional<DMA::ChannelControl> rx_dma_{std::nullopt};
  std::optional<DMA::ChannelControl> tx_dma_{std::nullopt};
  bool dma_enable_{false};
  uint8_t request_{0};  // request number for dma setup

  void StartTransfer();             // initiate a transfer
  virtual void SetupI2CTransfer();  // configure I2C to process a transfer
  void SetupDMATransfer();          // configure DMA registers to process a transfer
  void TransferByte();              // transfer a single byte (for non-DMA transfer)
  virtual void ReceiveByte();
  virtual void SendByte();
  virtual void WriteTransferSize();
  void EndTransfer();           // Clear necessary states
  virtual void StopTransfer();  // send stop condition

  // I²C interrupt getters:
  // Indicates that the hardware has processed the current byte and we can
  // read (or write) the next one in the Tx (or Rx) register.
  virtual bool NextByteNeeded() const;
  // Indicates that a 255 bytes transfer that does not end the current
  // request is completed.
  virtual bool TransferReload() const;
  // Indicates that a transfer is completed.
  virtual bool TransferComplete() const;
  // Indicates that the slave is busy, while this is not an error, per
  // say, we need to retry when a NACK occurs.  Because this is not an
  // error, this retry does not decrement the error_retry countdown.
  virtual bool NackDetected() const;
  // Interrupt clear
  virtual void ClearNack();
  virtual void ClearErrors();

  // Store the last request in order to be able to resume in case of
  // errors.
  Request last_request_;

  // Because Request cannot be std::move'd (and is therefore not
  // compatible with our circular buffer template), we use a circular
  // buffer of indexes to know the queue state and let the tested template
  // worry about buffer management but we also use our own Request table
  // (to which the circular buffer elements lead)
  CircularBuffer<uint8_t, QueueLength> buffer_;
  Request queue_[QueueLength];
  uint8_t ind_queue_{0};

  // Write buffer: the caller may send a write request with the address of
  // a non static variable, or alter that variable after requesting,
  // therefore we need to store the bytes to write in our own buffer. We
  // might have used a circular buffer for this purpose but: a. We don't
  // want a single transfer to wrap around in the buffer.
  //    Especially true for DMA transfers, which can't handle this, and to
  //    simplify handling non-DMA ones.
  //    This cannot be achieved with the circular buffer template.
  // b. Data in a circular buffer is pop'ed out of the buffer when used,
  // which
  //    means we lose the ability to retry a request after an I²C (or DMA)
  //    error.
  uint8_t write_buffer_[WriteBufferSize] = {0};
  size_t write_buffer_index_{0};
  size_t write_buffer_start_{0};
  size_t wrapping_index_{WriteBufferSize};
  bool CopyDataToWriteBuffer(const void* data, uint16_t size);

  // Log specific events for debug purposes
  Debug::Variable::UInt32 write_buffer_full_{"write_buffer_full", Debug::Variable::Access::ReadOnly,
                                             0, "",
                                             "Counter of how many times we can't fit a send request"
                                             " to our write buffer"};
  Debug::Variable::UInt32 queue_full_{"queueing_errors", Debug::Variable::Access::ReadOnly, 0, "",
                                      "Counter of how many times we couldn't queue a request"};
  Debug::Variable::UInt32 invalid_transfer_{"invalid_transfers", Debug::Variable::Access::ReadOnly,
                                            0, "", "Counter of invalid byte transfers"};
  Debug::Variable::UInt32 nacked_transfers_{"nacked_transfers", Debug::Variable::Access::ReadOnly,
                                            0, "", "Counter of transfers NACK'ed by I2C slave"};
  Debug::Variable::UInt32 transfer_errors_{"transfer_errors", Debug::Variable::Access::ReadOnly, 0,
                                           "", "Counter of transfer errors"};
  void UpdateDebugVariables(const char* name, const char* help_supplement) {
    write_buffer_full_.prepend_name(name);
    write_buffer_full_.append_help(help_supplement);
    queue_full_.prepend_name(name);
    queue_full_.append_help(help_supplement);
    invalid_transfer_.prepend_name(name);
    invalid_transfer_.append_help(help_supplement);
    nacked_transfers_.prepend_name(name);
    nacked_transfers_.append_help(help_supplement);
    transfer_errors_.prepend_name(name);
    transfer_errors_.append_help(help_supplement);
  }
};

}  // namespace I2C
