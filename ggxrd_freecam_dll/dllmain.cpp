// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Detouring.h"
#include "logging.h"
#include "memoryFunctions.h"
#include "Camera.h"
#include "Direct3DVTable.h"
#include "Game.h"
#include "EndScene.h"
#include "AltModes.h"
#include "Hud.h"
#include "Controls.h"
#include "Settings.h"
#include "Keyboard.h"

const char* DLL_NAME = "ggxrd_freecam_dll.dll";

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
#ifdef LOG_PATH
        {
            errno_t err;
            err = _wfopen_s(&logfile, LOG_PATH, L"wt");
            if (err != 0 || logfile == NULL) {
                return FALSE;
            }
            fputs("DllMain called with fdwReason DLL_PROCESS_ATTACH\n", logfile);
            fclose(logfile);
        }
#endif

        uintptr_t start;
        uintptr_t end;
        if (!getModuleBounds(DLL_NAME, &start, &end) || !start || !end) {
            logwrap(fputs("Note to developer: make sure to specify DLL_NAME char * constant correctly in dllmain.cpp\n", logfile));
            return FALSE;
        }

        if (!detouring.beginTransaction()) break;
        if (!game.onDllMain()) break;
        if (!camera.onDllMain()) break;
        if (!direct3DVTable.onDllMain()) break;
        if (!endScene.onDllMain()) break;
        if (!altModes.onDllMain()) break;
        if (!hud.onDllMain()) break;
        if (!settings.onDllMain()) break;
        if (!controls.onDllMain()) break;
        if (!detouring.endTransaction()) break;

        return TRUE;
    }
    case DLL_THREAD_ATTACH:
        return TRUE;
    case DLL_THREAD_DETACH:
        return TRUE;
    case DLL_PROCESS_DETACH: {
        logwrap(fputs("DLL_PROCESS_DETACH\n", logfile));
        detouring.dllMainThreadId = GetCurrentThreadId();
        logwrap(fprintf(logfile, "DllMain called from thread ID %d\n", GetCurrentThreadId()));
        detouring.detachAll();
        Sleep(100);
        while (detouring.someThreadsAreExecutingThisModule()) Sleep(100);
        hud.onDllDetach();
        controls.onDllDetach();
        return TRUE;
    }
    }
    return TRUE;
}

