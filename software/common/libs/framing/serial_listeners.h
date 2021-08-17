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

#pragma once

enum class RxError { Unknown, Overrun, SerialFraming, DMA };

// An interface that gets called on serial events.
// NOTE: all callbacks are called from interrupt context!
class RxListener {
public:
  // Called when the ordered number of bytes is transferred from UART to memory
  // over DMA
  virtual void OnRxComplete() = 0;
  // Called on specified character reception
  virtual void OnCharacterMatch() = 0;
  // Called on RX errors
  virtual void OnRxError(RxError) = 0;
};

class TxListener {
public:
  // Called when the ordered number of bytes is transferred from memory to UART
  // over DMA
  virtual void OnTxComplete() = 0;
  // Called on TX errors
  virtual void OnTxError() = 0;
};
