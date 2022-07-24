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

////////////////////////////////////////////////////////////////////////////////
/// WARNING: Do not include this file as header, nor compile it as a source! ///
///                                                                          ///
/// This is a template implementation file.                                  ///
/// It is included by the corresponding header.                              ///
////////////////////////////////////////////////////////////////////////////////

template<size_t MaxSlaves>
DaisyChain<MaxSlaves>::DaisyChain(Base spi, DMA::Base dma, const char* name, const char* help_supplement)
   : Channel(spi, dma)
  {
    queueing_errors_.prepend_name(name);
    queueing_errors_.append_help(help_supplement);
   }

template<size_t MaxSlaves>
void DaisyChain<MaxSlaves>::Initialize(uint8_t null_command, uint8_t reset_command,
                                       GPIO::Port clock_port, uint8_t clock_pin,
                                       GPIO::Port miso_port, uint8_t miso_pin,
                                       GPIO::Port mosi_port, uint8_t mosi_pin,
                                       GPIO::Port chip_select_port, uint8_t chip_select_pin,
                                       GPIO::Port reset_port, uint8_t reset_pin,
                                       uint8_t word_size, Bitrate bitrate) {
  Channel::Initialize(clock_port, clock_pin, miso_port, miso_pin, mosi_port, mosi_pin,
                      chip_select_port, chip_select_pin, reset_port, reset_pin, word_size,
                      bitrate, /*rx_interrupts_enabled=*/true,
                      /*tx_interrupts_enabled=*/false, /*rx_listener=*/this);
  null_command_=null_command;
  ProbeSlaves(null_command, reset_command);
}

template<size_t MaxSlaves>
bool DaisyChain<MaxSlaves>::SendRequest(const Request &request, size_t slave) {
  // Ensure thread safety as this function might be called from a timer interrupt
  // as well as the main loop.
  BlockInterrupts block;
  // Check that everything is in order to proceed: the slave is valid, there is room in the
  // request queue for that slave and in the command buffer, and the provided data looks OK
  if(slave >= num_slaves_ || queue_count_[slave] >= MaxQueueLength ||
       request.length > CommandBufferSize - command_buffer_count_ ||
       request.command == nullptr || request.length == 0){
    /// \TODO log an error (along with its nature?)
    queueing_errors_.set(queueing_errors_.get() + 1);
    return false;
  }

  // copy request to the request queue for that slave
  Request *new_request = &(request_queue_[slave][queue_count_[slave]++]);
  new_request->command = &command_buffer_[command_buffer_count_];
  new_request->length = request.length;
  new_request->response = request.response;
  new_request->processed = request.processed;
  // copy data to the command buffer
  for(size_t i = 0 ; i < request.length ; i++){
    command_buffer_[command_buffer_count_++] = request.command[i];
  }

  // Just in case the caller thinks the request is already processed, while it clearly isn't
  if(request.processed){
    *(request.processed) = false;
  }

  if(!transmitting_data_) {
    TransmitNextCommand();
  }
  return true;
}

template<size_t MaxSlaves>
void DaisyChain<MaxSlaves>::ProbeSlaves(uint8_t null_command, uint8_t reset_command) {
  // We are using a buffer slightly larger than normal here in order to detect the case where the
  // daisy chain is broken (with probe response being full of zeroes).
  // We are first sending a bunch of null commands in order to flush all of the data in the daisy
  // chain and any data currently being returned by the slaves (in case the controller was halted
  // in a weird state).
  uint8_t probe_buffer[MaxSlaves + 1] = {null_command};
  uint8_t response_buffer[sizeof(probe_buffer)] = {0};
  for (uint8_t i = 0; i < 5; i++) {
    SendDataWithBusyWait(probe_buffer, response_buffer, sizeof(probe_buffer));
  }

  // Send a reset command to all the slaves.
  for(size_t byte=0 ; byte < sizeof(probe_buffer) ; byte++){
    probe_buffer[byte] = reset_command;
  }
  SendDataWithBusyWait(probe_buffer, response_buffer, sizeof(probe_buffer));

  // At this point, the response buffer should be filled with as many null commands as there are
  // slaves.
  num_slaves_ = 0;
  for (uint8_t i : response_buffer) {
    if (i == null_command){
      num_slaves_++;
    } else {
      break;
    }
  }

  // If the response buffer is full of null commands, this most likely means the
  // daisy chain is broken (more slaves than we expect)
  if(num_slaves_==sizeof(response_buffer)) {
    num_slaves_ = 0;
  }

}

template<size_t MaxSlaves>
void DaisyChain<MaxSlaves>::SendDataWithBusyWait(uint8_t* command_buffer, uint8_t* response_buffer,
                                                 size_t length){
  // Blocking interrupts in order to force a busy wait until the end of the command
  BlockInterrupts block;

  SetupReception(response_buffer, length);
  SendCommand(command_buffer, length, true);

  WaitResponse();

  // NOTE - CS has to be high for at least 650ns between bytes.
  SetChipSelect();
  SystemTimer::singleton().delay(microseconds(1));
}


// This must only called with interrupts disabled.
template<size_t MaxSlaves>
void DaisyChain<MaxSlaves>::WaitResponse(){
  // Wait until transmission is finished and clear corresponding interrupt flag
  while (!rx_dma_->InterruptStatus(DMA::Interrupt::TransferComplete)) {
  }

  // Clear the interrupt flag so I won't get an interrupt as soon as I re-enable them
  rx_dma_->ClearInterrupt(DMA::Interrupt::Global);
}

template<size_t MaxSlaves>
void DaisyChain<MaxSlaves>::on_rx_complete(){
  // NOTE - CS has to be high for at least 650ns between bytes.
  // We don't include a busy wait here because in practice, the seting up of the next transmission
  // takes longer than that.
  SetChipSelect();
  ProcessReceivedData();
  TransmitNextCommand();
}

template<size_t MaxSlaves>
void DaisyChain<MaxSlaves>::ProcessReceivedData() {
  for(size_t slave = 0 ; slave < num_slaves_ ; slave++) {
    if(current_request_[slave] < queue_count_[slave]) {
      // We are processing a request for that slave.  Get that request handy.
      Request *request = &request_queue_[slave][current_request_[slave]];

      // We only record the response if the caller cares about it, and provided proper pointers
      // where we can put the response and inform him it is valid
      if(request->response && request->processed && save_response_[slave]) {
        // Update the response with received data from that slave
        request->response[response_count_[slave]++] = receive_buffer_[slave];
      }
      if(end_of_request_[slave]){
        // inform the caller that the request is processed
        if(request->processed) {
          *(request->processed) = true;
        }
        // reset all request-related states
        response_count_[slave] = 0;
        command_index_[slave] = 0;
        save_response_[slave] = false;
        end_of_request_[slave] = false;
        // and carry on to the next request (if any)
        if(++current_request_[slave] >= queue_count_[slave]){
          // We reached the end of the request queue for that slave
          queue_count_[slave] = 0;
          current_request_[slave] = 0;
        }
      }
    }
  }
}

template<size_t MaxSlaves>
void DaisyChain<MaxSlaves>::TransmitNextCommand() {
  // This method does not need to block interrupts: it is called from either SendRequest, which
  // blocks interrupts or from RxDMAInterruptHandler (through on_rx_complete), which is only
  // possible after SetupReception and SendCommand (our last instructions) have been called.
  transmitting_data_ = true;
  // This will get set to true if I find any data to send
  bool data_to_send = false;

  for(size_t slave = 0 ; slave < num_slaves_ ; slave++) {
    // prepare data to be sent for each slave
    if(current_request_[slave] < queue_count_[slave]){
      data_to_send = true;
      Request *request = &request_queue_[slave][current_request_[slave]];
      send_buffer_[slave] = request->command[command_index_[slave]++];

      // Trigger saving the response when we just sent the second byte.
      // This allows us to discard the first byte of the response.
      if(command_index_[slave] == 2){
        save_response_[slave] = true;
      }

      // Check whether we are at the end of the current request, and pass the info to
      // ProcessReceivedData which will update the states after having saves the last byte of the
      // response, which is actually a remnant of a previous state.
      if(command_index_[slave] >= request->length) {
        end_of_request_[slave] = true;
      }
    } else {
      send_buffer_[slave] = null_command_;
    }
  }

  if(data_to_send){
    SetupReception(receive_buffer_, num_slaves_);
    // Send command with CS low
    SendCommand(send_buffer_, num_slaves_, true);
  } else {
    command_buffer_count_ = 0;
    // wait for the next command, which will trigger a new transmission
    transmitting_data_ = false;
  }
}
