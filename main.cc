#include "spawn.h"

#include <iostream>

int main() {
  std::string program = "program.exe";
  std::cout << "Before spawning: [" << program << "]" << std::endl;
  int ret = spawn(program);
  std::cout << "After spawning: [" << program << "]" << std::endl;
  std::cout << "ret: [" << ret << "]" << std::endl;

  return 0;
}
