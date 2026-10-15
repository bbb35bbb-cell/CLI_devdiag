#include "cpu_info.hpp"

#include <chrono>
#include <fstream>
#include <sstream>
#include <thread>

/*
CPU Information
------------------------------------------------
Model           : Intel(R) Core(TM) i7-13700
Logical CPUs    : 24
CPU Usage       : 17.4 %
CPU Frequency   : 2112.45 MHz

Load Average
  1 min         : 0.72
  5 min         : 0.91
  15 min        : 1.02
*/

namespace {

CpuTimes ReadCpuTimes() {
  CpuTimes cpu_times{};
  std::ifstream cpu_stat("/proc/stat");
  std::string line;

  while (std::getline(cpu_stat, line)) {
    std::istringstream line_stream(line);
    std::string cpu_label;
    if (!(line_stream >> cpu_label) || cpu_label != "cpu") continue;
    line_stream >> cpu_times.user
                >> cpu_times.nice
                >> cpu_times.system
                >> cpu_times.idle
                >> cpu_times.iowait
                >> cpu_times.irq
                >> cpu_times.softirq
                >> cpu_times.steal;
    return cpu_times;
  }

  return cpu_times;
}

double CalCpuUsage() {
  CpuTimes before = ReadCpuTimes();
  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  CpuTimes after = ReadCpuTimes();

  uint64_t before_total = before.user + before.nice + before.system + before.idle + 
                          before.iowait + before.irq + before.softirq + before.steal;
  uint64_t after_total = after.user + after.nice + after.system + after.idle +
                         after.iowait + after.irq + after.softirq + after.steal;
  uint64_t total_delta = after_total - before_total;
  uint64_t idle_delta = (after.idle + after.iowait) - (before.idle + before.iowait);

  if(total_delta == 0) {
    return 0.0;
  }

  if (idle_delta > total_delta) return 0.0;
  return (1.0 - static_cast<double>(idle_delta) /
          static_cast<double>(total_delta)) * 100.0;
}

CpuHardwareInfo ReadCpuInfo() {
  CpuHardwareInfo info{};
  std::ifstream cpu_info("/proc/cpuinfo");
  std::string line;

  while(std::getline(cpu_info, line)) {
    std::size_t separator = line.find(':');
    if (separator == std::string::npos) {
      continue;
    }

    std::string key = line.substr(0, separator);
    std::string value = line.substr(separator+1);
    std::size_t key_first = key.find_first_not_of(" \t");
    std::size_t key_last = key.find_last_not_of(" \t");
    if (key_first == std::string::npos) {
      continue;
    }
    key = key.substr(key_first, key_last - key_first + 1);

    std::size_t value_first = value.find_first_not_of(" \t");
    if (value_first != std::string::npos) {
      value = value.substr(value_first);
    }

    if ((key == "model name" || key == "Processor") && info.model_name.empty()) {
      info.model_name = value;
    } else if (key == "processor") {
      ++info.cpu_count;
    } else if (key == "cpu MHz" && info.cpu_mhz == 0.0) {
      try { info.cpu_mhz = std::stod(value); } catch (const std::exception&) { }
    }
  }

  return info;
}

CpuTaskLoadInfo ReadTaskLoadInfo() {
  CpuTaskLoadInfo info{};
  std::ifstream task_load("/proc/loadavg");
  std::string line;

  if(!std::getline(task_load, line)){
    return info;
  }
  std::istringstream line_stream(line);

  if (!(line_stream >> info.load_avg_1min
                    >> info.load_avg_5min
                    >> info.load_avg_15min)) {
    return CpuTaskLoadInfo{};
  }
  return info;
}

}

CpuInfo CpuInfoProvider::GetCpuInfo() const {
  CpuInfo info{};
  CpuHardwareInfo hw_info = ReadCpuInfo();
  CpuTaskLoadInfo load_info = ReadTaskLoadInfo();

  info.cpu_usage = CalCpuUsage();
  info.cpu_frequency = hw_info.cpu_mhz;
  info.load_average.push_back(load_info.load_avg_1min);
  info.load_average.push_back(load_info.load_avg_5min);
  info.load_average.push_back(load_info.load_avg_15min);
  info.cpu_model = hw_info.model_name;
  info.cpu_core_count = hw_info.cpu_count;

  if (info.cpu_core_count == 0) info.cpu_core_count = 1;
  return info;
}
