/* Copyright 2020, Edwin Chiu

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

#ifndef COMMS_H
#define COMMS_H

#include "network_protocol.pb.h"
#include <stdint.h>

void comms_init();
void comms_handler();
void comms_sendFlow(float flow);
void comms_sendPressure(float pressure);
void comms_sendVolume(float volume);
void comms_sendResetState();
void comms_sendControllerStatus(ControllerStatus);

#ifdef TEST_MODE
// Sets an alternative callback to be run when receiving a command over the
// network.
//
// Pass a null handler to restore the default handler.
void comms_test_set_command_handler(void (*handler)(Command &cmd));
#endif

#endif // COMMS_H
