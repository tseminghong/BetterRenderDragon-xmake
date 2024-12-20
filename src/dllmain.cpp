#include <windows.h>
#include <wrl.h>

#include "ImGuiHooks.h"
#include "MCHooks.h"
#include "MCPatches.h"
#include "Options.h"

#include <fcntl.h>
#include <io.h>

/*
void openConsole() {
  AllocConsole();
  SetConsoleTitleA("Debug Console");
  system("chcp 65001>nul");
  HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
  INT hCrt = _open_osfhandle((INT)hCon, _O_TEXT);
  FILE *hf = _fdopen(hCrt, "w");
  setvbuf(hf, NULL, _IONBF, 0);
  *stdout = *hf;
  freopen("CONOUT$", "w+t", stdout);
}
*/

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH: {
    if (FAILED(Windows::Foundation::Initialize(RO_INIT_MULTITHREADED))) {
      printf("Windows::Foundation::Initialize failed\n");
      return TRUE;
    }
    Options::init();
    Options::load();

    initMCHooks();
    initMCPatches();
    initImGuiHooks();

    DisableThreadLibraryCalls(hModule);
    break;
  }
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    Options::save();
    Windows::Foundation::Uninitialize();
    break;
  }
  return TRUE;
}
