#ifndef SPAWN_H_
#define SPAWN_H_

#include <string>
#include <vector>

auto spawn(const std::string &program,
           const std::vector<std::string> &arguments = {}) -> int;

#endif
