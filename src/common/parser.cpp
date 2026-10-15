#include "parser.hpp"

#include <sstream>

namespace {
bool IsCommand(const std::string& value) {
  return value == "system" || value == "cpu" || value == "memory" ||
         value == "disk" || value == "network" || value == "collect" ||
         value == "help";
}
}

bool ParseCommandLine(const std::vector<std::string>& args,
                      CommandLineOptions* options,
                      std::string* error) {
  if (options == nullptr || error == nullptr) {
    return false;
  }
  if (args.empty()) {
    *error = "command is required";
    return false;
  }
  options->command = args.front();
  if (!IsCommand(options->command)) {
    *error = "unknown command: " + options->command;
    return false;
  }

  for (std::size_t i = 1; i < args.size(); ++i) {
    const std::string& arg = args[i];
    if (arg == "--json") {
      options->json = true;
    } else if (arg == "--verbose") {
      options->verbose = true;
    } else if (arg == "--path" && i + 1 < args.size()) {
      options->path = args[++i];
    } else if (arg == "--interface" && i + 1 < args.size()) {
      options->interface_name = args[++i];
    } else {
      *error = "unknown or incomplete option: " + arg;
      return false;
    }
  }
  return true;
}

std::string UsageText(const std::string& program_name) {
  std::ostringstream usage;
  usage << "Usage: " << program_name << " <command> [options]\n\n"
        << "Commands:\n"
        << "  system              Show operating-system information\n"
        << "  cpu                 Show CPU and load information\n"
        << "  memory              Show memory usage\n"
        << "  disk                Show filesystem usage (default: /)\n"
        << "  network             Show network interfaces\n"
        << "  collect             Show all supported diagnostics\n"
        << "  help                Show this help\n\n"
        << "Options:\n"
        << "  --path <path>       Filesystem path for disk\n"
        << "  --interface <name>  Limit network output to one interface\n"
        << "  --json              Reserved for structured output\n"
        << "  --verbose           Include diagnostic log messages\n";
  return usage.str();
}
