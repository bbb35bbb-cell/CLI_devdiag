#include "network_info.hpp"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>

#include <map>

std::vector<NetworkInterfaceInfo> NetworkInfoProvider::GetInterfaceInfo() const {
  return GetNetworkInfo({});
}

std::vector<NetworkInterfaceInfo> NetworkInfoProvider::GetNetworkInfo(
    const std::string& interface) const {
  std::map<std::string, NetworkInterfaceInfo> interfaces;
  struct ifaddrs* addresses = nullptr;
  if (getifaddrs(&addresses) != 0) {
    return {};
  }

  for (const struct ifaddrs* current = addresses; current != nullptr;
       current = current->ifa_next) {
    if (current->ifa_name == nullptr ||
        (!interface.empty() && interface != current->ifa_name)) {
      continue;
    }
    auto& info = interfaces[current->ifa_name];
    info.name = current->ifa_name;
    info.loopback = (current->ifa_flags & IFF_LOOPBACK) != 0;
    info.up = (current->ifa_flags & IFF_UP) != 0;
    if (current->ifa_addr == nullptr) {
      continue;
    }
    const int family = current->ifa_addr->sa_family;
    char address[INET6_ADDRSTRLEN] = {};
    if (family == AF_INET || family == AF_INET6) {
      const void* raw_address = family == AF_INET
          ? static_cast<const void*>(&reinterpret_cast<sockaddr_in*>(current->ifa_addr)->sin_addr)
          : static_cast<const void*>(&reinterpret_cast<sockaddr_in6*>(current->ifa_addr)->sin6_addr);
      if (inet_ntop(family, raw_address, address, sizeof(address)) != nullptr &&
          info.address.empty()) {
        info.address = address;
      }
    }
  }
  freeifaddrs(addresses);

  std::vector<NetworkInterfaceInfo> result;
  for (const auto& entry : interfaces) {
    result.push_back(entry.second);
  }
  return result;
}
