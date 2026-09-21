#pragma once

#include <string>
#include <chrono>

struct SystemInfo {
  std::string hostname;
  std::string os_name;
  std::string os_version;
  std::string kernel_version;
  std::string arch;
  std::chrono::seconds uptime;
};

class SystemInfoProvider {
public:
  SystemInfo GetSystemInfo() const;
private:
  std::string GetHostname() const;
  std::string GetOsName() const;
  std::string GetOsVersion() const;
  std::string GetKernelVersion() const;
  std::string GetArchitecture() const;
  std::chrono::seconds GetUptime() const;
};