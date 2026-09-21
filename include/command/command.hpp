#pragma once
#include <vector>
#include <string>

class ICommand {
public :
  virtual ~ICommand() = default;

  virtual int Execute(const std::vector<std::string>& args) = 0;
};