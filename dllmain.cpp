#include "pch.h"
#include <windows.h>
#include <iostream>

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        if (AllocConsole()) {
            FILE* f;
            freopen_s(&f, "CONOUT$", "w", stdout);

            std::cout << "j0eys DLL injected\n";
        }
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
