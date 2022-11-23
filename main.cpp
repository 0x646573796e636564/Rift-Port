#define _CRT_SECURE_NO_WARNINGS

#include "ChapterOne/core.h"
#include "memory.h"

void MainThread()
{
    uintptr_t IsChapterOne = Memory::FindPattern("48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 8B A5 ? ? ? ? 48 8D 05");

#ifdef RIFT_DEVELOPMENT
    Util::OPEN_CONSOLE();
#endif

    if (IsChapterOne)
        ChapterOneCore::Setup();
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        MainThread();
        break;
    }
    return TRUE;
}