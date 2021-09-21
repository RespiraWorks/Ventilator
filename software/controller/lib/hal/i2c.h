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

// [RM] 37.7 I2C Registers
struct I2CStruct {
  union {
    struct {
      uint32_t enable : 1;
      uint32_t tx_interrupts : 1;
      uint32_t rx_interrupts : 1;
      uint32_t addr_interrupts : 1;
      uint32_t nack_interrupts : 1;
      uint32_t stop_interrupts : 1;
      uint32_t tx_complete_interrupts : 1;
      uint32_t error_interrupts : 1;
      uint32_t digital_noise_filter_conntrol : 4;
      uint32_t analog_noise_filternig_off : 1;
      uint32_t reserved1 : 1;
      uint32_t dma_tx : 1;
      uint32_t dma_rx : 1;
      uint32_t slave_byte : 1;
      uint32_t no_clock_stretch : 1;
      uint32_t wake_from_stop : 1;
      uint32_t general_call : 1;
      uint32_t smbus_default_host : 1;
      uint32_t smbus_default_device : 1;
      uint32_t smbus_alert : 1;
      uint32_t smbus_packet_error_checking : 1;
      uint32_t reserved2 : 8;
    };
    uint32_t full_reg;
  } control_reg1;  // Control register 1 [RM] 37.7.1
  union {
    struct {
      uint32_t slave_addr_lsb : 1;  // lsb of 10 bits slave address
      // (master)
      uint32_t slave_addr_7b : 7;  // middle 7b bits of slave address
      // (master)
      uint32_t slave_addr_msb : 2;  // msb of 10 bits slave address
      // (master)
      uint32_t transfer_direction : 1;  // 0 = write, 1 = read (master)
      uint32_t address_10b : 1;         // Set to enable 10 bits address header
      // (master)
      uint32_t read_10b_header : 1;  // Clear to send complete read sequence
      // (master)
      uint32_t start : 1;  // Set to generate START condition
      uint32_t stop : 1;   // Set to generate STOP after byte transfer
      // (master)
      uint32_t nack : 1;                     // Generate NACK after byte reception (slave)
      uint32_t n_bytes : 8;                  // Set to the number of bytes to send
      uint32_t reload : 1;                   // Set to allow several consecutive transfers
      uint32_t autoend : 1;                  // Set to automatically send stop condition
      uint32_t packet_error_check_byte : 1;  // Set to send SMBus packet error
      // checking byte
      uint32_t reserved : 5;
    };
    uint32_t full_reg;
  } control2;        // Control register 2 [RM] 37.7.2
  uint32_t addr[2];  // Own address (1-2) register [RM] 37.7.{3,4} (slave)
  union {
    struct {
      uint32_t scl_low : 8;   // Duration of SCL Low state (cycles)
      uint32_t scl_high : 8;  // Duration of SCL High state (cycles)
      uint32_t sda_hold : 4;  // Delay between SCL falling edge and SDA
      // edge (cycles)
      uint32_t scl_delay : 4;  // Delay between SDA edge and SCL rising
      // edge (cycles)
      uint32_t reserved : 4;
      uint32_t prescaler : 4;  // Prescaler
    };
    uint32_t full_reg;
  } timing;          // Timing register [RM] 37.7.5
  uint32_t timeout;  // Timout register [RM] 37.7.6
  union {
    struct {
      uint32_t tx_empty : 1;
      uint32_t tx_interrupt : 1;
      uint32_t rx_not_empty : 1;
      uint32_t address_match : 1;
      uint32_t nack : 1;
      uint32_t stop : 1;
      uint32_t transfer_complete : 1;
      uint32_t transfer_reload : 1;
      uint32_t bus_error : 1;
      uint32_t arbitration_loss : 1;
      uint32_t overrun : 1;
      uint32_t packet_check_error : 1;
      uint32_t timeout : 1;
      uint32_t alert : 1;
      uint32_t reserved1 : 1;
      uint32_t busy : 1;
      uint32_t transfer_direction : 1;
      uint32_t address_code : 7;
      uint32_t reserved : 8;
    };
    uint32_t full_reg;
  } status;                     // Interrupt & status register [RM] 37.7.7
  uint32_t interrupt_clear;     // Interrupt clear register [RM] 37.7.8
  uint32_t packet_error_check;  // PEC register [RM] 37.7.9
  uint32_t rx_data;             // Receive data register [RM] 37.7.10
  uint32_t tx_data;             // Transmit data register [RM] 37.7.11
};
typedef volatile I2CStruct I2CReg;
inline I2CReg *const I2C1Base = reinterpret_cast<I2CReg *>(0x40005400);
inline I2CReg *const I2C2Base = reinterpret_cast<I2CReg *>(0x40005800);
inline I2CReg *const I2C3Base = reinterpret_cast<I2CReg *>(0x40005c00);
inline I2CReg *const I2C4Base = reinterpret_cast<I2CReg *>(0x40008400);

namespace I2C {

// Speed values correspond to the timing register value from [RM] table 182
enum class Speed {
  Slow = 0x3042C3C7,      // 10 kb/s
  Standard = 0x30420F13,  // 100 kb/s
  Fast = 0x10320309,      // 400 kb/s
  FastPlus = 0x00200204,  // 1 Mb/s
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
  void *data{nullptr};       // pointer to the data to be sent or received.
  bool *processed{nullptr};  // pointer to a boolean that informs the
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
  Channel() = default;
  // Method that ultimately sends a request over the I²C channel, note
  // that it may take some time to be processed: if the line is already
  // busy, the request ends up in a queue. Once the request has been
  // completed, *request.processed is set to True and (if the request is a
  // Read) *request.data contains desired data. Return value is false in
  // case the request cannot be processed (this can only happen if queue
  // is full).
  bool SendRequest(const Request &request);

  // Interrupt handlers
  void I2CEventHandler();
  void I2CErrorHandler();

 protected:
  // We queue of a few requests. The number of requests is arbitrary but
  // should be enough for all intents and purposes.
  static constexpr size_t QueueLength{80};

  // We copy the write data into a buffer to make sure nothing can be lost
  // due to the scope of the caller's variable. This is the buffer size.
  static constexpr size_t WriteBufferSize{4096};

  // Max retry-after-error allowed for a single request.
  static constexpr int8_t MaxRetries{5};

  // retry countdown - set to MaxRetries when starting a request
  // If it hits zero, we abort the request.
  int8_t error_retry_{MaxRetries};

  bool dma_enable_{false};

  bool transfer_in_progress_{false};

  void StartTransfer();               // initiate a transfer
  virtual void SetupI2CTransfer(){};  // configure a transfer
  void TransferByte();                // transfer a single byte (for non-DMA transfer)
  virtual void ReceiveByte(){};
  virtual void SendByte(){};
  virtual void WriteTransferSize(){};
  void EndTransfer();             // Clear necessary states
  virtual void StopTransfer(){};  // send stop condition

  // I²C interrupt getters:
  // Indicates that the hardware has processed the current byte and we can
  // read (or write) the next one in the Tx (or Rx) register.
  virtual bool NextByteNeeded() const { return false; };
  // Indicates that a 255 bytes transfer that does not end the current
  // request is completed.
  virtual bool TransferReload() const { return false; };
  // Indicates that a transfer is completed.
  virtual bool TransferComplete() const { return false; };
  // Indicates that the slave is busy, while this is not an error, per
  // say, we need to retry when a NACK occurs.  Because this is not an
  // error, this retry does not decrement the error_retry countdown.
  virtual bool NackDetected() const { return false; };
  // Interrupt clear
  virtual void ClearNack(){};
  virtual void ClearErrors(){};

  // Store the last request in order to be able to resume in case of
  // errors.
  Request last_request_;
  // For non-DMA transfers, store pointer to the next data to be
  // sent/received
  uint8_t *next_data_{nullptr};
  // For transfers longer than 255 bytes and non-DMA transfers, store size
  // of data that is still expected to be received/sent
  uint16_t remaining_size_{0};

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
  uint8_t write_buffer_[WriteBufferSize];
  size_t write_buffer_index_{0};
  size_t write_buffer_start_{0};
  size_t wrapping_index_{WriteBufferSize};
  bool CopyDataToWriteBuffer(const void *data, uint16_t size);
};

#ifdef BARE_STM32
class STM32Channel : public Channel {
 public:
  STM32Channel() = default;
  // Init I²C channel, setting up registers DMA_Reg and I²C_Reg to enable
  // the channel using DMA if possible. If DMA_Reg is invalid (or that DMA
  // cannot be linked to this I²C), dma is disabled and all transfers are
  // handled in software.
  void Init(I2CReg *i2c, DMA::Base dma, Speed speed);
  // Interrupt handlers for DMA, which only makes sense on the STM32
  void DMAIntHandler(DMA::Channel chan);

 private:
  I2CReg *i2c_{nullptr};
  DMA::Base dma_;
  volatile DmaReg::ChannelRegs *rx_channel_{nullptr};
  volatile DmaReg::ChannelRegs *tx_channel_{nullptr};

  void SetupI2CTransfer() override;  // configure a transfer
  void ReceiveByte() override { *next_data_ = static_cast<uint8_t>(i2c_->rx_data); };
  void SendByte() override { i2c_->tx_data = *next_data_; };
  void WriteTransferSize() override;
  void StopTransfer() override { i2c_->control2.stop = 1; };

  // Override interrupt getters:
  bool NextByteNeeded() const override {
    return i2c_->status.rx_not_empty || i2c_->status.tx_interrupt;
  };
  bool TransferReload() const override { return i2c_->status.transfer_reload; };
  bool TransferComplete() const override { return i2c_->status.transfer_complete; };
  bool NackDetected() const override { return i2c_->status.nack; };
  // Override Interrupt clear
  void ClearNack() override { i2c_->interrupt_clear = 0x10; }
  void ClearErrors() override { i2c_->interrupt_clear = 0x720; }

  void SetupDMAChannels(DMA::Base dma);
  void ConfigureDMAChannel(volatile DmaReg::ChannelRegs *channel, ExchangeDirection direction);
  void SetupDMATransfer();
};
#endif

class TestChannel : public Channel {
 public:
  TestChannel() = default;
  // in test mode, setters and getters for faked sent/received data
  std::optional<uint8_t> TESTGetSentData() { return sent_buffer_.Get(); };
  bool TESTQueueReceiveData(uint8_t data) { return rx_buffer_.Put(data); };
  // setter to simulate Nack received
  void TESTSimulateNack() { nack_ = true; };

 private:
  // in test mode, fake sending and receiving data through circular
  // buffers.
  CircularBuffer<uint8_t, WriteBufferSize> sent_buffer_;
  // note it is up to the tester to put data in the rx_buffer before
  // calling I2CEventHandler during a read request
  CircularBuffer<uint8_t, WriteBufferSize> rx_buffer_;
  // fake a NACK condition on next handler call
  bool nack_{false};

  // mock the sending and receiving of bytes from internal buffers
  void SendByte() override {
    bool ok = sent_buffer_.Put(*next_data_);
    if (ok) return;
  };
  void ReceiveByte() override {
    std::optional<uint8_t> data = rx_buffer_.Get();
    if (data != std::nullopt) {
      *next_data_ = *data;
    }
  };

  // Override I²C interrupt getters for Mock
  bool NextByteNeeded() const override { return remaining_size_ > 0; };
  bool TransferReload() const override {
    return remaining_size_ % 255 == 0 && remaining_size_ > 0;
  };
  bool TransferComplete() const override { return remaining_size_ == 0; };
  bool NackDetected() const override { return nack_; };

  void ClearNack() override { nack_ = false; };
};

}  // namespace I2C

#ifdef BARE_STM32
extern I2C::STM32Channel i2c1;
#else
extern I2C::Channel i2c1;
#endif
