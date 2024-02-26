#include "spawn.h"

#include <Windows.h>

#include <format>
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>

namespace {

auto inline_args_command(const std::string &prog,
                         const std::vector<std::string> &args) -> std::string {
  std::ostringstream cmd;
  cmd << prog;
  for (auto &arg : args) {
    cmd << " ";
    bool has_spaces = arg.find(" ") == std::string::npos;
    if (has_spaces)
      cmd << arg;
    else
      cmd << '\"' << arg << '\"';
  }
  return cmd.str();
}

auto get_error_message() -> std::string {
  DWORD err = GetLastError();
  char msg[512];

  FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, // dwFlags
                 nullptr,                    // lpSource
                 err,                        // dwMessageId
                 0,                          // dwLanguageId
                 msg,                        // lpBuffer
                 512,                        // nSize
                 nullptr);                   // *Arguments

  return std::format("Error {}: {}", err, msg);
}

} // namespace

auto spawn(const std::string &program,
           const std::vector<std::string> &arguments) -> int {
  const std::string command = inline_args_command(program, arguments);
  std::cout << "Executing: [" << command << "]" << std::endl;

  // NOTE: Specifies the window station, desktop, standard handles,
  // and appearance of the main window for a process at creation time.
  STARTUPINFO si;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);

  // NOTE: Contains information about a newly created process
  // and its primary thread.
  PROCESS_INFORMATION pi;
  ZeroMemory(&pi, sizeof(pi));

  // NOTE: As the second argument of CreateProcess will always
  // be modified, we copied the content of `command` into a
  // new buffer, aiming to avoid undefined behavior.
  constexpr rsize_t cmd_size = 32767;
  constexpr rsize_t last_index = cmd_size - 1;
  char cmd[cmd_size];
  strncpy_s(cmd, cmd_size, command.c_str(), last_index);
  cmd[last_index] = '\0';

  if (!CreateProcessA(nullptr, // lpApplicationName
                      cmd,     // lpCommandLine
                      nullptr, // lpProcessAttributes
                      nullptr, // lpThreadAttributes
                      FALSE,    // bInheritHandles
                      // TRUE,    // bInheritHandles
                      0,       // dwCreationFlags
                      nullptr, // lpEnvironment
                      nullptr, // lpCurrentDirectory
                      &si,     // lpStartupInfo
                      &pi)     // lpProcessInformation
  ) {
    std::string err_msg = get_error_message();
    std::cerr << "Spawn failed: [" << err_msg << "]" << std::endl;
    return -1;
  } else {
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exit_code;
    if (!GetExitCodeProcess(pi.hProcess, &exit_code)) {
      // Failed to get the exit code of the process.
      std::string err_msg = get_error_message();
      std::cerr << "Spawn failed: [" << err_msg << "]" << std::endl;
      return -1;
    }

    if (exit_code != 0) {
      // The process exited with a non-zero exit code.
      return exit_code;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
  }
}
