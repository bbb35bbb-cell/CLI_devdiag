#pragma once

#include <string>
#include <vector>

class ICommand {
public :
  virtual ~ICommand() = default;

  // args contains the command name at index zero.
  virtual int Execute(const std::vector<std::string>& args) = 0;
};
