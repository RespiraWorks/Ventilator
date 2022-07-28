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

namespace SPI {

template<size_t QueueLength>
bool RequestQueue<QueueLength>::AddRequest(const Request &request, uint8_t *command_buffer){
  if (queue_count_ >= QueueLength || request.command == nullptr || request.length == 0 || !command_buffer){
    return false;
  }

  Request *new_request = &(queue_[queue_count_++]);
  new_request->command = command_buffer;
  new_request->length = request.length;
  new_request->response = request.response;
  new_request->processed = request.processed;

  return true;
}

template<size_t QueueLength>
bool RequestQueue<QueueLength>::WriteNextResponseByte(uint8_t byte){
  Request *request = GetCurrentRequest();
  if(!request) {
    return false;
  }

  bool byte_written{false};
  // We only record the response if the caller cares about it, and provided a proper pointer
  // where we can put the response
  if(request->response && save_response_) {
    // Update the response with received data
    request->response[response_count_++] = byte;
    byte_written=true;
  }

  if(end_of_request_){
    // inform the caller that the request is processed
    if(request->processed) {
      *(request->processed) = true;
    }
    AdvanceToNextRequest();
  }
  return byte_written;
};

template<size_t QueueLength>
std::optional<uint8_t> RequestQueue<QueueLength>::GetNextCommandByte(){
  Request *request = GetCurrentRequest();
  if(!request) {
    return std::nullopt;
  }
  std::optional<uint8_t> byte = request->command[command_index_++];

  // Trigger saving the response when we are sending the second byte. This allows us to discard
  // the first byte of the response, which is actually a remnant of a previous state.
  if(command_index_ >= 2){
    save_response_ = true;
  }

  // Check whether we are at the end of the current request, and pass the info to
  // WriteNextResponseByte which will update the states after having saved the last byte of the
  // response.
  if(command_index_ >= request->length) {
    end_of_request_ = true;
  }

  return byte;
}

template<size_t QueueLength>
void RequestQueue<QueueLength>::AdvanceToNextRequest() {
  // Carry on to the next request (if any)
  if(++current_request_ >= queue_count_){
    // We reached the end of the queue
    queue_count_ = 0;
    current_request_ = 0;
  }
  // reset all request-related states
  command_index_ = 0;
  response_count_ = 0;
  save_response_ = false;
  end_of_request_ = false;
}

template<size_t QueueLength>
Request *RequestQueue<QueueLength>::GetCurrentRequest(){
  if(current_request_ < queue_count_){
    return &(queue_[current_request_]);
  }
  return nullptr;
}

template<size_t MaxSlaves, size_t MaxRequestsPerSlave>
DaisyChain<MaxSlaves, MaxRequestsPerSlave>::DaisyChain(const char* name, const char* help_supplement, Channel *spi, Duration min_cs_high_time)
   : min_cs_high_time_(min_cs_high_time), spi_(spi)
  {
    dbg_queueing_errors_.prepend_name(name);
    dbg_queueing_errors_.append_help(help_supplement);
   }

template<size_t MaxSlaves, size_t MaxRequestsPerSlave>
size_t DaisyChain<MaxSlaves, MaxRequestsPerSlave>::ProbeSlaves(uint8_t null_command, uint8_t reset_command) {
  // Remember the null_command for later
  SetNullCommand(null_command);
  FlushSlavesData();

  // Send a reset command to all the slaves + 1, in order to capture the case where we have no
  // slaves at all (and the response buffer will stay full of zeros).
  uint8_t response_buffer[MaxSlaves+1] = {0};
  ResetSlaves(reset_command, response_buffer, sizeof(response_buffer));

  // for testing purposes, this returns the number of slaves
  return ParseProbeResponse(response_buffer, sizeof(response_buffer));;
}

template<size_t MaxSlaves, size_t MaxRequestsPerSlave>
void DaisyChain<MaxSlaves, MaxRequestsPerSlave>::SetNullCommand(uint8_t null_command){
  null_command_ = null_command;
};

template<size_t MaxSlaves, size_t MaxRequestsPerSlave>
void DaisyChain<MaxSlaves, MaxRequestsPerSlave>::FlushSlavesData(){
  // Send a bunch of null commands in order to flush all of the data in the daisy chain and any data
  // currently being returned by the slaves (in case the controller was halted in a weird state).
  // This assumes the longest response we can expect for an eventual ongoing command is less than 5
  // bytes long.
  uint8_t flush_buffer[MaxSlaves*5] = {0};
  for(size_t i = 0 ; i < sizeof(flush_buffer) ; i++){
    flush_buffer[i] = null_command_;
  }
  uint8_t response_buffer[sizeof(flush_buffer)] = {0};
  SendDataWithBusyWait(flush_buffer, response_buffer, sizeof(flush_buffer));
};

//
template<size_t MaxSlaves, size_t MaxRequestsPerSlave>
void DaisyChain<MaxSlaves, MaxRequestsPerSlave>::ResetSlaves(uint8_t reset_command, uint8_t *response_buffer, size_t length){
  uint8_t reset_buffer[length] = {0};
  for(size_t i = 0 ; i < sizeof(reset_buffer) ; i++){
    reset_buffer[i] = reset_command;
  }
  SendDataWithBusyWait(reset_buffer, response_buffer, length);
};

template<size_t MaxSlaves, size_t MaxRequestsPerSlave>
size_t DaisyChain<MaxSlaves, MaxRequestsPerSlave>::ParseProbeResponse(uint8_t *response_buffer, size_t length) {
  // At this point, the response buffer should be filled with as many null commands as there are
  // slaves. Parse it to determine how many slaves there are.
  num_slaves_ = 0;
  for(size_t i = 0 ; i < length ; i++){
    if (response_buffer[i] == null_command_){
      num_slaves_++;
    }
  }

  // If the response buffer is full of null commands, this most likely means the
  // daisy chain is broken (more slaves than we expect)
  if(num_slaves_==sizeof(response_buffer)) {
    num_slaves_ = 0;
  }
  return num_slaves_;
}

template<size_t MaxSlaves, size_t MaxRequestsPerSlave>
bool DaisyChain<MaxSlaves, MaxRequestsPerSlave>::SendRequest(const Request &request, size_t slave) {
  // Ensure thread safety as this function might be called from a timer interrupt
  // as well as the main loop.
  BlockInterrupts block;
  // Check that the slave is valid, and there is room in the command buffer
  if(slave >= num_slaves_ || request.length > CommandBufferSize - command_buffer_count_){
    /// \TODO log an error (along with its nature?)
    queueing_errors_++;
    return false;
  }

  // Copy request to the request queue for that slave
  if(!request_queue_[slave].AddRequest(request, &command_buffer_[command_buffer_count_])){
    queueing_errors_++;
    return false;
  }
  // Copy data to the command buffer
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

template<size_t MaxSlaves, size_t MaxRequestsPerSlave>
void DaisyChain<MaxSlaves, MaxRequestsPerSlave>::SendDataWithBusyWait(uint8_t* command_buffer, uint8_t* response_buffer, size_t length){
  if(!command_buffer || !response_buffer){
    return;
  }

  // Blocking interrupts in order to force a busy wait until the end of the command
  BlockInterrupts block;

  spi_->SetupReception(response_buffer, length);
  spi_->SendCommand(command_buffer, length, true);

  spi_->WaitResponse();

  // NOTE - CS has to be high between bytes (how long depends on the nature of the slaves).
  spi_->SetChipSelect();
  SystemTimer::singleton().delay(min_cs_high_time_);
}

template<size_t MaxSlaves, size_t MaxRequestsPerSlave>
void DaisyChain<MaxSlaves, MaxRequestsPerSlave>::on_rx_complete(){
  spi_->SetChipSelect();
  last_cs_rise_=SystemTimer::singleton().now();
  ProcessReceivedData();
  TransmitNextCommand();
  // Slight delay here allows us to not halt the controller when driving steppers.
  /// \TODO investigate the reason we need this.
  SystemTimer::singleton().delay(microseconds(1));
}

template<size_t MaxSlaves, size_t MaxRequestsPerSlave>
void DaisyChain<MaxSlaves, MaxRequestsPerSlave>::ProcessReceivedData() {
  for(size_t slave = 0 ; slave < num_slaves_ ; slave++) {
    request_queue_[slave].WriteNextResponseByte(receive_buffer_[slave]);
  }
}

template<size_t MaxSlaves, size_t MaxRequestsPerSlave>
void DaisyChain<MaxSlaves, MaxRequestsPerSlave>::TransmitNextCommand() {
  // This method does not need to block interrupts: it is called from either SendRequest, which
  // blocks interrupts or from RxDMAInterruptHandler (through on_rx_complete), which is only
  // possible after SetupReception and SendCommand (our last instructions) have been called.
  transmitting_data_ = true;

  // This will get set to true if I find any data to send
  bool data_to_send = false;

  for(size_t slave = 0 ; slave < num_slaves_ ; slave++) {
    std::optional<uint8_t> byte = request_queue_[slave].GetNextCommandByte();
    send_buffer_[slave] = byte.value_or(null_command_);
    if(byte.has_value()){
      data_to_send = true;
    }
  }

  if(data_to_send){
    spi_->SetupReception(receive_buffer_, num_slaves_);
    EnsureMinCSHighTime();
    // Send command with CS low
    spi_->SendCommand(send_buffer_, num_slaves_, true);
  } else {
    // reset the command buffer
    command_buffer_count_ = 0;
    // wait for the next command, which will trigger a new transmission
    transmitting_data_ = false;
  }
}

template<size_t MaxSlaves, size_t MaxRequestsPerSlave>
void DaisyChain<MaxSlaves, MaxRequestsPerSlave>::EnsureMinCSHighTime(){
  // In case cs has not been high for long enough, delay sending the command
    Time now = SystemTimer::singleton().now();
    if(now - last_cs_rise_ < min_cs_high_time_) {
      SystemTimer::singleton().delay(now - last_cs_rise_);
    }
}

} //namespace SPI
