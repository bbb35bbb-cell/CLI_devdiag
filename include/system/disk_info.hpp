#pragma once

#include <string>
#include <cstdint>
#include <filesystem>

struct DiskInfo {
  std::string mount_point;
  uint64_t total_bytes = 0;
  uint64_t free_bytes = 0;
  uint64_t available_bytes = 0;
  uint64_t used_bytes = 0;
  double usage_percent = 0.0;
};

class DiskInfoProvider {
public:
  DiskInfo GetDiskInfo(const std::filesystem::path& path) const;
};
