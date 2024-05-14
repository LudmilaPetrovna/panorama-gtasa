#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "ApiGtaSA.h"

typedef struct{
// clock
int clockHours;
int clockMinutes;
int clockSeconds;

// camera
CCamera *camState;

// weather
float weatherInterpolator;
short weatherForced;
short weatherNewType;
short weatherOldType;

// volume
int volumeSfx;

// dark overlay
int darkness;
int darknessEnable;

// ui
int displayHud;
int hideRadar;

void *cped;

}FROZENSTATE;

FROZENSTATE frozen={0};

void prepareFreeze(){

frozen.cped=getPlayerCped();

// When we start creating panorama?
frozen.clockHours=*clockHours;
frozen.clockMinutes=*clockMinutes;
frozen.clockSeconds=0;

// Save current camera state to restore it again after screenshot session
if(!frozen.camState){
frozen.camState=malloc(sizeof(CCamera));
}
memcpy(frozen.camState,theCamera,sizeof(CCamera));

// Save weather state
frozen.weatherInterpolator=(*weatherInterpolationValue)/2.0;
frozen.weatherForced=*weatherForcedType;
frozen.weatherNewType=*weatherNewType;
frozen.weatherOldType=*weatherOldType;

frozen.darkness=*darkness;
frozen.darknessEnable=*darknessEnable;
frozen.displayHud=*CTheScripts__bDisplayHud;
frozen.hideRadar=*CHud__bScriptDontDisplayRadar;
frozen.volumeSfx=getVolumeSfx();

*timeScale=0.0;
*timeStep=0.0;

cpedSetVisibility(frozen.cped, 0);

*(char *)0x96916D=1; // Infinite Health
*(char *)0x96916E=1; // Infinite Oxygen
}


void refreshFreeze(){
// Resync clock to initial panorama state
*clockHours=frozen.clockHours;
*clockMinutes=frozen.clockMinutes;
*clockSeconds=frozen.clockSeconds;

// restore/refresh weather state, like was in start
*weatherInterpolationValue=frozen.weatherInterpolator;
*weatherForcedType=frozen.weatherForced;
*weatherNewType=frozen.weatherNewType;
*weatherOldType=frozen.weatherOldType;

// Pause game time, we don't want any movements during screenshot session
*timeScale=0.0;
*timeStep=0.0;

// change cped
void *cped=getPlayerCped();
*(float*)(cped+0x540)=99999.99; // health


setGravity(0);
}


void restoreFreeze(){

// Restore camera
if(frozen.camState){
setCameraFromState(frozen.camState);
}

// Restore FPS
setGameFPSLimit(105);

// Restore Player visibility
cpedSetVisibility(frozen.cped, 1);

// Restore audio, if any
setVolumeSfx(frozen.volumeSfx);

// Restore time speed
*timeScale=1.0;
*timeStep=1.0;
*codePause=0;
*userPause=0;

// Remove darkness, if any
*darkness=frozen.darkness;
*darknessEnable=frozen.darknessEnable;

// Restore HUD
*CTheScripts__bDisplayHud=frozen.displayHud;
*CHud__bScriptDontDisplayRadar=frozen.hideRadar;

restoreGravity();

}


