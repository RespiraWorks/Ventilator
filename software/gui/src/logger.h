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

#pragma once

#include <spdlog/spdlog.h>

#include <cassert>  /// Somehow spdlog needs this but fails to bring it
#include <string>

#pragma GCC system_header

namespace CustomLogger {

void initLogger(const spdlog::level::level_enum &LoggingLevel, bool console_log,
                const std::string &log_file_name);
void closeLogger();

}  // namespace CustomLogger

/// Do not use directly; use the defines below instead
/// global loglevel is set in main.cpp, etc.
#define LOG(Severity, Format, ...) spdlog::log(Severity, Format, ##__VA_ARGS__)

/// \brief macros to be used in your code
///   for example:
///     DBG("Here be a message, to tell you about about a variable={},
///     variable);
///   Messages follow fmt syntax: https://github.com/fmtlib/fmt
#define CRIT(Format, ...) LOG(spdlog::level::critical, Format, ##__VA_ARGS__)
#define ERR(Format, ...) LOG(spdlog::level::err, Format, ##__VA_ARGS__)
#define WARN(Format, ...) LOG(spdlog::level::warn, Format, ##__VA_ARGS__)
#define INFO(Format, ...) LOG(spdlog::level::info, Format, ##__VA_ARGS__)
#define DBG(Format, ...) LOG(spdlog::level::debug, Format, ##__VA_ARGS__)
#define TRC(Format, ...) LOG(spdlog::level::trace, Format, ##__VA_ARGS__)
