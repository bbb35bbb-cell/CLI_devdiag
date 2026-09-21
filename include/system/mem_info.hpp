#pragma once

#include <cstdint>

struct MemoryInfo {
  uint64_t total_memory;
  uint64_t used_memory;
  uint64_t available_memory;
  uint64_t free_memory;
  double memory_usage;
};

class MemoryInfoProvider {
public:
  MemoryInfo GetMemInfo() const;
};