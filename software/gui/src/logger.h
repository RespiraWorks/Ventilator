#pragma once

#include <string>

#include <spdlog/spdlog.h>

#pragma GCC system_header

namespace CustomLogger {

void initLogger(const spdlog::level::level_enum &LoggingLevel, bool console_log,
                const std::string &log_file_name);
void closeLogger();

} // namespace CustomLogger

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
