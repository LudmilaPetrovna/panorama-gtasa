#include <windows.h>
#include <time.h>

#include "ApiGtaSA.h"
#include "Panorama.h"
#include "Hooks.h"

#include "toolbox.h"
#include "Webmenu.h"

// Very simple DLL entry


DWORD WINAPI Watchdog(LPVOID lpParam){
Sleep(2000);
memory512(0);
if(*CTimer_m_FrameCounter<5){
exit(0);
}
return 0;
}


BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved){

DWORD dwTID;
srand(time(0));

if(fdwReason==DLL_PROCESS_ATTACH){
work_at_background();
hooks_install();
memory512(0);
CreateThread(NULL,0,MyASIThread,NULL,0,&dwTID);
CreateThread(NULL,0,Watchdog,NULL,0,&dwTID);
webmenu_start();
remove_annoing_tutorial();
no_forbidden_territory();

}

return TRUE;
}

