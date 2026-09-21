#pragma once

#include "command.hpp"
#include <vector>
#include <string>

class SystemCommand : public ICommand {
public:
  SystemCommand() = default;
  ~SystemCommand() override = default;

  int Execute(const std::vector<std::string>& args) override;
};