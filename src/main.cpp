#include <iostream>
#include <string>
#include <vector>

#include "common/parser.hpp"
#include "network_command.hpp"
#include "system_command.hpp"

int main(int argc, char **argv) {
  std::vector<std::string> args(argv + 1, argv + argc);
  CommandLineOptions options;
  std::string error;
  if (!ParseCommandLine(args, &options, &error)) {
    std::cerr << "Error: " << error << "\n\n" << UsageText(argv[0]);
    return 2;
  }
  if (options.command == "help") {
    std::cout << UsageText(argv[0]);
    return 0;
  }
  if(options.command == "system" || options.command == "cpu" ||
     options.command == "memory" || options.command == "disk") {
    SystemCommand system_command;
    return system_command.Execute(args);
  } else if (options.command == "network") {
    NetworkCommand network_command;
    std::vector<std::string> network_args{options.command};
    if (!options.interface_name.empty()) network_args.push_back(options.interface_name);
    return network_command.Execute(network_args);
  } else if (options.command == "collect") {
    SystemCommand system_command;
    for (const std::string& command : {std::string("system"), std::string("cpu"),
                                       std::string("memory"), std::string("disk")}) {
      const int result = system_command.Execute({command});
      if (result != 0) return result;
    }
  }
  return 0;
}
