#ifndef __COMMAND_HANDLERS_H
#define __COMMAND_HANDLERS_H
#include "network_protocol.pb.h"

void command_handler(Command);
void ack_handler(GuiAck);

#endif