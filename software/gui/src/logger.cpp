#include "logger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace CustomLogger {

void initLogger(const spdlog::level::level_enum &LoggingLevel, bool console_log,
                const std::string &log_file_name) {
  closeLogger();

  std::vector<spdlog::sink_ptr> sinks;

  if (console_log) {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern(
        "[%Y-%m-%d %H:%M:%S.%f] [%l] [processID: %P]: %v");
    sinks.push_back(console_sink);
  }

  if (!log_file_name.empty()) {
    auto file_sink =
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file_name);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%l] [processID: %P]: %v");
    sinks.push_back(file_sink);
  }

  auto combined_logger = std::make_shared<spdlog::logger>(
      "daquiri_logger", begin(sinks), end(sinks));
  combined_logger->set_level(LoggingLevel);
  combined_logger->flush_on(LoggingLevel);
  spdlog::flush_every(std::chrono::seconds(1));
  spdlog::set_default_logger(combined_logger);
}

void closeLogger() {
  // Release all spdlog resources, and drop all loggers in the registry.
  // This is optional (only mandatory if using windows + async log).
  spdlog::shutdown();
}

} // namespace CustomLogger
