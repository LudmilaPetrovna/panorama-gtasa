#include <stdint.h>
#include "ApiGtaSA.h"


void memory512(){
*CStreaming__memoryAvailable=512000000;
}


void No_more_haze(){
// patch code
//.text:0072C1B7 7C 06   jl   short loc_72C1BF ; Jump if Less (SF!=OF)

*(uint8_t*)0x72C1B7=0xEB;
}

void nohud(){

*CTheScripts__bDisplayHud=0;
*CHud__bScriptDontDisplayRadar=1;
*CHud__textBoxNumLines=0.0;
*CHud__helpMessageState=0;

//May be better call it?
//CMessages::ClearAllMessagesDisplayedByGame(0);
//CMessages::ClearMessages(0);
//CHud::GetRidOfAllHudMessages(0);
}