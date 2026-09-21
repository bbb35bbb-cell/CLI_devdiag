#include "system_command.hpp"
#include "sys_info.hpp"
#include "cpu_info.hpp"
#include "mem_info.hpp"
#include "disk_info.hpp"
#include "logger.hpp"

#include <sstream>
#include <iomanip>
#include <iostream>

namespace {
std::string FormatDouble(double value) {
  std::ostringstream stream;
  stream << std::fixed << std::setprecision(2) << value;
  return stream.str();
}

double KiBToGiB(uint64_t kib) {
  return static_cast<double>(kib) / (1024.0 * 1024.0);
}

double ByteToGiB(uint64_t bytes) {
  return static_cast<double>(bytes) / (1024.0 * 1024.0 * 1024.0);
}
}

int SystemCommand::Execute(const std::vector<std::string>& args) {
  Logger logger;
  SystemInfoProvider sys_info_provider;
  CpuInfoProvider cpu_info_provider;
  MemoryInfoProvider mem_info_provider;
  DiskInfoProvider disk_info_provider;
  SystemInfo sys_info = sys_info_provider.GetSystemInfo();
  CpuInfo cpu_info = cpu_info_provider.GetCpuInfo();
  MemoryInfo mem_info = mem_info_provider.GetMemInfo();
  DiskInfo disk_info = disk_info_provider.GetDiskInfo("/");

  std::string info_kind = args[0];

  if(info_kind == "system") {
    std::string msg = 
      "============================================\n"
      "System Information\n"
      "============================================\n"
      "Host Name      : " + sys_info.hostname + "\n"
      "OS Name        : " + sys_info.os_name + "\n"
      "OS Version     : " + sys_info.os_version + "\n"
      "Kernel Version : " + sys_info.kernel_version + "\n"
      "Architecture   : " + sys_info.arch + "\n"
      "Uptime         : " + std::to_string(sys_info.uptime.count()) + " seconds\n"
      "============================================";
    logger.Log(LogLevel::Info, msg);
  }else if(info_kind == "cpu") {
    std::string msg = 
      "============================================\n"
      "CPU Information\n"
      "============================================\n"
      "Model Name     : " + cpu_info.cpu_model + "\n"
      "Logical CPUs   : " + std::to_string(cpu_info.cpu_core_count) + "\n"
      "CPU Usage      : " + FormatDouble(cpu_info.cpu_usage) + " %\n"
      "CPU Frequency  : " + FormatDouble(cpu_info.cpu_freqency) + " MHz\n"
      "\n"
      "Load Average\n"
      "  1 min        : " + FormatDouble(cpu_info.load_avg[0]) + "\n"
      "  5 min        : " + FormatDouble(cpu_info.load_avg[1]) + "\n"
      "  15 min       : " + FormatDouble(cpu_info.load_avg[2]) + "\n"
      "============================================";
    logger.Log(LogLevel::Info, msg);
  }else if(info_kind == "memory") {
    std::string msg = 
      "============================================\n"
      "Memory Information\n"
      "============================================\n"
      "Total          : " + FormatDouble(KiBToGiB(mem_info.total_memory)) + " GB\n"
      "Used           : " + FormatDouble(KiBToGiB(mem_info.used_memory)) + " GB\n"
      "Available      : " + FormatDouble(KiBToGiB(mem_info.available_memory)) + " GB\n"
      "Free           : " + FormatDouble(KiBToGiB(mem_info.free_memory)) + " GB\n"
      "Usage          : " + FormatDouble(mem_info.memory_usage) + "%\n"
      "============================================";
    logger.Log(LogLevel::Info, msg);
  }else if(info_kind == "disk") {
    std::string msg = 
      "============================================\n"
      "Disk Information\n"
      "============================================\n"
      "Mount Point    : " + disk_info.mount_point + "\n"
      "Total          : " + FormatDouble(ByteToGiB(disk_info.total_bytes)) + " GB\n"
      "Used           : " + FormatDouble(ByteToGiB(disk_info.used_bytes)) + " GB\n"
      "Available      : " + FormatDouble(ByteToGiB(disk_info.available_bytes)) + " GB\n"
      "Free           : " + FormatDouble(ByteToGiB(disk_info.free_bytes)) + " GB\n"
      "Usage          : " + FormatDouble(disk_info.usage_percent) + "%\n"
      "============================================";
    logger.Log(LogLevel::Info, msg);
  }

  return 0;
}