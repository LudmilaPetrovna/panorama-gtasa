#include <stdint.h>
#include "ApiGtaSA.h"
#include "toolbox.h"


void work_at_background(){
// no move mouse
*(char*)0x007453F0=0xc3;
*(char*)0x0057C520=0xc3;

// is foreground
*(char*)0x8D621C=1;
}

void apply_game_filters(FILTERS_PARAMS *p){
*(char)0xC7C70C=p->vidcam;
*(char)0xC7C70D=p->liftcam;
*(char)0xC402b4=p->grain;
*(char)0xC402b7=p->cutscene;
*(char)0xC402b8=p->nightvision;
*(char)0xC402b9=p->infravision;
*(char)0xC402ba=p->heat;
*(char)0xC402D1=p->rain;
*(char)0xC402D2=p->smokey;
*(char)0xC402D3=p->underwater;
*(char)0xC402C5=p->cctv;
*(unsigned char)0xC402C4=p->darkness;
}


void memory512(int avail){
if(avail<=0){
avail=512000000;
}
*CStreaming__memoryAvailable=avail;
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

