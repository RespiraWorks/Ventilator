#!/usr/bin/env python3

__copyright__ = "Copyright 2021 RespiraWorks"

__license__ = """

    Copyright 2021 RespiraWorks

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

"""


import git
from datetime import datetime

Import("env")


def get_firmware_specifier_build_flag():
    repo = git.Repo(search_parent_directories=True)
    # TODO: contingency for detached heads, individual commits or some mid-rebase situations?
    GitVersion = repo.head.object.hexsha
    # TODO: reintroduce semantic version info using repo.git.describe("--tags")
    GitBranch = repo.active_branch.name
    GitDirty = int(repo.is_dirty(untracked_files=True))
    PioEnv = env["PIOENV"].upper()
    build_time_utc = datetime.utcnow().strftime("UTC\\ %Y-%m-%d\\ %H:%M:%S")

    print(f"GitVersion: {GitVersion}")
    print(f"pio_branch: {GitBranch}")
    print(f"GitDirty: {GitDirty}")
    print(f"PioEnv: {PioEnv}")

    version_flag = f'-D GIT_VERSION=\\"{GitVersion}\\"'
    branch_flag = f'-D GIT_BRANCH=\\"{GitBranch}\\"'
    dirty_flag = "-D GIT_DIRTY=" + f"{GitDirty}"
    env_flag = f'-D PIO_ENV=\\"{PioEnv}\\"'
    time_flag = f'-D BUILD_TIME=\\"{build_time_utc}\\"'
    return [version_flag, branch_flag, dirty_flag, env_flag, time_flag]


ret = get_firmware_specifier_build_flag()
print(f"BUILD FLAGS:   {ret}")

env.Append(BUILD_FLAGS=ret)
