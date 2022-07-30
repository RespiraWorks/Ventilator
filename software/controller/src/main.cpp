/* Copyright 2020-2022, RespiraWorks

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

#include "main_loops.h"
#include "vars.h"
#include "version.h"

/// \TODO only need StartLoopTimer, could be factored away into MainContainer?
#include "hal.h"

/// \TODO may not need this if high priority functor could be bound
static MainContainer god_object;

// This function handles all the high priority tasks which need to be called periodically.
// The HAL calls this function from a timer interrupt.
//
// NOTE - its important that anything being called from this function executes quickly.
// No busy waiting here.
void HighPriorityTask(void *arg) { god_object.high_priority_task(); }

/// \todo is this function needed at all, can go in main?
// This function is the lower priority background loop which runs continuously after some basic
// system init.  Pretty much everything not time critical should go here.
[[noreturn]] void BackgroundLoop() {
  // After all initialization is done, ask the HAL to start our high priority thread.
  hal.StartLoopTimer(Controller::GetLoopPeriod(), HighPriorityTask, nullptr);
  while (true) {
    god_object.background_task();
  }
}

int main() {
  using DAccess = Debug::Variable::Access;
  using DUint32 = Debug::Variable::UInt32;

  [[maybe_unused]] DEBUG_STRING(version, "0_controller_version", DAccess::ReadOnly,
                                Version::GitVersion,
                                "controller version at compile time: long git sha");
  [[maybe_unused]] DEBUG_STRING(branch, "0_controller_branch", DAccess::ReadOnly,
                                Version::GitBranch, "controller built from git branch");
  /// \TODO implement Bool type and use that?
  [[maybe_unused]] DUint32 git_dirty("0_controller_git_dirty", DAccess::ReadOnly, Version::GitDirty,
                                     "bool",
                                     "Was code dirty (had uncommitted changes) at time of"
                                     " building?",
                                     "%s");
  [[maybe_unused]] DEBUG_STRING(pio_env, "0_controller_pio_env", DAccess::ReadOnly, Version::PioEnv,
                                "platformio environment used for building");
  [[maybe_unused]] DEBUG_STRING(build_time, "0_controller_build_time", DAccess::ReadOnly,
                                Version::BuildTime, "UTC timestamp at time of controller build");

  god_object.init();
  BackgroundLoop();
}
