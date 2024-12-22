#include <stdint.h>
#include <string.h>

#include "ApiGtaSA.h"
#include "toolbox.h"



void disable_ped_vehicle_shadows(){
// disable all shadows
*(char*)(0x55FC5E)=0x90;
*(char*)(0x55FC5F)=0x90;
*(char*)(0x55FC60)=0x90;
*(char*)(0x55FC61)=0x90;
*(char*)(0x55FC89)=0x90;
*(char*)(0x55FC89+1)=0x90;
*(char*)(0x55FC89+2)=0x90;
*(char*)(0x55FC89+3)=0x90;
*(float*)0xB7C4E8=0; // shadow strength
}

void set_green_screen_mode(){


// add green color to skybox
static uint8_t green_color_to_skybox1[]={0xB9, 0x00, 0xFF, 0x00, 0xFF, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
static uint8_t green_color_to_skybox2[]={0xB9, 0x00, 0xFF, 0x00, 0xFF, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};

memcpy((void*)0x713060,green_color_to_skybox1,sizeof(green_color_to_skybox1));
memcpy((void*)0x7130FF,green_color_to_skybox2,sizeof(green_color_to_skybox2));

// remove clouds from bottom
memset((void*)0x53E121,0x90,5);


// green tint

// sky top/bottom
//0xB7AAA0,0xB7A4E0,0xB7A7C0,,0xB7C1A0,0xB7C3C8,0xB7AB58,0xB7AEF0,0xB7B900,0xB7BB28
//0xB7A9E8,0xB7A428,0xB7A708,,0xB7C0E8,0xB7C310 0xB7AC10,0xB7AE38,0xB7B848,0xB7BA70,
//0xB7A930,0xB7A370,0xB7A650,,0xB7C030,0xB7C258,0xB7ACC8,0xB7AD80,0xB7B790,0xB7B9B8
//uint8_t* redpools[]=  {0xB7BD50,0xB7BF78};
//uint8_t* greenpools[]={0xB7BC98,0xB7BEC0};
//uint8_t* bluepools[]= {0xB7BBE0,0xB7BE08};

uint8_t* redpools[]=  {(void*)0xB7BF78};
uint8_t* greenpools[]={(void*)0xB7BEC0};
uint8_t* bluepools[]= {(void*)0xB7BE08};

#define TIMECYC_POOL_LEN 184

int q,w;
int pools_count=sizeof(redpools)/sizeof(redpools[0]);
for(w=0;w<pools_count;w++){
for(q=0;q<TIMECYC_POOL_LEN;q++){
redpools[w][q]=0;
greenpools[w][q]=255;
bluepools[w][q]=0;
}
}

MessageJumpQ("green sky", 1000, 0, false);

}

void set_water_level(){
int q;
CVector *pos=getPlayerVector();
void *water=(void*)0xC22910;
int water_vertex_size=20;
float newlevel=pos->z+1.0;
for(q=0;q<1200;q++){
*(float*)(water+q*water_vertex_size+4)=newlevel;
}
/*
      <Description>"WAVENESS_DEFAULT"</Description>
      <Address>72C659+6</Address>

      <Description>"CWeather_Wavyness"</Description>
      <Address>C812E8</Address>
*/
MessageJumpQ("sea level changed", 1000, 0, false);

}


void remove_annoing_tutorial(){

// remove intro tutorials
// restarts count
*(char*)0xA4326C=1;
*(char*)0xA43268=1;

// possible help_wasted_shown
*(uint32_t*)0xA49b3c=1;

// help_busted
*(uint32_t*)0xA49b58=1;

// remove hospital_pickup1-2
CPickups__RemovePickUp(*(uint32_t*)0xA4d534);
CPickups__RemovePickUp(*(uint32_t*)0xA4d538);
CPickups__RemovePickUp(*(uint32_t*)0xA4d53c);

MessageJumpQ("police/hospital restars patched", 1000, 0, false);

}


void work_at_background(){
// no move mouse / SetCursorPos
*(char*)0x007453F0=0xc3;
*(char*)0x0057C520=0xc3;

// no change cursor visibility / ShowCursor
*(char*)0x7453E0=0xc3;
*(char*)(0x747FE9+1)=1;
*(char*)(0x7481CD+1)=1;

// is foreground
*(char*)0x8D621C=1;
*(int*)(0x747FB0+6)=1;
*(int*)(0x748054+6)=1;
}

void apply_game_filters(FILTERS_PARAMS *p){
*(char*)0xC7C70C=p->vidcam;
*(char*)0xC7C70D=p->liftcam;
*(char*)0xC402b4=p->grain;
*(char*)0xC402b7=p->cutscene;
*(char*)0xC402b8=p->nightvision;
*(char*)0xC402b9=p->infravision;
*(char*)0xC402ba=p->heat;
*(char*)0xC402D1=p->rain;
*(char*)0xC402D2=p->smokey;
*(char*)0xC402D3=p->underwater;
*(char*)0xC402C5=p->cctv;
*(unsigned char*)0xC402C4=p->darkness;
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

