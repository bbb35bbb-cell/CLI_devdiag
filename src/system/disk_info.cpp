#include "disk_info.hpp"


DiskInfo DiskInfoProvider::GetDiskInfo(const std::filesystem::path& path) const {
  DiskInfo info {};
  const auto file_system = std::filesystem::space(path);

  info.mount_point = path.string();
  info.total_bytes = file_system.capacity;
  info.free_bytes = file_system.free;
  info.available_bytes = file_system.available;
  info.used_bytes = file_system.capacity - file_system.free;
  info.usage_percent = static_cast<double>(info.used_bytes) / 
                        static_cast<double>(info.total_bytes) * 100.0;

  return info;
}