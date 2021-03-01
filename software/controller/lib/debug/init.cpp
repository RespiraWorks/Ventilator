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

#include "commands.h"
#include "interface.h"

// Create a trace buffer that the Debug Handler uses to handle the trace
// command.
Debug::Trace trace;

// These variables are used to control the trace function
static FnDebugVar varTraceCtrl(
    VarType::UINT32, "trace_ctrl", "Used to start/stop the trace function",
    "0x%08x", [] { return static_cast<uint32_t>(trace.GetStatus()); },
    [](uint32_t value) {
      if (value == 1)
        trace.Start();
      else
        trace.Stop();
    });

static FnDebugVar varTracePeriod(
    VarType::UINT32, "trace_period",
    "Period that data will be captured.  Loop cycle units", "%u",
    [] { return trace.GetPeriod(); },
    [](uint32_t value) { trace.SetPeriod(value); });

static FnDebugVar varTraceSamp(
    VarType::UINT32, "trace_samples", "Number of trace samples saved so far",
    "%u", [] { return static_cast<uint32_t>(trace.GetNumSamples()); },
    [](uint32_t value) { /*ignored*/ });

static FnDebugVar vartrace_var1(
    VarType::INT32, "trace_var1", "Variable to be saved to the trace buffer",
    "%d", [] { return trace.GetTracedVarId<0>(); },
    [](int32_t value) { trace.SetTracedVarId<0>(value); });
static FnDebugVar vartrace_var2(
    VarType::INT32, "trace_var2", "Variable to be saved to the trace buffer",
    "%d", [] { return trace.GetTracedVarId<1>(); },
    [](int32_t value) { trace.SetTracedVarId<1>(value); });
static FnDebugVar vartrace_var3(
    VarType::INT32, "trace_var3", "Variable to be saved to the trace buffer",
    "%d", [] { return trace.GetTracedVarId<2>(); },
    [](int32_t value) { trace.SetTracedVarId<2>(value); });
static FnDebugVar vartrace_var4(
    VarType::INT32, "trace_var4", "Variable to be saved to the trace buffer",
    "%d", [] { return trace.GetTracedVarId<3>(); },
    [](int32_t value) { trace.SetTracedVarId<3>(value); });

// Create a handler for each of the known commands that the Debug Handler can
// link to.  This is a bit tedious but I can't find a simpler way.
Debug::Command::ModeHandler mode_command;
Debug::Command::PeekHandler peek_command;
Debug::Command::PokeHandler poke_command;
Debug::Command::VarHandler var_command;
Debug::Command::TraceHandler trace_command(&trace);

namespace Debug {

Interface::Interface() {
  // TODO - This is annoying. Could we make the Command Handler automatically
  // add themselves to this registry?
  registry_[static_cast<uint8_t>(Command::Code::kMode)] = &mode_command;
  registry_[static_cast<uint8_t>(Command::Code::kPeek)] = &peek_command;
  registry_[static_cast<uint8_t>(Command::Code::kPoke)] = &poke_command;
  registry_[static_cast<uint8_t>(Command::Code::kVariable)] = &var_command;
  registry_[static_cast<uint8_t>(Command::Code::kTrace)] = &trace_command;

  trace_ = &trace;
}

} // namespace Debug
