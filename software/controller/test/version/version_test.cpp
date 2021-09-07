/* Copyright 2020, RespiraWorks

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

#include "version.h"

#include "gtest/gtest.h"

TEST(Version, GitVersion) { EXPECT_GT(sizeof(git_version), 14); }

TEST(Version, GitDirty) { EXPECT_EQ(typeid(git_dirty), typeid(bool)); }

TEST(Version, PioEnv) { EXPECT_STREQ(pio_env, "NATIVE"); }
