#include "mem_info.hpp"

#include <string>
#include <fstream>

namespace {

MemoryInfo ReadMemValue() {
  MemoryInfo info{};
  std::ifstream mem_info("/proc/meminfo");
  std::string line;

  while(std::getline(mem_info, line)) {
    std::size_t seperator = line.find(':');
    if(seperator == std::string::npos) {
      continue;
    }

    std::string key = line.substr(0, seperator);
    std::string value = line.substr(seperator+1);
    std::size_t value_first = value.find_first_not_of(" \t");
    std::size_t value_last = value.rfind(' ');
    if(value_first == std::string::npos || value_last == std::string::npos) {
      continue;
    }

    value = value.substr(value_first, value_last - value_first);

    if(key == "MemTotal") {
      info.total_memory = std::stoull(value);
    }else if(key == "MemAvailable") {
      info.available_memory = std::stoull(value);
    }else if(key == "MemFree") {
      info.free_memory = std::stoull(value);
    }

    info.used_memory = info.total_memory - info.available_memory;
    info.memory_usage = static_cast<double>(info.used_memory) / 
                        static_cast<double>(info.total_memory) * 100.0;
  }

  return info;
}

}

MemoryInfo MemoryInfoProvider::GetMemInfo() const {
  MemoryInfo info = ReadMemValue();
  
  return info;
}