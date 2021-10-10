/* Copyright 2020-2021, RespiraWorks

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

#if !defined(BARE_STM32)

#include <cstring>

#include "hal.h"

uint32_t HalApi::GetCpuFreq() { return 0; }
void HalApi::EarlyInit() {}
void HalApi::Init() {}
void HalApi::init_PCB_ID_pins() {}
void HalApi::InitUARTs() {}

uint16_t HalApi::SerialRead(char *buf, uint16_t len) { return serial_port_.Read(buf, len); }
uint16_t HalApi::SerialBytesAvailableForRead() { return serial_port_.BytesAvailableForRead(); }
uint16_t HalApi::SerialWrite(const char *buf, uint16_t len) { return serial_port_.Write(buf, len); }
uint16_t HalApi::SerialBytesAvailableForWrite() { return serial_port_.BytesAvailableForWrite(); }
uint16_t HalApi::TESTSerialGetOutgoingData(char *data, uint16_t len) {
  return serial_port_.GetOutgoingData(data, len);
}
void HalApi::TESTSerialPutIncomingData(const char *data, uint16_t len) {
  serial_port_.PutIncomingData(data, len);
}

uint16_t HalApi::DebugRead(char *buf, uint16_t len) { return debug_serial_port_.Read(buf, len); }
uint16_t HalApi::DebugBytesAvailableForRead() { return debug_serial_port_.BytesAvailableForRead(); }
uint16_t HalApi::DebugWrite(const char *buf, uint16_t len) {
  return debug_serial_port_.Write(buf, len);
}
uint16_t HalApi::DebugBytesAvailableForWrite() {
  return debug_serial_port_.BytesAvailableForWrite();
}
uint16_t HalApi::TESTDebugGetOutgoingData(char *data, uint16_t len) {
  return debug_serial_port_.GetOutgoingData(data, len);
}
void HalApi::TESTDebugPutIncomingData(const char *data, uint16_t len) {
  debug_serial_port_.PutIncomingData(data, len);
}

uint16_t TestSerialPort::Read(char *buf, uint16_t len) {
  if (incoming_data_.empty()) {
    return 0;
  }
  auto &read_buffer = incoming_data_.front();
  uint16_t n = std::min(len, static_cast<uint16_t>(read_buffer.size()));
  memcpy(buf, read_buffer.data(), n);
  read_buffer.erase(read_buffer.begin(), read_buffer.begin() + n);
  if (read_buffer.empty()) {
    incoming_data_.pop_front();
  }
  return n;
}
uint16_t TestSerialPort::BytesAvailableForRead() {
  return incoming_data_.empty() ? 0 : static_cast<uint16_t>(incoming_data_.front().size());
}
uint16_t TestSerialPort::Write(const char *buf, uint16_t len) {
  uint16_t n = std::min(len, BytesAvailableForWrite());
  outgoing_data_.insert(outgoing_data_.end(), buf, buf + n);
  return n;
}
uint16_t TestSerialPort::BytesAvailableForWrite() {
  // TODO: Simulate partial writes?  For now, simply return the true size of
  // the Arduino tx buffer.
  return 64;
}
uint16_t TestSerialPort::GetOutgoingData(char *data, uint16_t len) {
  uint16_t n = std::min(len, static_cast<uint16_t>(outgoing_data_.size()));
  memcpy(data, outgoing_data_.data(), n);
  outgoing_data_.erase(outgoing_data_.begin(), outgoing_data_.begin() + n);
  return n;
}
void TestSerialPort::PutIncomingData(const char *data, uint16_t len) {
  constexpr uint16_t max_message_size = 64;
  while (len > max_message_size) {
    incoming_data_.push_back(std::vector<char>(data, data + max_message_size));
    data += max_message_size;
    len = static_cast<uint16_t>(len - max_message_size);
  }
  incoming_data_.push_back(std::vector<char>(data, data + len));
}

void HalApi::StartLoopTimer(const Duration &period, void (*callback)(void *), void *arg) {}

#endif
