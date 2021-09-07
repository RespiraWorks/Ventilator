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

Import("env")


def get_firmware_specifier_build_flag():
    repo = git.Repo(search_parent_directories=True)
    git_version = repo.git.describe("--tags")
    git_dirty = repo.is_dirty(untracked_files=True)
    pio_env = env["PIOENV"].upper()
    print(f"git_version: {git_version}")
    print(f"git_dirty: {git_dirty}")
    print(f"pio_env: {pio_env}")

    version_flag = f'-D GIT_VERSION=\\"{git_version}\\"'
    dirty_flag = "-D GIT_DIRTY=" + f"{int(git_dirty)}"
    env_flag = f'-D PIO_ENV=\\"{pio_env}\\"'
    return version_flag + " " + dirty_flag + " " + env_flag


ret = get_firmware_specifier_build_flag()
print(f"BUILD FLAGS:   {ret}")

env.Append(BUILD_FLAGS=[get_firmware_specifier_build_flag()])
