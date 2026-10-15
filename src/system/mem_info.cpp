#include "mem_info.hpp"

#include <string>
#include <fstream>
#include <sstream>

namespace {

MemoryInfo ReadMemValue() {
  MemoryInfo info{};
  std::ifstream mem_info("/proc/meminfo");
  std::string line;

  while(std::getline(mem_info, line)) {
    std::size_t separator = line.find(':');
    if(separator == std::string::npos) {
      continue;
    }

    std::string key = line.substr(0, separator);
    std::istringstream value_stream(line.substr(separator + 1));
    uint64_t value = 0;
    value_stream >> value;

    if(key == "MemTotal") {
      info.total_kib = value;
    }else if(key == "MemAvailable") {
      info.available_kib = value;
    }else if(key == "MemFree") {
      info.free_kib = value;
    }

  }

  if (info.available_kib > info.total_kib) info.available_kib = info.total_kib;
  info.used_kib = info.total_kib - info.available_kib;
  info.usage_percent = info.total_kib == 0 ? 0.0 :
      static_cast<double>(info.used_kib) / static_cast<double>(info.total_kib) * 100.0;

  return info;
}

}

MemoryInfo MemoryInfoProvider::GetMemInfo() const {
  MemoryInfo info = ReadMemValue();
  
  return info;
}
