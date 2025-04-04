#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "ApiGtaSA.h"
#include "log.h"


char screenMessage[1024];
static FILE *height_map=NULL;


// float BA6788 - draw distance
// float c812f0 - bending
// float a9aeb0 - fx_wind

/*
if(GetAsyncKeyState(VK_F5)&1){
((void(__cdecl *)(char *, int, unsigned short, bool))(0x69F1E0))("Test", 2000, 0, false);
}

if(GetAsyncKeyState(VK_F6)&1){
((__cdecl void(*)(void))(0x00438E90))();
}
*/

unsigned char* sunCoreBlue=(unsigned char*)0xB7B9B8;
unsigned char* sunCoreGreen=(unsigned char*)0xB7BA70;
unsigned char* sunCoreRed=(unsigned char*)0xB7BB28;

float *weatherInterpolationValue=(float*)0xC8130C;
short *weatherForcedType=(short *)0xC81318;
short *weatherNewType=(short *)0xC8131C;
short *weatherOldType=(short *)0xC81320;
float *weatherWindynessForWeatherID=(float*)0x8D5E50;


float *CRenderer_ms_lodDistScale=(float*)0x8CD800;
float *CRenderer_ms_fCameraHeading=(float*)0xB7684C; // (-pi ... +pi)

uint32_t *CStreaming__memoryAvailable=(uint32_t*)0x8A5A80;


volatile int *CTimer_m_FrameCounter=(int*)0xB7CB4C;
volatile float *CTimer_game_FPS=(float*)0xB7CB50;

float *cameraFov=(float *)(0xB6F028+0xCB8); // by Vital (vitalrus) at plugin-sdk-ru (https://discord.gg/QEesDGb)
char *cameraFovLock=(char *)(0xb6fd15);
int *brightness=(int *)(0x00BA6784); // by CE
int *darkness=(int*)(0x8d5204);
char *darknessEnable=(char*)(0xc402c4);

char *CTheScripts__bDisplayHud=(char*)0xA444A0;
char *CHud__bScriptDontDisplayRadar=(char*)0xBAA3FB;
float *CHud__textBoxNumLines=(float*)0xBAA460;
int *CHud__helpMessageState=(int*)0xBAA474;


float *timeScale=(float*)0xB7CB64;
float *timeStep=(float*)0xB7CB5C;
float *timeStepMin=(float*)(0x5619AF+6);
float *timeStepNonClippedMin=(float*)(561967+6);

char *menuActive=(char*)0xBA6748+0x5C;
char *codePause=(char*)0xB7CB48;
char *userPause=(char*)0xB7CB49;
void **CWorld__Players=(void**)0xB7CD98;
uint8_t *CWorld__PlayerInFocus=(uint8_t*)0xB7CD74;


volatile unsigned short *clockSeconds=(unsigned short *)0xB70150;
volatile unsigned char *clockMinutes=(unsigned char *)0xB70152;
volatile unsigned char *clockHours=(unsigned char *)0xB70153;
CCamera *theCamera=(CCamera*)0xB6F028;
float *GAME_GRAVITY=(float*)0x863984;


int *LastScreenWidth=(int*)0xc9bee4;
int *LastScreenHeight=(int*)0xc9bee8;

int *screenHudMenuWidth=(int*)0xc17044;
int *screenHudMenuHeight=(int*)0xc17048;

void setGravity(float n){
*GAME_GRAVITY=n;
}
void restoreGravity(){
*GAME_GRAVITY=0.0080000004;
}


void waitNFrames(int n){
int to=n+(*CTimer_m_FrameCounter);
while(*CTimer_m_FrameCounter<to){
Sleep(10);
}
}


void setDrawingDistance(float dist){
*CRenderer_ms_lodDistScale=dist;// use immediatly
*(float*)0xBA6788=dist;// draw distance in options, saved at exit
}


void setWindynessForCurrentWeather(float val){
if(*weatherForcedType>=0){
weatherWindynessForWeatherID[*weatherForcedType]=val;
}
if(*weatherNewType>=0){
weatherWindynessForWeatherID[*weatherNewType]=val;
}
if(*weatherNewType>=0){
weatherWindynessForWeatherID[*weatherOldType]=val;
}
sprintf(screenMessage,"Windyness %.3f for weathers: %d, %d, %d",val,*weatherForcedType,*weatherNewType,*weatherOldType);
MessageJumpQ(screenMessage, 1000, 0, false);
}

void setAspectRatio(float aspect){
/* Apect ratio:
00862CE4=1.77777
008595F0=1.25
0085F09C=1.333
Patch them all!!!
*/
*(float*)(0x862CE4)=aspect;
*(float*)(0x8595F0)=aspect;
*(float*)(0x85F09C)=aspect;
//MessageJumpQ("Aspect patched", 1000, 0, false);
}

void setGameFPSLimit(int fps){
// frame limits
// use 105 for smooth game
*(int*)0x619626=fps; // from SA Limits Ajuster
*(int*)0xC1704C=fps;
}

CVector *getPlayerVector(){
void *cped=getPlayerCped();
void *cped_matrix=*(void**)(cped+0x14); // matrix???
CVector *player_pos=(CVector *)(cped_matrix+0x30);
return(player_pos);
}

void *getPlayerCped(){
void *cped=NULL,*player_info=(void*)(0xB7CD98); // (CPlayerInfo *)0xB7CD98
if(player_info){
cped=*(void**)(player_info+0); // CPlayerPed -> CPed -> CPhysical -> CEntity -> CPlaceable
}
return cped;
}

void cpedSetHeading(void *cped, float angle){ // -M_PI..+M_PI
CPlaceable__SetHeading(cped,angle);
*(float*)(cped+0x558)=angle; // rotate from?
*(float*)(cped+0x55C)=angle; // rotate to?
//CSimpleTransform *player_trans=(void*)(cped+0x4);
//player_trans->m_fHeading=angle;
}

void cpedSetVisibility(void *cped, int state){
if(state){
*(char *)(cped+0x474)&=~2; // remove bit "don't render"
} else {
*(char *)(cped+0x474)|=2; // set bit
}
}

void cpedSetHealth(void *cped, float h){
*(float*)(cped+0x540)=h; // health
}

char getVolumeSfx(){
return *(char*)0xB5FCCC;
}

void setVolumeSfx(int vol){ // vol 0..64
setEffectsMasterVolume((void*)0xB6BC90,vol);
}

void playSoundId(int id, CVector *pos){
reportMissionAudioEvent((void*)0xB6BC90,id,pos);
}

double drand_num(double max){
return((double)rand()/RAND_MAX*max);
}

void rollTime(){
*clockHours=drand_num(24.0);
*clockMinutes=drand_num(60.0);
*clockSeconds=drand_num(60.0);
sprintf(screenMessage,"Time: %02d:%02d:%02d",*clockHours,*clockMinutes,*clockSeconds);
//MessageJumpQ(screenMessage, 10000, 0, false);
}

void flyTo(float tx, float ty, float tz, float heading, int interior, int need_detect_land){
setGravity(0);
void *cped=getPlayerCped();
CVector *player_pos=getPlayerVector();
float th=heading/180.0*M_PI;
float fx=player_pos->x,fy=player_pos->y,fz=player_pos->z,fh=CPlaceable__GetHeading(cped);


*(int*)0xB72914=interior; // currentArea
*(char*)(cped+0x2F)=interior; //linked with
CStreaming__RemoveBuildingsNotInArea(interior);

float dist=sqrt(pow(fx-tx,2)+pow(fy-ty,2)+pow(fz-tz,2));
int steps=dist/10.0+1;
int q;
float p;
for(q=0;q<steps;q++){
p=(float)q/steps;
cpedSetHeading(cped,(th-fh)*p+fh);
player_pos->x=(tx-fx)*p+fx;
player_pos->y=(ty-fy)*p+fy;
player_pos->z=(tz-fz)*p+fz+sin(M_PI*p)*300.0;
sprintf(screenMessage,"%d/%d: %.3fx%.3fx%.3f",q,steps,player_pos->x,player_pos->y,player_pos->z);
MessageJumpQ(screenMessage, 100, 0, false);
waitNFrames(1);
}

// slow landing
if(need_detect_land){
tz=findGroundZForCoord(tx,ty)+2.0;
}

for(q=10;q>=0;q--){
player_pos->x=tx;
player_pos->y=ty;
player_pos->z=tz+(float)q/3.0;
waitNFrames(1);
}

restoreGravity();
sprintf(screenMessage,"You landed at: %.3fx%.3fx%.3f",player_pos->x,player_pos->y,player_pos->z);
MessageJumpQ(screenMessage, 100, 0, false);

}

void setCameraFromToFov(float sx, float sy, float sz, float tx, float ty, float tz, float fov){
theCamera->m_nTypeOfSwitch=2;
theCamera->m_bStartInterScript=1;
theCamera->m_bLookingAtVector=1;
theCamera->m_nModeToGoTo=15;
theCamera->m_bLookingAtPlayer=0;
theCamera->m_nWhoIsInControlOfTheCamera=1;
theCamera->m_bGarageFixedCamPositionSet=0;
theCamera->m_bBlockZoom=1;
theCamera->m_vecFixedModeVector.x=tx;
theCamera->m_vecFixedModeVector.y=ty;
theCamera->m_vecFixedModeVector.z=tz;
theCamera->m_vecFixedModeSource.x=sx;
theCamera->m_vecFixedModeSource.y=sy;
theCamera->m_vecFixedModeSource.z=sz;
theCamera->m_fFOVNew=fov;
}


void setCameraFromState(CCamera *state){
theCamera->m_nTypeOfSwitch=state->m_nTypeOfSwitch;
theCamera->m_bStartInterScript=state->m_bStartInterScript;
theCamera->m_bLookingAtVector=state->m_bLookingAtVector;
theCamera->m_nModeToGoTo=state->m_nModeToGoTo;
theCamera->m_bLookingAtPlayer=state->m_bLookingAtPlayer;
theCamera->m_nWhoIsInControlOfTheCamera=state->m_nWhoIsInControlOfTheCamera;
theCamera->m_bGarageFixedCamPositionSet=state->m_bGarageFixedCamPositionSet;
theCamera->m_bBlockZoom=state->m_bBlockZoom;
theCamera->m_vecFixedModeVector.x=state->m_vecFixedModeVector.x;
theCamera->m_vecFixedModeVector.y=state->m_vecFixedModeVector.y;
theCamera->m_vecFixedModeVector.z=state->m_vecFixedModeVector.z;
theCamera->m_vecFixedModeSource.x=state->m_vecFixedModeSource.x;
theCamera->m_vecFixedModeSource.y=state->m_vecFixedModeSource.y;
theCamera->m_vecFixedModeSource.z=state->m_vecFixedModeSource.z;
theCamera->m_fFOVNew=state->m_fFOVNew;
}

void setCameraDefault(){
theCamera->m_bJustInitalised=1;
theCamera->m_bTransitionState=0;
theCamera->m_bLookingAtPlayer=1;
theCamera->m_bLookingAtVector=0;
theCamera->m_bGarageFixedCamPositionSet=0;
theCamera->m_nWhoIsInControlOfTheCamera=0;
theCamera->m_bUseNearClipScript=0;
theCamera->m_bAllow1rstPersonWeaponsCamera=0;
theCamera->m_bUseScriptZoomValuePed=0;
theCamera->m_bUseScriptZoomValueCar=0;
theCamera->m_fAvoidTheGeometryProbsTimer=0.0;
theCamera->m_bStartInterScript=1;
theCamera->m_bCameraJustRestored=1;
theCamera->m_fFOVNew=60.0;
}


void setCameraProjection(int t){
// t=1 = pespective
// t=2 = parallel
void *scene=(void*)0xC17038;
void *rwcam=*(void**)(scene+4);
*(int*)(rwcam+0x14)=t;
}

float findGroundZForCoordByFile(float ox, float oy){

int px,py,p;
if(!height_map){height_map=fopen("heightmap.bin","rb");}
if(height_map){
px=(int)ox+3000.0;
py=(int)oy+3000.0;
if(px>=0 && px<6000 && py>=0 && py<6000){
fseek(height_map,(px+py*6000)*4,SEEK_SET);
fread(&ox,1,4,height_map);
return(ox);
}
}
return -20000.0;
}

void findGroundZForCoordRangeByFile(int px1, int py1, int px2, int py2, float *ret_min, float *ret_max){
int px,py,ox,oy,p;
if(!height_map){height_map=fopen("heightmap.bin","rb");}
//logme("try find height for range: %dx%d ... %dx%d",px1,py1,px2,py2);
float min=20000,max=-20000,val;
if(height_map){
for(oy=py1;oy<=py2;oy++){
for(ox=px1;ox<=px2;ox++){
px=(int)ox+3000.0;
py=(int)oy+3000.0;
if(px>=0 && px<6000 && py>=0 && py<6000){
fseek(height_map,(px+py*6000)*4,SEEK_SET);
fread(&val,1,4,height_map);
if(min>val){min=val;}
if(max<val){max=val;}
//logme("reading coord %dx%d (%dx%d), val:%.3f, range:%.3f...%.3f",px,py,ox,oy,val,min,max);
}
}
}
}
*ret_min=min;
*ret_max=max;
}


