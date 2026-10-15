#pragma once

#include <cstdint>

struct MemoryInfo {
  uint64_t total_kib = 0;
  uint64_t used_kib = 0;
  uint64_t available_kib = 0;
  uint64_t free_kib = 0;
  double usage_percent = 0.0;
};

class MemoryInfoProvider {
public:
  MemoryInfo GetMemInfo() const;
};
