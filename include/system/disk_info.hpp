#pragma once

#include <string>
#include <cstdint>
#include <filesystem>

struct DiskInfo {
  std::string mount_point;
  uint64_t total_bytes;
  uint64_t free_bytes;
  uint64_t available_bytes;
  uint64_t used_bytes;
  double usage_percent;
};

class DiskInfoProvider {
public:
  DiskInfo GetDiskInfo(const std::filesystem::path& path) const;
};