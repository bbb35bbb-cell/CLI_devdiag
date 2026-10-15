#pragma once

#include <string>

enum class LogLevel {
  Debug,
  Info,
  Warning,
  Error,
};

class Logger {
public:
  explicit Logger(bool verbose = false) : verbose_(verbose) {}

  void Log(LogLevel level, const std::string& message);

  void Debug(const std::string& message);
  void Info(const std::string& message);
  void Warn(const std::string& message);
  void Error(const std::string& message);

private:
  bool verbose_;
  std::string FormatMessage(LogLevel level, const std::string& message) const;
  std::string LevelToString(LogLevel level) const;
  std::string GetTimestamp() const;
};
