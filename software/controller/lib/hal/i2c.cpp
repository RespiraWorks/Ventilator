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
#include "hal.h"
#include <cstring>

#if defined(BARE_STM32)
#include "hal_stm32.h"
I2C::STM32Channel i2c1;

// Reference abbreviations ([RM], [PCB], etc) are defined in hal/README.md
void HalApi::InitI2C() {
  // Enable I2C1 and DMA2 peripheral clocks (we use DMA2 to send/receive data)
  EnableClock(I2C1_BASE);
  EnableClock(DMA2_BASE);

  // The following pins are used as i2c1 bus on the rev-1 PCB (see [PCB]):
  // - PB8 (I2C1 - DATA)
  // - PB9 (I2C1 - CLOCK)
  // Set Pin Function to I²C (see [DS] Table 17)
  GPIO_PinAltFunc(GPIO_B_BASE, 8, 4);
  GPIO_PinAltFunc(GPIO_B_BASE, 9, 4);
  // Set output speed to HIGH
  GPIO_OutSpeed(GPIO_B_BASE, 8, GPIO_OutSpeed::HIGH);
  GPIO_OutSpeed(GPIO_B_BASE, 9, GPIO_OutSpeed::HIGH);
  // Set open drain mode
  GPIO_OutType(GPIO_B_BASE, 8, GPIO_OutType::OPENDRAIN);
  GPIO_OutType(GPIO_B_BASE, 9, GPIO_OutType::OPENDRAIN);
  // Set Pull Up resistors
  GPIO_PullUp(GPIO_B_BASE, 8);
  GPIO_PullUp(GPIO_B_BASE, 9);

  EnableInterrupt(InterruptVector::I2C1_EV, IntPriority::LOW);
  EnableInterrupt(InterruptVector::I2C1_ERR, IntPriority::LOW);
  EnableInterrupt(InterruptVector::DMA2_CH6, IntPriority::LOW);
  EnableInterrupt(InterruptVector::DMA2_CH7, IntPriority::LOW);

  // init i2c1
  i2c1.Init(I2C1_BASE, DMA2_BASE, I2C::Speed::kFast);
}

// Those interrupt service routines are specific to our configuration, unlike
// the I2C::Channel::*ISR() which are generic ISR associated with an I²C channel
void I2C1_EV_ISR() { i2c1.I2CEventHandler(); };

void I2C1_ER_ISR() { i2c1.I2CErrorHandler(); };

void DMA2_CH6_ISR() { i2c1.DMAIntHandler(DMA_Chan::C6); };

void DMA2_CH7_ISR() { i2c1.DMAIntHandler(DMA_Chan::C7); };
#else
I2C::Channel i2c1;
#endif // BARE_STM32

namespace I2C {

bool Channel::SendRequest(const Request &request) {
  *(request.processed) = false;
  // We need to ensure thread safety as this function might be
  // called from a timer interrupt as well as the main loop.
  // Also, because our ISR change the transfer_in_progress_ member variable.
  BlockInterrupts block;

  // Queue the request if possible: check that there is room in the index
  // buffer
  if (buffer_.FreeCount() <= 0) {
    return false;
  }

  // Add the current queue_ index into the buffer
  if (buffer_.Put(ind_queue_)) {
    queue_[ind_queue_] = request;
  } else {
    return false;
  }

  // In case of a write request, copy data to our write buffer
  if (request.direction == ExchangeDirection::kWrite) {
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
  if (++ind_queue_ >= kQueueLength) {
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
  if (write_buffer_index_ + size > kWriteBufferSize) {
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
    error_retry_ = kMaxRetries;
  }

  SetupI2CTransfer();
}

// Method called by interrupt handler when dma is disabled. This method
// transfers data to/from the tx/rx registers from/to *request.data
void Channel::TransferByte() {
  if (remaining_size_ == 0) {
    // this shouldn't happen, but just to be safe, we stop here.
    return;
  }
  if (last_request_.direction == ExchangeDirection::kRead) {
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

void Channel::EndTransfer() {
  // Ensure thread safety
  BlockInterrupts block;
  *last_request_.processed = true;
  if (last_request_.direction == ExchangeDirection::kWrite) {
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

#if defined(BARE_STM32)
void STM32Channel::Init(I2C_Regs *i2c, DMA_Regs *dma, Speed speed) {
  i2c_ = i2c;

  // Disable I²C peripheral
  i2c_->ctrl1.peripheral_en = 0;

  // Set I²C speed using timing values from [RM] table 182
  i2c_->timing.r = static_cast<uint32_t>(speed);

  // Setup DMA channels
  if (dma != nullptr) {
    SetupDMAChannels(dma);
  }

  // enable I²C peripheral
  i2c_->ctrl1.peripheral_en = 1;

  // configure I²C interrupts
  i2c_->ctrl1.nack_interrupts = 1;
  i2c_->ctrl1.error_interrupts = 1;
  // in DMA mode, we do not treat the transfer-specific ones
  if (!dma_enable_) {
    i2c_->ctrl1.rx_interrupts = 1;
    i2c_->ctrl1.tx_interrupts = 1;
    i2c_->ctrl1.tx_complete_interrupts = 1;
  } else {
    i2c_->ctrl1.rx_interrupts = 0;
    i2c_->ctrl1.tx_interrupts = 0;
    i2c_->ctrl1.tx_complete_interrupts = 0;
  }
}

void STM32Channel::SetupI2CTransfer() {
  // set transfer-specific registers per [RM] p1149 to 1158
  i2c_->ctrl2.slave_addr_7b = last_request_.slave_address & 0x7f;
  i2c_->ctrl2.transfer_dir = static_cast<bool>(last_request_.direction);

  if (dma_enable_) {
    SetupDMATransfer();
  }

  WriteTransferSize();

  i2c_->ctrl2.start = 1;
}

// Write the remaining size to the appropriate register with reload logic
void STM32Channel::WriteTransferSize() {
  if (remaining_size_ <= 255) {
    i2c_->ctrl2.n_bytes = static_cast<uint8_t>(remaining_size_);
    i2c_->ctrl2.reload = 0;
  } else {
    i2c_->ctrl2.n_bytes = 255;
    // Transfer reload is not currently supported by our HAL in DMA mode,
    // we will treat a reload as a new transfer. In effect this means we
    // will have to reissue the I²C header and start condition.
    // It also means that any request that has a (functional) header inside
    // its data and is longer than 255 bytes may not be processed correctly
    // by the I²C slave, as it will be split (this is referred to as a
    // Restart condition rather than Reload)
    if (!dma_enable_) {
      i2c_->ctrl2.reload = 1;
    } else {
      i2c_->ctrl2.reload = 0;
    }
  }
}

// DMA functions are only meaningful in BARE_STM32
void STM32Channel::SetupDMAChannels(DMA_Regs *dma) {
  // DMA mapping for I²C (see [RM] p299)
  static struct {
    volatile void *dma;
    volatile void *i2c;
    DMA_Chan tx_channel;
    DMA_Chan rx_channel;
    uint8_t request;
  } DMAmap[] = {
      {DMA1_BASE, I2C1_BASE, DMA_Chan::C6, DMA_Chan::C7, 3},
      {DMA1_BASE, I2C2_BASE, DMA_Chan::C4, DMA_Chan::C5, 3},
      {DMA1_BASE, I2C3_BASE, DMA_Chan::C2, DMA_Chan::C3, 3},
      {DMA2_BASE, I2C1_BASE, DMA_Chan::C7, DMA_Chan::C6, 5},
      {DMA2_BASE, I2C4_BASE, DMA_Chan::C2, DMA_Chan::C1, 0},
  };
  for (auto &map : DMAmap) {
    if (dma == map.dma && i2c_ == map.i2c) {

      dma_ = dma;
      tx_channel_ = &dma_->channel[static_cast<uint8_t>(map.tx_channel)];
      rx_channel_ = &dma_->channel[static_cast<uint8_t>(map.rx_channel)];

      // Tell the STM32 that those two DMA channels are used for I2C
      DMA_SelectChannel(dma, map.rx_channel, map.request);
      DMA_SelectChannel(dma, map.tx_channel, map.request);

      // configure both DMA channels to handle I²C transfers
      ConfigureDMAChannel(rx_channel_, ExchangeDirection::kRead);
      ConfigureDMAChannel(tx_channel_, ExchangeDirection::kWrite);

      dma_enable_ = true;
      i2c_->ctrl1.dma_rx = 1;
      i2c_->ctrl1.dma_tx = 1;
      break;
    }
  }
}

void I2C::STM32Channel::ConfigureDMAChannel(
    volatile DMA_Regs::ChannelRegs *channel, ExchangeDirection direction) {
  channel->config.priority = 0b01; // medium priority
  channel->config.teie = 1;        // interrupt on error
  channel->config.htie = 0;        // no half-transfer interrupt
  channel->config.tcie = 1;        // interrupt on DMA complete
  channel->config.mem2mem = 0;     // memory-to-memory mode disabled
  channel->config.msize = static_cast<uint8_t>(DmaTransferSize::BITS8);
  channel->config.psize = static_cast<uint8_t>(DmaTransferSize::BITS8);
  channel->config.memInc = 1; // increment dest address
  channel->config.perInc = 0; // don't increment source address
  channel->config.circular = 0;
  if (direction == ExchangeDirection::kRead) {
    channel->config.dir =
        static_cast<uint8_t>(DmaChannelDir::PERIPHERAL_TO_MEM);
    channel->pAddr = &(i2c_->rxData);
  } else {
    channel->config.dir =
        static_cast<uint8_t>(DmaChannelDir::MEM_TO_PERIPHERAL);
    channel->pAddr = &(i2c_->txData);
  }
}

void STM32Channel::SetupDMATransfer() {
  if (!dma_enable_) {
    return;
  }
  // to be on the safe size, disable both channels in case they weren't
  // (likely when this is called as a "retry after error")
  rx_channel_->config.enable = 0;
  tx_channel_->config.enable = 0;

  volatile DMA_Regs::ChannelRegs *channel{nullptr};
  if (last_request_.direction == ExchangeDirection::kRead) {
    channel = rx_channel_;
  } else {
    channel = tx_channel_;
  }

  channel->mAddr = next_data_;

  if (remaining_size_ <= 255) {
    channel->count = remaining_size_;
  } else {
    channel->count = 255;
  }

  // when using DMA, we need to use autoend, otherwise the STOP condition
  // which we issue at the end of the DMA transfer (which means the last byte
  // has been written to the register) may arrive before the last byte is
  // actually written on the line. Tests with both DMA and I2C interrupts
  // enabled to send Stop at the end of the I2C transfer were inconclusive.
  i2c_->ctrl2.autoend = 1;

  channel->config.enable = 1;
}

void STM32Channel::DMAIntHandler(DMA_Chan chan) {
  if (!dma_enable_ || !transfer_in_progress_)
    return;
  dma_->channel[static_cast<uint8_t>(chan)].config.enable = 0;
  if (DMA_IntStatus(dma_, chan, DmaInterrupt::XFER_COMPLETE)) {
    if (remaining_size_ > 255) {
      // decrement remaining size by 255 (the size of the DMA transfer)
      remaining_size_ = static_cast<uint16_t>(remaining_size_ - 255);
      next_data_ += 255 * sizeof(uint8_t);
    } else {
      remaining_size_ = 0;
      EndTransfer();
    }
  } else if (DMA_IntStatus(dma_, chan, DmaInterrupt::XFER_ERR)) {
    // we are dealing with an error --> reset transfer (up to kMaxRetries
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
  DMA_ClearInt(dma_, chan, DmaInterrupt::GLOBAL);
  StartTransfer();
}
#endif // BARE_STM32

} // namespace I2C
