#pragma once

#include <string>
#include <vector>

struct CommandLineOptions {
  std::string command;
  std::string path = "/";
  std::string interface_name;
  bool json = false;
  bool verbose = false;
};

// Parses command-line arguments and returns a user-facing error on failure.
bool ParseCommandLine(const std::vector<std::string>& args,
                      CommandLineOptions* options,
                      std::string* error);

std::string UsageText(const std::string& program_name);
