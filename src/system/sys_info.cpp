#include "sys_info.hpp"

#include <sys/utsname.h>
#include <fstream>

namespace {

std::string ReadOsReleaseValue(const std::string& key) {
  std::ifstream os_release_file("/etc/os-release");
  std::string line;
  const std::string prefix = key + "=";

  while (std::getline(os_release_file, line)) {
    if (line.rfind(prefix, 0) != 0) {
      continue;
    }

    std::string value = line.substr(prefix.length());
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
      value = value.substr(1, value.size() - 2);
    }
    return value;
  }

  return {};
}

}  // namespace

SystemInfo SystemInfoProvider::GetSystemInfo() const {
  SystemInfo info;

  info.hostname = GetHostname();
  info.os_name = GetOsName();
  info.os_version = GetOsVersion();
  info.kernel_version = GetKernelVersion();
  info.arch = GetArchitecture();
  info.uptime = GetUptime();

  return info;
}

std::string SystemInfoProvider::GetHostname() const {
  struct utsname system_info;

  if (uname(&system_info) != 0) {
    return {};
  }

  return system_info.nodename;
}

std::string SystemInfoProvider::GetOsName() const {
  return ReadOsReleaseValue("NAME");
}

std::string SystemInfoProvider::GetOsVersion() const {
  return ReadOsReleaseValue("VERSION");
}

std::string SystemInfoProvider::GetKernelVersion() const {
  struct utsname system_info;

  if (uname(&system_info) != 0) {
    return {};
  }

  return system_info.release;
}

std::string SystemInfoProvider::GetArchitecture() const {
  struct utsname system_info;

  if (uname(&system_info) != 0) {
    return {};
  }

  return system_info.machine;
}

std::chrono::seconds SystemInfoProvider::GetUptime() const {
  std::ifstream uptime("/proc/uptime");
  double uptime_seconds;

  if(!(uptime >> uptime_seconds)) {
    return std::chrono::seconds{0};
  }

  return std::chrono::seconds{
    static_cast<long long>(uptime_seconds)
  };
}