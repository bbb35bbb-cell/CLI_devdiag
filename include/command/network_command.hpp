#pragma once

#include "command.hpp"

class NetworkCommand : public ICommand {
public:
  NetworkCommand() = default;
  ~NetworkCommand() override = default;

  int Execute(const std::vector<std::string>& args) override;
};