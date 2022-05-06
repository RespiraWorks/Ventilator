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

////////////////////////////////////////////////////////////////////
//
// This file contains code to setup, send and receive data on the I²C channel
// See header file for implementation philosophy.
//
////////////////////////////////////////////////////////////////////

#include "i2c.h"

#include <cstring>

#include "clocks.h"

// Declaration of I2C channel, as global since hal requires it to exist for interrupt handlers
// TODO: find a way to get rid of this as global variable (probably requires a more flexible
// handling of InterruptVector in Reset_Handler() function from hal_stm32.cpp)
I2C::Channel i2c1(I2C::Base::I2C1, DMA::Base::DMA2);

namespace I2C {
// [RM] 37.7 I2C Registers
struct Register {
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
typedef volatile Register I2CReg;

// I2C base registers, per [RM] table 2 (p68)
I2CReg *get_register(const Base id) {
  switch (id) {
    case Base::I2C1:
      return reinterpret_cast<I2CReg *>(0x40005400);
    case Base::I2C2:
      return reinterpret_cast<I2CReg *>(0x40005800);
    case Base::I2C3:
      return reinterpret_cast<I2CReg *>(0x40005c00);
    case Base::I2C4:
      return reinterpret_cast<I2CReg *>(0x40008400);
  }
  // All cases covered above (and GCC checks this).
  __builtin_unreachable();
}

Channel::Channel(Base i2c, DMA::Base dma) : i2c_(i2c) {
  // DMA mapping for I²C (see [RM] p299)
  static struct {
    DMA::Base dma_base;
    Base i2c_base;
    DMA::Channel tx_channel_id;
    DMA::Channel rx_channel_id;
    uint8_t request_number;
  } DmaMap[] = {
      {DMA::Base::DMA1, Base::I2C1, DMA::Channel::Chan6, DMA::Channel::Chan7, 3},
      {DMA::Base::DMA1, Base::I2C2, DMA::Channel::Chan4, DMA::Channel::Chan5, 3},
      {DMA::Base::DMA1, Base::I2C3, DMA::Channel::Chan2, DMA::Channel::Chan3, 3},
      {DMA::Base::DMA2, Base::I2C1, DMA::Channel::Chan7, DMA::Channel::Chan6, 5},
      {DMA::Base::DMA2, Base::I2C4, DMA::Channel::Chan2, DMA::Channel::Chan1, 0},
  };

  for (auto &map : DmaMap) {
    if (dma == map.dma_base && i2c_ == map.i2c_base) {
      tx_dma_.emplace(dma, map.tx_channel_id);
      rx_dma_.emplace(dma, map.rx_channel_id);
      request_ = map.request_number;
      dma_enable_ = true;
      break;
    }
  }
}

void Channel::Initialize(Speed speed, GPIO::Port port, uint8_t scl_pin, uint8_t sda_pin,
                         GPIO::AlternativeFunction alt_function) {
  GPIO::AlternatePin(port, scl_pin, alt_function, GPIO::PullType::Up, GPIO::OutSpeed::Fast,
                     GPIO::OutType::OpenDrain);
  GPIO::AlternatePin(port, sda_pin, alt_function, GPIO::PullType::Up, GPIO::OutSpeed::Fast,
                     GPIO::OutType::OpenDrain);

  switch (i2c_) {
    case Base::I2C1:
      enable_peripheral_clock(PeripheralID::I2C1);
      Interrupts::singleton().EnableInterrupt(InterruptVector::I2c1Event, InterruptPriority::Low);
      Interrupts::singleton().EnableInterrupt(InterruptVector::I2c1Error, InterruptPriority::Low);
      break;
    case Base::I2C2:
      enable_peripheral_clock(PeripheralID::I2C2);
      Interrupts::singleton().EnableInterrupt(InterruptVector::I2c2Event, InterruptPriority::Low);
      Interrupts::singleton().EnableInterrupt(InterruptVector::I2c2Error, InterruptPriority::Low);
    case Base::I2C3:
      enable_peripheral_clock(PeripheralID::I2C3);
      Interrupts::singleton().EnableInterrupt(InterruptVector::I2c3Event, InterruptPriority::Low);
      Interrupts::singleton().EnableInterrupt(InterruptVector::I2c3Error, InterruptPriority::Low);
    case Base::I2C4:
      enable_peripheral_clock(PeripheralID::I2C4);
      Interrupts::singleton().EnableInterrupt(InterruptVector::I2c4Event, InterruptPriority::Low);
      Interrupts::singleton().EnableInterrupt(InterruptVector::I2c4Error, InterruptPriority::Low);
      break;
    default:
      // All cases covered above (and GCC checks this).
      __builtin_unreachable();
  }

  I2CReg *i2c_reg{get_register(i2c_)};

  // Disable I²C peripheral
  i2c_reg->control_reg1.enable = 0;

  // Set I²C speed using timing values from [RM] table 182
  i2c_reg->timing.full_reg = [speed] {
    switch (speed) {
      case Speed::Slow:
        return 0x3042C3C7;
      case Speed::Standard:
        return 0x30420F13;
      case Speed::Fast:
        return 0x10320309;
      case Speed::FastPlus:
        return 0x00200204;
    }
    // All cases covered above (and GCC checks this).
    __builtin_unreachable();
  }();

  // Setup DMA channels
  if (dma_enable_) {
    tx_dma_->Initialize(request_, &(i2c_reg->tx_data), DMA::ChannelDir::MemoryToPeripheral,
                        /*tx_interrupt=*/true, DMA::ChannelPriority::Low, InterruptPriority::Low);
    rx_dma_->Initialize(request_, &(i2c_reg->rx_data), DMA::ChannelDir::PeripheralToMemory,
                        /*tx_interrupt=*/true, DMA::ChannelPriority::Low, InterruptPriority::Low);
    i2c_reg->control_reg1.dma_rx = 1;
    i2c_reg->control_reg1.dma_tx = 1;
  }

  // enable I²C peripheral
  i2c_reg->control_reg1.enable = 1;

  // configure I²C interrupts
  i2c_reg->control_reg1.nack_interrupts = 1;
  i2c_reg->control_reg1.error_interrupts = 1;
  // in DMA mode, we do not treat the transfer-specific ones
  if (!dma_enable_) {
    i2c_reg->control_reg1.rx_interrupts = 1;
    i2c_reg->control_reg1.tx_interrupts = 1;
    i2c_reg->control_reg1.tx_complete_interrupts = 1;
  } else {
    i2c_reg->control_reg1.rx_interrupts = 0;
    i2c_reg->control_reg1.tx_interrupts = 0;
    i2c_reg->control_reg1.tx_complete_interrupts = 0;
  }
}

bool Channel::SendRequest(const Request &request) {
  *(request.processed) = false;
  // We need to ensure thread safety as this function might be
  // called from a timer interrupt as well as the main loop.
  // Also, because our ISR change the transfer_in_progress_ member variable.
  BlockInterrupts block;

  // Queue the request if possible: check that there is room in the index
  // buffer
  if (buffer_.IsFull()) {
    return false;
  }

  // Add the current queue_ index into the buffer
  if (buffer_.Put(ind_queue_)) {
    queue_[ind_queue_] = request;
  } else {
    return false;
  }

  // In case of a write request, copy data to our write buffer
  if (request.direction == ExchangeDirection::Write) {
    if (!CopyDataToWriteBuffer(request.data, request.size)) {
      return false;
    }
    // update the request's data pointer to the write buffer instead of the
    // caller's scope variable
    queue_[ind_queue_].data = &write_buffer_[write_buffer_index_];

    // update the write buffer index
    write_buffer_index_ += request.size;
  }
  // increment ind_queue_, which is the index at which the next request will
  // be put in the queue, with wrapping around the queue.
  if (++ind_queue_ >= QueueLength) {
    ind_queue_ = 0;
  }

  if (!transfer_in_progress_) {
    StartTransfer();
  }
  // if a transfer is already in progress, this request will be initiated by
  // the interrupt handlers, our work is done!
  return true;
}

bool Channel::CopyDataToWriteBuffer(const void *data, const uint16_t size) {
  // This protected function is only called from an already thread safe
  // function, but leaning on the safe side here, I am disabling interrupts
  // for this one anyway, in case someone changes the design of the I²C class.
  BlockInterrupts block;

  // Check if the empty space at the end of the buffer is big enough to
  // store all of the data
  if (write_buffer_index_ + size > WriteBufferSize) {
    // It isn't ==> Check if the empty space at the beginning of the buffer
    // is big enough to store all of the data instead
    if (size >= write_buffer_start_) {
      // There is no contiguous space left in buffer that is big enough,
      // we can't safely send this Write request.
      return false;
    }

    // We can write at the beginning of the buffer, need to remember that we
    // wrap at that index in order to properly wrap when updating
    // write_buffer_start_ when the (previous) transfer will end.
    wrapping_index_ = write_buffer_index_;
    write_buffer_index_ = 0;
  }

  memcpy(&write_buffer_[write_buffer_index_], data, size);
  return true;
}

void Channel::StartTransfer() {
  transfer_in_progress_ = true;
  // In DMA mode, a single request can lead to several transfers, when it is
  // longer than 255 bytes. Therefore we need to check whether this call is a
  // continuation of a long request or a new request.
  // Also in case of transfer error, we may need to re-send the last request
  if (remaining_size_ == 0) {
    // Ensure thread safety
    BlockInterrupts block;
    // This indicates the last request has been successfully sent, hence we
    // will send the next request in the queue.
    std::optional<uint8_t> index = buffer_.Get();
    if (index == std::nullopt) {
      // no request in the queue
      transfer_in_progress_ = false;
      return;
    }
    last_request_ = queue_[*index];
    next_data_ = reinterpret_cast<uint8_t *>(last_request_.data);
    remaining_size_ = last_request_.size;
    error_retry_ = MaxRetries;
  }

  SetupI2CTransfer();
}

void Channel::SetupI2CTransfer() {
  I2CReg *i2c_reg{get_register(i2c_)};
  // set transfer-specific registers per [RM] p1149 to 1158
  i2c_reg->control2.slave_addr_7b = last_request_.slave_address & 0x7f;
  i2c_reg->control2.transfer_direction = static_cast<bool>(last_request_.direction);

  if (dma_enable_) {
    SetupDMATransfer();
  }

  WriteTransferSize();

  i2c_reg->control2.start = 1;
}

// Method called by interrupt handler when dma is disabled. This method
// transfers data to/from the tx/rx registers from/to *request.data
void Channel::TransferByte() {
  if (remaining_size_ == 0) {
    // this shouldn't happen, but just to be safe, we stop here.
    return;
  }
  if (last_request_.direction == ExchangeDirection::Read) {
    ReceiveByte();
  } else {
    SendByte();
  }
  if (--remaining_size_ > 0) {
    // increment next_data_ pointer only in case we are still expecting more
    // data to prevent unwarranted memory access
    next_data_ += sizeof(uint8_t);
  };
}

void Channel::ReceiveByte() { *next_data_ = static_cast<uint8_t>(get_register(i2c_)->rx_data); }

void Channel::SendByte() { get_register(i2c_)->tx_data = *next_data_; }

// Write the remaining size to the appropriate register with reload logic
void Channel::WriteTransferSize() {
  I2CReg *i2c_reg{get_register(i2c_)};
  if (remaining_size_ <= 255) {
    i2c_reg->control2.n_bytes = static_cast<uint8_t>(remaining_size_);
    i2c_reg->control2.reload = 0;
  } else {
    i2c_reg->control2.n_bytes = 255;
    // Transfer reload is not currently supported by our HAL in DMA mode,
    // we will treat a reload as a new transfer. In effect this means we
    // will have to reissue the I²C header and start condition.
    // It also means that any request that has a (functional) header inside
    // its data and is longer than 255 bytes may not be processed correctly
    // by the I²C slave, as it will be split (this is referred to as a
    // Restart condition rather than Reload)
    if (!dma_enable_) {
      i2c_reg->control2.reload = 1;
    } else {
      i2c_reg->control2.reload = 0;
    }
  }
}

void Channel::EndTransfer() {
  // Ensure thread safety
  BlockInterrupts block;
  *last_request_.processed = true;
  if (last_request_.direction == ExchangeDirection::Write) {
    // free the part of the write buffer that was dedicated to this request
    write_buffer_start_ += last_request_.size;
    if (write_buffer_start_ >= wrapping_index_) {
      // We don't allow data in the same request to wrap around, so we
      // must detect that the next write data actually starts at index 0
      // to properly free the end of the buffer as well
      write_buffer_start_ = 0;
    }
  }
  transfer_in_progress_ = false;
}

void Channel::StopTransfer() { get_register(i2c_)->control2.stop = 1; };

bool Channel::NextByteNeeded() const {
  return get_register(i2c_)->status.rx_not_empty || get_register(i2c_)->status.tx_interrupt;
};
// Interrupt getters
bool Channel::TransferReload() const { return get_register(i2c_)->status.transfer_reload; }
bool Channel::TransferComplete() const { return get_register(i2c_)->status.transfer_complete; }
bool Channel::NackDetected() const { return get_register(i2c_)->status.nack; }
// Interrupt clear
void Channel::ClearNack() { get_register(i2c_)->interrupt_clear = 0x10; }
void Channel::ClearErrors() { get_register(i2c_)->interrupt_clear = 0x720; }

void Channel::I2CEventHandler() {
  if (!transfer_in_progress_) {
    return;
  }

  // resend the request in case the slave NACK'd our request
  if (NackDetected()) {
    // clear the nack
    ClearNack();
    // the slave is non-responsive --> start the request anew
    next_data_ = reinterpret_cast<uint8_t *>(last_request_.data);
    remaining_size_ = last_request_.size;
    StartTransfer();
  }

  // When we are using DMA, NACK is the only I²C event we are dealing with
  if (dma_enable_) {
    return;
  }

  if (NextByteNeeded()) {
    // carry on with the current transfer
    TransferByte();
  }

  if (TransferReload()) {
    // To continue a request that is longer than 255 bits, all we need to do
    // is write a non-zero transfer size, and set the reload byte
    // accordingly (see [RM] p1151 (Write request) and 1155 (Read request))
    WriteTransferSize();
  }

  if (TransferComplete()) {
    // Send stop condition
    StopTransfer();
    // Clean necessary states
    EndTransfer();
    // And start the next one (if any)
    StartTransfer();
  }
}

void Channel::SetupDMATransfer() {
  if (!dma_enable_) {
    return;
  }
  // to be on the safe size, disable both channels in case they weren't
  // (likely when this is called as a "retry after error")
  rx_dma_->Disable();
  tx_dma_->Disable();

  DMA::ChannelControl *channel{nullptr};
  if (last_request_.direction == ExchangeDirection::Read) {
    channel = &(rx_dma_.value());
  } else {
    channel = &(tx_dma_.value());
  }

  uint16_t transfer_size{0};
  if (remaining_size_ <= 255) {
    transfer_size = remaining_size_;
  } else {
    transfer_size = 255;
  }

  channel->SetupTransfer(next_data_, transfer_size);

  // when using DMA, we need to use autoend, otherwise the STOP condition
  // which we issue at the end of the DMA transfer (which means the last byte
  // has been written to the register) may arrive before the last byte is
  // actually written on the line. Tests with both DMA and I2C interrupts
  // enabled to send Stop at the end of the I2C transfer were inconclusive.
  get_register(i2c_)->control2.autoend = 1;

  channel->Enable();
}

// Interrupt handlers
void Channel::I2CErrorHandler() {
  // I²C error --> clear all error flags (except those that are SMBus only)
  ClearErrors();
  // and restart the request up to error_retry_ times
  if (--error_retry_ > 0) {
    next_data_ = reinterpret_cast<uint8_t *>(last_request_.data);
    remaining_size_ = last_request_.size;
  } else {
    // skip this request and go to next one;
    remaining_size_ = 0;
  }
  StartTransfer();
}

void Channel::DMAInterruptHandler(ExchangeDirection direction) {
  if (!dma_enable_ || !transfer_in_progress_) return;

  DMA::ChannelControl *channel{nullptr};
  if (last_request_.direction == ExchangeDirection::Read) {
    channel = &(rx_dma_.value());
  } else {
    channel = &(tx_dma_.value());
  }

  channel->Disable();
  if (channel->InterruptStatus(DMA::Interrupt::TransferComplete)) {
    if (remaining_size_ > 255) {
      // decrement remaining size by 255 (the size of the DMA transfer)
      remaining_size_ = static_cast<uint16_t>(remaining_size_ - 255);
      next_data_ += 255 * sizeof(uint8_t);
    } else {
      remaining_size_ = 0;
      EndTransfer();
    }
  } else if (channel->InterruptStatus(DMA::Interrupt::TransferError)) {
    // we are dealing with an error --> reset transfer (up to MaxRetries
    // times)
    if (--error_retry_ > 0) {
      next_data_ = reinterpret_cast<uint8_t *>(last_request_.data);
      remaining_size_ = last_request_.size;
    } else {
      // skip this request and go to next request;
      remaining_size_ = 0;
    }
  }
  // clear all interrupts and (re-)start the current or next transfer
  channel->ClearInterrupt(DMA::Interrupt::Global);
  StartTransfer();
}

}  // namespace I2C
