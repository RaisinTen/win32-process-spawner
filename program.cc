#include <Windows.h>

#include <iostream>

int wWinMain(HINSTANCE /* hInstance */, HINSTANCE /* hPrevInstance */,
               PWSTR /* pCmdLine */, int /* nCmdShow */) {
  /* This attaches a console to the parent process if it has a console.
   * This code must run before the constructors of the members of the
   * Application subclass gets run, so that necessary output from those
   * could be shown in the console.
   */
  if (AttachConsole(ATTACH_PARENT_PROCESS)) {
    /* Reopens the stdout handle as console window output. */
    FILE *stdout_stream;
    freopen_s(&stdout_stream, "CONOUT$", "wb", stdout);
    FILE *stderr_stream;
    /* Reopens the stderr handle as console window output. */
    freopen_s(&stderr_stream, "CONOUT$", "wb", stderr);

    /* Sets the output code page to UTF-8. */
    SetConsoleOutputCP(CP_UTF8);
  }

  std::cout << "Hello, from program stdout" << std::endl;
  std::cerr << "Goodbye, from program stderr!" << std::endl;

  return 0;
}
