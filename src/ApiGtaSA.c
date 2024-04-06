#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "ApiGtaSA.h"


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

float *cameraFov=(float *)(0xB6F028+0xCB8); // by Vital (vitalrus) at plugin-sdk-ru (https://discord.gg/QEesDGb)
char *cameraFovLock=(char *)(0xb6fd15);
int *brightness=(int *)(0x00BA6784); // by CE
int *darkness=(int*)(0x8d5204);
char *darknessEnable=(char*)(0xc402c4);

float *timeScale=(float*)0xB7CB64;
float *timeStep=(float*)0xB7CB5C;
char *codePause=(char*)0xB7CB48;
char *userPause=(char*)0xB7CB49;

unsigned short *clockSeconds=(unsigned short *)0xB70150;
unsigned char *clockMinutes=(unsigned char *)0xB70152;
unsigned char *clockHours=(unsigned char *)0xB70153;
CCamera *theCamera=(CCamera*)0xB6F028;
float *GAME_GRAVITY=(float*)0x863984;


void setVolume(int vol){ // vol 0..64
setEffectsMasterVolume((void*)0xB6BC90,vol);
}

void playSoundId(int id, CVector *pos){
reportMissionAudioEvent((void*)0xB6BC90,id,pos);
}

static double drand_num(double max){
return((double)rand()/RAND_MAX*max);
}

void rollTime(){
*clockHours=drand_num(24.0);
*clockMinutes=drand_num(60.0);
*clockSeconds=drand_num(60.0);
char tmp[256];
sprintf(tmp,"Time: %02d:%02d:%02d",*clockHours,*clockMinutes,*clockSeconds);
MessageJumpQ(tmp, 10000, 0, false);
}

void flyTo(float tx, float ty, float tz, float heading){
char tmp[256];
void *player_data=(void*)(0xB7CD98);
void *cped=*(void**)(player_data+0);
void *cped_xyz=*(void**)(cped+0x14); // matrix???
float *player_x=(float*)(cped_xyz+0x30);
float *player_y=(float*)(cped_xyz+0x34);
float *player_z=(float*)(cped_xyz+0x38);
*GAME_GRAVITY=0;

float fx=*player_x,fy=*player_y,fz=*player_z;

float dist=sqrt(pow(fx-tx,2)+pow(fy-ty,2)+pow(fz-tz,2));
int steps=dist/30.0+1;
int q;
float p;
for(q=0;q<steps;q++){
p=(float)q/steps;
*player_x=(tx-fx)*p+fx;
*player_y=(ty-fy)*p+fy;
*player_z=(tz-fz)*p+fz+sin(M_PI*p)*300.0;
sprintf(tmp,"%d/%d: %.3fx%.3fx%.3f",q,steps,*player_x,*player_y,*player_z);
MessageJumpQ(tmp, 100, 0, false);
Sleep(33);
}

// slow landing
tz=findGroundZForCoord(tx,ty)+2.0;
for(q=10;q>=0;q--){
*player_x=tx;
*player_y=ty;
*player_z=tz+(float)q/3.0;
Sleep(33);
}

*GAME_GRAVITY=0.0080000004;
sprintf(tmp,"You landed at: %.3fx%.3fx%.3f",*player_x,*player_y,*player_z);
MessageJumpQ(tmp, 10000, 0, false);
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


