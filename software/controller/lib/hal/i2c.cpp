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
// This file contains code to setup, send and recieve data on the i2c channel
// See header file for implementation philosophy.
//
////////////////////////////////////////////////////////////////////

#if defined(BARE_STM32)

#include "i2c.h"
#include "hal.h"
#include "hal_stm32.h"

I2CChannel i2c1;

// Reference abbreviations ([RM], [PCB], etc) are defined in hal/README.md
void HalApi::InitI2C() {
  // Enable I2C1 and DMA2 peripheral clocks (we use DMA2 to send/recieve data)
  EnableClock(I2C1_BASE);
  EnableClock(DMA2_BASE);

  // The following pins are used as i2c1 bus on the rev-1 PCB (see [PCB]):
  // - PB8 (I2C1 - DATA)
  // - PB9 (I2C1 - CLOCK)
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

  // init i2c1
  i2c1.Init(I2C1_BASE, DMA2_BASE, I2CSpeed::kFast);
}

// Those interrupt handlers are specific to our configuration, unlike the
// I2C::*ISR() which are generic ISR associated with an I2C channel
void I2C1_EV_ISR() { i2c1.I2CEventHandler(); };

void I2C1_ER_ISR() { i2c1.I2CErrorHandler(); };

void DMA2_CH6_ISR() { i2c1.DMAIntHandler(DMA_Chan::C6); };

void DMA2_CH7_ISR() { i2c1.DMAIntHandler(DMA_Chan::C7); };

void I2CChannel::Init(I2C_Regs *i2c, DMA_Regs *dma, I2CSpeed speed) {
  i2c_ = i2c;
  dma_ = dma;

  // Disable I2C peripheral
  i2c_->ctrl1.peripheral_en = 0;

  // Setup DMA channel
  // DMA mapping for I2C (see [RM] p299)
  static struct {
    void *dma;
    void *i2c;
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

  dma_enable_ = false;
  for (auto &map : DMAmap) {
    if (dma == map.dma && i2c == map.i2c) {
      tx_channel_ = map.tx_channel;
      rx_channel_ = map.rx_channel;

      DMA_SelectChannel(dma, rx_channel_, map.request);
      DMA_SelectChannel(dma, tx_channel_, map.request);

      dma_enable_ = true;
      break;
    }
  }

  // Set I2C speed using timing values from [RM] table 182
  switch (speed) {
  case I2CSpeed::kSlow:
    i2c_->timing.prescaler = 0x3;
    i2c_->timing.scl_delay = 0x4;
    i2c_->timing.sda_hold = 0x2;
    i2c_->timing.scl_high = 0xC3;
    i2c_->timing.scl_low = 0xC7;
    break;
  case I2CSpeed::kStandard:
    i2c_->timing.prescaler = 0x3;
    i2c_->timing.scl_delay = 0x4;
    i2c_->timing.sda_hold = 0x2;
    i2c_->timing.scl_high = 0xF;
    i2c_->timing.scl_low = 0x13;
    break;
  case I2CSpeed::kFast:
    i2c_->timing.prescaler = 0x1;
    i2c_->timing.scl_delay = 0x3;
    i2c_->timing.sda_hold = 0x2;
    i2c_->timing.scl_high = 0x3;
    i2c_->timing.scl_low = 0x9;
    break;
  case I2CSpeed::kFastPlus:
    i2c_->timing.prescaler = 0x0;
    i2c_->timing.scl_delay = 0x2;
    i2c_->timing.sda_hold = 0x0;
    i2c_->timing.scl_high = 0x2;
    i2c_->timing.scl_low = 0x4;
    break;
  }

  if (dma_enable_) {
    uint8_t rx_chan = static_cast<uint8_t>(rx_channel_);
    uint8_t tx_chan = static_cast<uint8_t>(tx_channel_);

    // set DMA channels to be used for this I2C channel
    dma_->channel[rx_chan].config.priority = 0b01; // medium priority
    dma_->channel[rx_chan].config.teie = 1;        // interrupt on error
    dma_->channel[rx_chan].config.htie = 0;        // no half-transfer interrupt
    dma_->channel[rx_chan].config.tcie = 1;        // interrupt on DMA complete
    dma_->channel[rx_chan].config.mem2mem = 0; // memory-to-memory mode disabled
    dma_->channel[rx_chan].config.msize =
        static_cast<uint8_t>(DmaTransferSize::BITS8);
    dma_->channel[rx_chan].config.psize =
        static_cast<uint8_t>(DmaTransferSize::BITS8);
    dma_->channel[rx_chan].config.memInc = 1; // increment dest address
    dma_->channel[rx_chan].config.perInc = 0; // don't increment source address
    dma_->channel[rx_chan].config.circular = 0;
    dma_->channel[rx_chan].config.dir =
        static_cast<uint8_t>(DmaChannelDir::PERIPHERAL_TO_MEM);

    dma_->channel[tx_chan].config.priority = 0b01; // medium priority
    dma_->channel[tx_chan].config.teie = 1;        // interrupt on error
    dma_->channel[tx_chan].config.htie = 0;        // no half-transfer interrupt
    dma_->channel[tx_chan].config.tcie = 1;    // DMA complete interrupt enable
    dma_->channel[tx_chan].config.mem2mem = 0; // memory-to-memory mode disabled
    dma_->channel[tx_chan].config.msize =
        static_cast<uint8_t>(DmaTransferSize::BITS8);
    dma_->channel[tx_chan].config.psize =
        static_cast<uint8_t>(DmaTransferSize::BITS8);
    dma_->channel[tx_chan].config.memInc = 1; // increment source address
    dma_->channel[tx_chan].config.perInc = 0; // don't increment dest address
    dma_->channel[tx_chan].config.circular = 0;
    dma_->channel[tx_chan].config.dir =
        static_cast<uint8_t>(DmaChannelDir::MEM_TO_PERIPHERAL);
  }

  // enable I2C peripheral
  i2c_->ctrl1.peripheral_en = 1;
}

bool I2CChannel::SendRequest(const I2CRequest request) {
  // queue the request if possible
  if (buffer_.Put(ind_queue_)) {
    queue_[ind_queue_] = request;
    if (++ind_queue_ >= kQueueLength) {
      ind_queue_ = 0;
    }
  } else {
    return false;
  }
  if (i2c_->status.busy == 0) {
    StartTransfer();
  }
  // if the bus is busy, the transfer will be initiated by the interrupt
  // handlers, our work is done!
  return true;
}

void I2CChannel::StartTransfer() {
  // A single request can lead to several transfers, when it is longer than 255
  // bytes. Therefore we need to check whether this call is a continuation of a
  // long request or a new request.
  // Also in case of transfer error, we may need to re-send the last request
  if (remaining_size_ <= 0) {
    // This indicates the last request has been successfully sent, hence we will
    // send the next request in the queue.
    std::optional<uint8_t> ind = buffer_.Get();
    if (ind == std::nullopt) {
      // trigger stop condition and end
      i2c_->ctrl2.stop = 1;
      return;
    }
    last_request_ = queue_[*ind];
    next_data_ = reinterpret_cast<uint8_t *>(last_request_.data);
    remaining_size_ = last_request_.size;
  }

  // Per [RM] p1149 to 1158
  // set transfer-specific registers
  i2c_->ctrl2.slave_addr_7b = last_request_.slave_address & 0x7f;
  i2c_->ctrl2.transfer_dir = static_cast<bool>(last_request_.read_write);

  if (remaining_size_ <= 255) {
    i2c_->ctrl2.n_bytes = static_cast<uint8_t>(remaining_size_);
    i2c_->ctrl2.reload = 0;
  } else {
    i2c_->ctrl2.n_bytes = 255;
    i2c_->ctrl2.reload = 1;
  }

  // configure interrupts
  if (last_request_.read_write == I2CExchangeDir::kWrite) {
    i2c_->ctrl1.nack_interrupts = 1;
  }
  i2c_->ctrl1.tx_complete_interrupts = 1;
  i2c_->ctrl1.error_interrupts = 1;

  // configure per byte transfer handling interrupts or DMA if enabled
  if (dma_enable_ == 0) {
    i2c_->ctrl2.autoend = 0;
    if (last_request_.read_write == I2CExchangeDir::kRead) {
      i2c_->ctrl1.rx_interrupts = 1;
      i2c_->ctrl1.tx_interrupts = 0;
    } else {
      i2c_->ctrl1.rx_interrupts = 0;
      i2c_->ctrl1.tx_interrupts = 1;
    }
  } else {
    i2c_->ctrl1.rx_interrupts = 0;
    i2c_->ctrl1.tx_interrupts = 0;
    uint8_t chan = 0;
    if (last_request_.read_write == I2CExchangeDir::kRead) {
      chan = static_cast<uint8_t>(rx_channel_);
      i2c_->ctrl1.dma_rx = 1;
    } else {
      chan = static_cast<uint8_t>(tx_channel_);
      i2c_->ctrl1.dma_tx = 1;
    }
    // to be on the safe size, disable the channel in case it wasn't
    // (likely when this is called as a "retry after error")
    dma_->channel[chan].config.enable = 0;

    dma_->channel[chan].mAddr = next_data_;
    dma_->channel[chan].pAddr = &i2c_->rxData;
    if (remaining_size_ <= 255) {
      i2c_->ctrl2.autoend = 1;
      dma_->channel[chan].count = remaining_size_;
    } else {
      i2c_->ctrl2.autoend = 0;
      dma_->channel[chan].count = 255;
    }

    dma_->channel[chan].config.enable = 1;
  }
  transfer_in_progress_ = true;
  error_retry_ = kMaxRetries;
  if (remaining_size_ == last_request_.size) {
    // we only need to send start on the first transfer of a request
    i2c_->ctrl2.start = 1;
  }
}

// Method called by interrupt handler when dma is disabled. This method
// transfers data to/from the tx/rx registers from/to *request.data
void I2CChannel::ByteTransfer() {
  if (remaining_size_ <= 0) {
    // this shouldn't happen, but just to be safe, we stop here.
    return;
  }
  if (last_request_.read_write == I2CExchangeDir::kRead) {
    *next_data_ = static_cast<uint8_t>(i2c_->rxData);
  } else {
    i2c_->txData = *next_data_;
  }
  if (--remaining_size_ > 0) {
    // increment next_data_ pointer only in case we are still expecting more
    // data to prevent unwarranted memory access
    next_data_ += sizeof(uint8_t);
  };
}

void I2CChannel::I2CEventHandler() {
  if (transfer_in_progress_) {
    if (i2c_->status.rx_not_empty || i2c_->status.tx_interrupt) {
      // carry on with the current transfer
      ByteTransfer();
    }
    if (i2c_->status.transfer_reload) {
      // Start the next transfer, which is part of the current request
      StartTransfer();
    }
    if (i2c_->status.transfer_complete) {
      *last_request_.processed = true;
      // start transfer to initiate the next request (or stop if there is none)
      StartTransfer();
    }
    if (i2c_->status.nack) {
      // clear the nack
      i2c_->intClr = 0x10;
      // the peripheral is non-responsive --> start the transfer anew
      next_data_ = reinterpret_cast<uint8_t *>(last_request_.data);
      remaining_size_ = last_request_.size;
      StartTransfer();
    }
  }
}

void I2CChannel::I2CErrorHandler() {
  // i2c error --> clear all error flags (except those that are SMBus only)
  i2c_->intClr = 0x720;
  // and restart the transfer up to error_retry_ times
  if (--error_retry_ > 0) {
    next_data_ = reinterpret_cast<uint8_t *>(last_request_.data);
    remaining_size_ = last_request_.size;
    StartTransfer();
  }
}

void I2CChannel::DMAIntHandler(DMA_Chan chan) {
  if (dma_enable_) {
    if (DMA_IntStatus(dma_, chan, DmaInterrupt::XFER_COMPLETE)) {
      if (remaining_size_ > 255) {
        // decrement remaining size by 255 (the size of the DMA transfer)
        remaining_size_ = static_cast<uint16_t>(remaining_size_ - 255);
      } else {
        // indicate the last request as processed
        *last_request_.processed = true;
        remaining_size_ = 0;
      }
    } else if (DMA_IntStatus(dma_, chan, DmaInterrupt::XFER_ERR)) {
      // we are dealing with an error --> reset transfer
      next_data_ = reinterpret_cast<uint8_t *>(last_request_.data);
      remaining_size_ = last_request_.size;
    }
    // clear all interrupts
    DMA_ClearInt(dma_, chan, DmaInterrupt::GLOBAL);
    // in all cases start a transfer, either to restart the last transfer after
    // an error or to start the next one (if there is one...)
    StartTransfer();
  }
}

#endif // BARE_STM32
