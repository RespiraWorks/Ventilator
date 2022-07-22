/* Copyright 2022, RespiraWorks

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

#pragma once

#include "spi_chain.h"

namespace SPI {

template <size_t MaxSlaves>
class MockChain : public DaisyChain<MaxSlaves> {
public:
  MockChain(size_t num_slaves, uint8_t null_command)
    : DaisyChain<MaxSlaves>(Base::SPI1, DMA::Base::DMA1) {
    
    // Set stuff that is usually done in Initialize function
    // Note we are using "this->" for inherited members because we are subclassing from a template
    // (see https://tinyurl.com/4ejssh23)
    this->null_command_ = null_command;
    this->rx_listener_ = this;
    SetNumSlaves(num_slaves);
  }
  
  void SetupReception(uint8_t *receive_buffer, size_t length) override {
    dma_rx_buffer_ = receive_buffer;
    dma_buffer_length_ = length;
    dma_buffer_index_ = 0;
  }

  void SendCommand(uint8_t *send_buffer, size_t length, bool clear_chip_select) override {
    for(size_t i = 0 ; i < length ; i++){
      if(spi_flow_ && spi_flow_index_ < max_flow_length_){
        spi_flow_[spi_flow_index_++] = send_buffer[i];
      }
      if(this->num_slaves_==0){
        // if there are no slaves, the daisy chain is considered broken, the spi flow never comes
        // back to the bus master.
        continue;
      }
      // advance the spi flow
      std::optional<uint8_t> val = mock_chain_.Get();
      dma_rx_buffer_[dma_buffer_index_++] = *val;
      [[maybe_unused]] bool done = mock_chain_.Put(send_buffer[i]);
    }
    if(this->num_slaves_>0 && !manual_interrupts_){
      // For all use cases pertaining to a DaisyChain, send and receive buffer have the same length,
      // so we always call on_rx_complete after having copied the send buffer (if we did). 
      this->rx_listener_->on_rx_complete();
    }
  }
  
  void SetChipSelect() override {};
  void ClearChipSelect() override {};

  // Allows us to enqueue requests and not send any byte, sending requests simultaneously to
  // several slaves for example.
  void BlockTransmission() {
    this->transmitting_data_ = true;
  }
  
  // Start the transmission, optionally one byte per slave at a time if manual_interrupts is set.
  void StartTransmission(bool manual_interrupts = false){
    manual_interrupts_ = manual_interrupts;
    this->TransmitNextCommand();
  }

  // Setup the chain to send one byte per slave at a time. 
  void BlockInterrupts(){
    manual_interrupts_ = true;
  }

  // Transmits the next set of bytes (one per slave). Necessary when blocking interrupts.
  // Returns false if there is nothing to transmit.
  bool TransmitNextBytes(){
    this->on_rx_complete();
    return this->transmitting_data_;
  }

  size_t max_queue_length(){
    return this->MaxQueueLength;
  }

  size_t FreeCommandBuffer(){
    return this->CommandBufferSize - this->command_buffer_count_;
  }

  // Copies the last value received by each slave to a user-provided array
  void GetChainState(uint8_t *destination, size_t length){
    if(length < this->num_slaves_){
      return;
    }
    for(size_t i = 0 ; i < this->num_slaves_ ; i++){
      std::optional<uint8_t> val = mock_chain_.Get();
      destination[i] = *val;
      // Get removes the value from the circular buffer, we have to put it back.
      // Since we do this with the exact number of values currently in the buffer,
      // the order of the elements will be restored at the end.
      [[maybe_unused]] bool done = mock_chain_.Put(*val);
    }
  }

  // Method used to instruct the daisy chain to export its spi data to a client-provided buffer.
  // Call with max_length = 0 or with buffer=nullptr to stop the instrumentation.
  void ExportSpiFlow(uint8_t *buffer, size_t max_length){
    spi_flow_ = buffer;
    max_flow_length_ = max_length;
    spi_flow_index_ = 0;
  }
  
  size_t TestProbeSlaves(size_t num_slaves, uint8_t null_command, uint8_t reset_command){
    SetNumSlaves(num_slaves);
    this->ProbeSlaves(null_command, reset_command);
    return this->num_slaves_;
  }

private:
  // Buffer where the client asked us to export the spi flow as we transmit it
  uint8_t *spi_flow_{nullptr};
  size_t max_flow_length_{0};
  size_t spi_flow_index_{0};

  // Buffer where simulated dma transmits data
  uint8_t *dma_rx_buffer_{nullptr};
  size_t dma_buffer_length_{0};
  size_t dma_buffer_index_{0};

  // 
  bool manual_interrupts_{false};

  // This circular buffer represents the last value received by each slave (in order),
  // acting as a shift register.
  CircularBuffer<uint8_t, MaxSlaves> mock_chain_;

  void WaitResponse() override {}

  void SetNumSlaves(size_t num_slaves){
    if(this->num_slaves_>num_slaves){
      // Removing slaves, clear as many values from the mock chain 
      for( size_t i = 0 ; i < this->num_slaves_-num_slaves ; i++){
        [[maybe_unused]] std::optional<uint8_t> val = mock_chain_.Get();
      }
    } else {
      // Adding slaves, add as many values (set to 0) to the mock chain
      for( size_t i = 0 ; i < num_slaves-this->num_slaves_ ; i++){
        [[maybe_unused]] bool check = mock_chain_.Put(0);
      } 
    }
    this->num_slaves_ = num_slaves;
  }

};

} // namespace SPI
