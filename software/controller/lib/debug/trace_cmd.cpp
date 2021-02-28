
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

namespace Debug::Command {

ErrorCode TraceHandler::Process(Context *context) {

  // The first byte of data is always required, this
  // gives the sub-command.
  if (context->request_length < 1)
    return ErrorCode::kMissingData;

  switch (context->request[0]) {

  // Sub-command 0 is used to flush the trace buffer
  // It also disables the trace
  case 0: {
    trace_->Stop();
    context->response_length = 0;
    return ErrorCode::kNone;
  }

  // Sub-command 1 is used to read data from the buffer
  case 1:
    return ReadTraceBuffer(context);

  default:
    return ErrorCode::kInvalidData;
  }
}

ErrorCode TraceHandler::ReadTraceBuffer(Context *context) {
  // See how many active trace variables there are
  // This gives us our sample size;
  size_t var_count = trace_->GetNumActiveVars();

  // If there aren't any active variables, I'm done
  if (!var_count) {
    context->response_length = 0;
    return ErrorCode::kNone;
  }

  // See how many samples I can return
  // First, find out how many I could based on the max value
  size_t max_samples = context->max_response_length /
                       static_cast<size_t>(var_count * sizeof(uint32_t));

  // If there's not enough room for even one sample, return an error.
  // That really shouldn't happen
  if (max_samples == 0) {
    return ErrorCode::kNoMemory;
  }

  // Find the total number of samples in the buffer
  size_t samples_count = trace_->GetNumSamples();
  if (samples_count > max_samples) {
    samples_count = max_samples;
  }

  std::array<uint32_t, kMaxTraceVars> record;
  for (size_t sample = 0; sample < samples_count; sample++) {
    // This shouldn't fail since I've already confirmed
    // the number of elements in the buffer.  If it does
    // fail it's a bug.
    if (!trace_->GetNextTraceRecord(&record, &var_count)) {
      break;
    }
    for (size_t variable = 0; variable < var_count; variable++) {
      u32_to_u8(record[variable], context->response);
      context->response += sizeof(uint32_t);
    }
  }

  context->response_length =
      static_cast<uint32_t>(samples_count * var_count * sizeof(uint32_t));
  return ErrorCode::kNone;
}

} // namespace Debug::Command
