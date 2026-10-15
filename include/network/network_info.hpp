#pragma once

#include <vector>
#include <string>

struct NetworkInterfaceInfo {
  std::string name;
  std::string address;
  std::string netmask;
  bool loopback = false;
  bool up = false;
};

class NetworkInfoProvider {
public:
  std::vector<NetworkInterfaceInfo> GetInterfaceInfo() const;
  std::vector<NetworkInterfaceInfo> GetNetworkInfo(const std::string& interface) const;
};
