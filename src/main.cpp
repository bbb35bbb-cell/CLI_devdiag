#include <iostream>
#include <string>
#include <vector>

#include "system_command.hpp"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <command>" << std::endl;
    return 1;
  }

  std::vector<std::string> args(argv + 1, argv + argc);

  if(args[0] == "system" || args[0] == "cpu" || args[0] == "memory" || args[0] == "disk") {
    SystemCommand system_command;
    system_command.Execute(args);
  }else if(args[0] == "network") {

  }else if(args[0] == "process") {

  }else if(args[0] == "collect") {

  }else if(args[0] == "help") {
    
  }
  
  return 0;
}