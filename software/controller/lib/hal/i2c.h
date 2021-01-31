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

#ifndef I2C_H
#define I2C_H

#include "circular_buffer.h"
#if defined(BARE_STM32)
#include "hal_stm32.h"
#endif

namespace I2C {

// Speed values correspond to the timing register value from [RM] table 182
enum class Speed {
  kSlow = 0x3042C3C7,     // 10 kb/s
  kStandard = 0x30420F13, // 100 kb/s
  kFast = 0x10320309,     // 400 kb/s
  kFastPlus = 0x00200204, // 1 Mb/s
};

enum class ExchangeDirection {
  kWrite = 0,
  kRead = 1,
};

// Structure that represents an I²C request. It is up to the caller to ensure
// that size is consistent with data limits.
// For read requests, the caller must use a variable with the appropriate scope
// (ideally a static variable) to ensure the result is put at a safe memory.
struct Request {
  uint8_t slave_address{0}; // for now we only support 7 bits addresses and NOT
                            // 10 bits addresses (I²C bus is one or the other)
  ExchangeDirection direction{ExchangeDirection::kRead};
  uint16_t size{1};         // size (in bytes) of the data to be sent or
                            // recieved - limited to 16 bytes because of DMA
                            // limitation
  void *data{nullptr};      // pointer to the data to be sent or recieved.
  bool *processed{nullptr}; // pointer to a boolean that informs the caller that
                            // his request has been processed
};

// Class that represents an I²C channel (we have 4 of those on the STM32)
// Said channel may or may not use DMA (spoiler alert: ours does).
//
// I²C requests are queued in the class and processed in order.
// On the STM32, a request consists of one or several transfers of up to 255
// bytes.
//
// When we use DMA, a transfer is performed directly in hardware and the end of
// transfer triggers a DMA interrupt which we use to start the next transfer.
//
// When we don't, we listen to the I²C interrupts to know when to read/write
// the next byte in a transfer, and when a transfer is complete.
//
// We don't necessarily plan to use the second mode in production, but it should
// come in handy during integration testing, when we want to closely monitor
// what is sent on the bus.
class Channel {
public:
  Channel() = default;
  // Method that ultimately sends a request over the I²C channel, note that it
  // may take some time to be processed: if the line is already busy, the
  // request ends up in a queue.
  // Once the request has been completed, *request.processed is set to True and
  // (if the request is a Read) *request.data contains desired data.
  // Return value is false in case the request cannot be processed (this can
  // only happen if queue is full).
  bool SendRequest(const Request &request);

  // Interrupt handlers
  void I2CEventHandler();
  void I2CErrorHandler();

protected:
  // We queue of a few requests. The number of requests is arbitrary but should
  // be enough for all intents and purposes.
  static constexpr uint8_t kQueueLength{80};

  // We copy the write data into a buffer to make sure nothing can be lost due
  // to the scope of the caller's variable. This is the buffer size.
  static constexpr uint32_t kWriteBufferSize{4096};

  // Max retry-after-error allowed for a single request.
  static constexpr int8_t kMaxRetries{5};

  // retry countdown - set to kMaxRetries when startng a request
  // If it hits zero, we abort the request.
  int8_t error_retry_{kMaxRetries};

  bool dma_enable_{false};

  bool transfer_in_progress_{false};

  void StartTransfer();              // initiate a transfer
  virtual void SetupI2CTransfer(){}; // configure a transfer
  void TransferByte(); // transfer a single byte (for non-DMA transfer)
  virtual void ReceiveByte(){};
  // {*next_data_ = static_cast<uint8_t>(i2c_->rxData);};
  virtual void SendByte(){};
  // {i2c_->txData = *next_data_;};
  virtual void ReloadTransfer(){};
  void EndTransfer();            // Clear necessary states
  virtual void StopTransfer(){}; // send stop condition
  // {i2c_->ctrl2.stop = 1;};

  // I²C interrupt getters:
  // Indicates that the hardware has processed the current byte and we can read
  // (or write) the next one in the Tx (or Rx) register.
  virtual bool NextByteNeeded() const { return false; };
  // Indicates that a 255 bytes transfer that does not end the current request
  // is completed.
  virtual bool TransferReload() const { return false; };
  // Indicates that a transfer is completed.
  virtual bool TransferComplete() const { return false; };
  // Indicates that the slave is busy, while this is not an error, per say, we
  // need to retry when a NACK occurs.  Because this is not an error, this retry
  // does not decrement the error_retry countdown.
  virtual bool NackDetected() const { return false; };
  // Interrupt clear
  virtual void ClearNack(){};
  virtual void ClearErrors(){};

  // Store the last request in order to be able to resume in case of errors.
  Request last_request_;
  // For non-DMA transfers, store pointer to the next data to be sent/recieved
  uint8_t *next_data_{nullptr};
  // For transfers longer than 255 bytes and non-DMA transfers, store size of
  // data that is still expected to be recieved/sent
  uint16_t remaining_size_{0};

  // Because Request cannot be std::move'd (and is therefore not compatible
  // with our circular buffer template), we use a circular buffer of indexes to
  // know the queue state and let the tested template worry about buffer
  // management but we also use our own Request table (to which the circular
  // buffer elements lead)
  CircularBuffer<uint8_t, kQueueLength> buffer_;
  Request queue_[kQueueLength];
  uint8_t ind_queue_{0};

  // Write buffer: the caller may send a write request with the address of a non
  // static variable, or alter that variable after requesting, therefore we need
  // to store the bytes to write in our own buffer.
  // We might have used a circular buffer for this purpose but:
  // a. We don't want a single transfer to wrap around in the buffer.
  //    Especially true for DMA transfers, which can't handle this, and to
  //    simplify handling non-DMA ones.
  //    This cannot be achieved with the circular buffer template.
  // b. Data in a circular buffer is pop'ed out of the buffer when used, which
  //    means we lose the ability to retry a request after an I²C (or DMA)
  //    error.
  uint8_t write_buffer_[kWriteBufferSize];
  uint32_t write_buffer_index_{0};
  uint32_t write_buffer_start_{0};
  uint32_t wrapping_index_{kWriteBufferSize};
  bool CopyDataToWriteBuffer(const void *data, uint16_t size);
};

#ifdef BARE_STM32
class STM32Channel : public Channel {
public:
  STM32Channel() = default;
  // Init I²C channel, setting up registers DMA_Reg and I²C_Reg to enable the
  // channel using DMA if possible.
  // If DMA_Reg is invalid (or that DMA cannot be linked to this I²C), dma is
  // disabled and all transfers are handled in software.
  void Init(I2C_Regs *i2c, DMA_Regs *dma, Speed speed);
  // Interrupt handlers for DMA, which only makes sense on the STM32
  void DMAIntHandler(DMA_Chan chan);

private:
  I2C_Regs *i2c_{nullptr};
  DMA_Regs *dma_{nullptr};
  DMA_Regs::ChannelRegs *rx_channel_{nullptr};
  DMA_Regs::ChannelRegs *tx_channel_{nullptr};

  void SetupI2CTransfer(); // configure a transfer
  void ReceiveByte() { *next_data_ = static_cast<uint8_t>(i2c_->rxData); };
  void SendByte() { i2c_->txData = *next_data_; };
  void ReloadTransfer();
  void StopTransfer() { i2c_->ctrl2.stop = 1; };

  // Override interrupt getters:
  bool NextByteNeeded() const {
    return i2c_->status.rx_not_empty || i2c_->status.tx_interrupt;
  };
  bool TransferReload() const { return i2c_->status.transfer_reload; };
  bool TransferComplete() const { return i2c_->status.transfer_complete; };
  bool NackDetected() const { return i2c_->status.nack; };
  // Override Interrupt clear
  void ClearNack() { i2c_->intClr = 0x10; }
  void ClearErrors() { i2c_->intClr = 0x720; }

  void SetupDMAChannels(DMA_Regs *dma);
  void ConfigureDMAChannel(DMA_Regs::ChannelRegs *channel,
                           ExchangeDirection direction);
  void SetupDMATransfer();
};
#endif

class TestChannel : public Channel {
public:
  TestChannel() = default;
  // in test mode, setters and getters for faked sent/received data
  std::optional<uint8_t> TEST_GetSentData() { return sent_buffer_.Get(); };
  bool TEST_QueueReceiveData(uint8_t data) { return rx_buffer_.Put(data); };
  // setter to simulate Nack received
  void TEST_SimulateNack() { nack_ = true; };

private:
  // in test mode, fake sending and receiving data through circular buffers.
  CircularBuffer<uint8_t, kWriteBufferSize> sent_buffer_;
  // note it is up to the tester to put data in the rx_buffer before calling
  // I2CEventHandler during a read request
  CircularBuffer<uint8_t, kWriteBufferSize> rx_buffer_;
  // fake a NACK condition on next handler call
  bool nack_{false};

  // mock the sending and recieving of bytes from internal buffers
  void SendByte() {
    bool OK = sent_buffer_.Put(*next_data_);
    if (OK)
      return;
  };
  void ReceiveByte() {
    std::optional<uint8_t> data = rx_buffer_.Get();
    if (data != std::nullopt) {
      *next_data_ = *data;
    }
  };

  // Override I²C interrupt getters for Mock
  bool NextByteNeeded() const { return remaining_size_ > 0; };
  bool TransferReload() const {
    return remaining_size_ % 255 == 0 && remaining_size_ > 0;
  };
  bool TransferComplete() const { return remaining_size_ == 0; };
  bool NackDetected() const { return nack_; };

  void ClearNack() { nack_ = false; };
};

} // namespace I2C

#ifdef BARE_STM32
extern I2C::STM32Channel i2c1;
#endif

#endif // I2C_H
