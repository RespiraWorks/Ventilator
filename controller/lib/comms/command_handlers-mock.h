#ifndef __COMMAND_HANDLERS_H
#define __COMMAND_HANDLERS_H
#include "network_protocol.pb.h"

extern void command_handler(Command);
extern void gui_ack_handler(GuiAck);

#endif