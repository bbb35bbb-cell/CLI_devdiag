#pragma once

#include "command.hpp"

class SystemCommand : public ICommand {
public:
  SystemCommand() = default;
  ~SystemCommand() override = default;

  int Execute(const std::vector<std::string>& args) override;
};