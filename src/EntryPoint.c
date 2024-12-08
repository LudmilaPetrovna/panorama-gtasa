#include <windows.h>
#include <time.h>

#include "Panorama.h"
#include "Hooks.h"

// Very simple DLL entry

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved){

DWORD dwTID;
srand(time(0));

if(fdwReason==DLL_PROCESS_ATTACH){
hooks_install();
CreateThread(NULL,0,MyASIThread,NULL,0,&dwTID);
}

return TRUE;
}

