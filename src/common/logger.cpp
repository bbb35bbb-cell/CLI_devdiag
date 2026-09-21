#include "logger.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>

void Logger::Log(LogLevel level, const std::string& message) {
  std::string s = FormatMessage(level, message);

  std::cout << s << std::endl;
}

void Logger::Debug(const std::string& message) {
  Log(LogLevel::Debug, message);
}

void Logger::Info(const std::string& message) {
  Log(LogLevel::Info, message);
}

void Logger::Warn(const std::string& message) {
  Log(LogLevel::Warning, message);
}

void Logger::Error(const std::string& message) {
  Log(LogLevel::Error, message);
}

std::string Logger::FormatMessage(LogLevel level, const std::string& message) const {
  std::string timestamp = GetTimestamp();
  std::string prefix;

  switch(level) {
    case LogLevel::Debug:
      prefix = "[" + timestamp + "] [DEBUG]";
      break;
    case LogLevel::Info:
      prefix = "[" + timestamp + "] [INFO]";
      break;
    case LogLevel::Warning:
      prefix = "[" + timestamp + "] [WARN]";
      break;
    case LogLevel::Error:
      prefix = "[" + timestamp + "] [ERROR]";
      break;
    default:
      prefix = "[" + timestamp + "] [INFO]";
  }

  if (message.find('\n') != std::string::npos) {
    return prefix + "\n" + message;
  }

  return prefix + " " + message;
}

std::string Logger::LevelToString(LogLevel level) const {
  switch(level) {
    case LogLevel::Debug: return "DEBUG";
    case LogLevel::Info: return "INFO";
    case LogLevel::Warning: return "WARN";
    case LogLevel::Error: return "ERROR";
    default: return "INFO";
  }

  return "INFO";
}

std::string Logger::GetTimestamp() const {
  std::chrono::system_clock::time_point time_point = std::chrono::system_clock::now();
  std::time_t t = std::chrono::system_clock::to_time_t(time_point);
  std::tm* tm = std::localtime(&t);

  std::stringstream stream;
  stream << std::put_time(tm, "%Y-%m-%d %X");
  return stream.str();
}

