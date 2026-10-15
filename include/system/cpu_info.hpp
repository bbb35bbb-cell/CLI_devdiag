#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <cstdint>

struct CpuTimes {
  uint64_t user;
  uint64_t nice;
  uint64_t system;
  uint64_t idle;
  uint64_t iowait;
  uint64_t irq;
  uint64_t softirq;
  uint64_t steal;
};

struct CpuHardwareInfo {
  std::string model_name;
  uint32_t cpu_count;
  double cpu_mhz;
};

struct CpuTaskLoadInfo {
  double load_avg_1min;
  double load_avg_5min;
  double load_avg_15min;
};

struct CpuInfo {
  double cpu_usage;
  double cpu_frequency;
  std::vector<double> load_average;
  std::string cpu_model;
  uint32_t cpu_core_count;
};

class CpuInfoProvider {
public:
  CpuInfo GetCpuInfo() const;
};
