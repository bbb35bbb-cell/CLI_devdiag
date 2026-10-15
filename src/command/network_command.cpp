#include "network_command.hpp"
#include "network_info.hpp"

#include <iostream>

int NetworkCommand::Execute(const std::vector<std::string>& args) {
  const std::string interface = args.size() > 1 ? args[1] : "";
  NetworkInfoProvider provider;
  const auto interfaces = provider.GetNetworkInfo(interface);
  if (interfaces.empty()) {
    std::cerr << "No network interface information available.\n";
    return 1;
  }
  std::cout << "Network Interfaces\n------------------\n";
  for (const auto& item : interfaces) {
    std::cout << item.name << "  " << (item.up ? "UP" : "DOWN")
              << "  " << (item.loopback ? "LOOPBACK" : "")
              << "  " << (item.address.empty() ? "-" : item.address) << '\n';
  }
  return 0;
}
