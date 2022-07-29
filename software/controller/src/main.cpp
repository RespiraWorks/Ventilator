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

int main() {
  MainContainer god_object;

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

  // Initializes everything and starts high priority loop.
  god_object.init();

  // This is the lower priority loop which runs continuously after some basic system init.
  // Pretty much everything not time critical should happen here.
  while (true) {
    god_object.background_task();
  }
}
