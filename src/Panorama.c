#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "ApiGtaSA.h"

int width=128;
int height=96;

// TODO: add more fun points of game, game is so beautiful!
typedef struct{
float x,y,z,heading;
}PLACES;

PLACES places[]={
{1932.6908, -1341.0327, 17.923, 90},
{1522.7098, -1660.6588, 13.5392, 12},
{-2400.6841, -1633.3374, 520.8088, 145},
{-2696.0698, 1933.2408, 217.2739, 204},
{-2666.7161, 1595.2468, 217.2739, 263},
{-640.8665, 2591.4866, 90.3694, 206},
{1623.3267, -2282.9629, 87.2895, 100},
{1192.7832, -2037.0789, 69.0078, 89},
{1083.4121, -2033.5021, 69.0078, 44},
{633.8432, -1499.2915, 14.7032, 44},
{1103.0865, 1567.7642, 47.1623, 351},
{181.1962, 1266.635, 22.0121, 346}, // oil rafinery (animated)
{-290.4352, 1607.5458, 69.2709, 47},
{-675.8931, 2043.8042, 77.8984, 342},
{-1567.4363, 2574.2236, 68.2592, 313},
{1452.2943, -757.7571, 95.043, 190},
{1110.7382, -834.2145, 112.5787, 223}, // sex shop
{1772.3441, -1308.8655, 131.7344, 45},
{154.5134, -1951.1897, 51.3437, 65},
{2466.3037, 2148.748, 10.6719, 45},
{2304.2021, 2305.6111, 10.8203, 12},
{2304.2021, 2305.6111, 10.8203, 301}
};
int places_current=0;
int places_count=sizeof(places)/sizeof(places[0]);




float mex=2083.0;
float mey=-1263.0;
float mez=23.0;

float fov=90.0;
int fov_find=0;

//char *screensSource="v:\\Screenshots";
//char *screensTmp="v:\\Screenshots";
char *screensSource="T:\\GTA-trains\\Screenshots";
char *panoRoot="T:\\GTA-trains\\Pano";


char panoName[1024];
char path[1024];
char tmp[256];



double drand(){
return((double)rand()/RAND_MAX);
}

void removeTrash(){
WIN32_FIND_DATA ffd;
sprintf(path,"%s\\*.png",screensSource);
HANDLE dd=FindFirstFileA(path, &ffd);
if(dd!=INVALID_HANDLE_VALUE){
do{
if(!(ffd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
printf("we found trash %s, removing\n",ffd.cFileName);
sprintf(path,"%s\\%s",screensSource,ffd.cFileName);
DeleteFile(path);
}
}while(FindNextFile(dd,&ffd)!=0);
}
}

void waitForFile(char *outname){
WIN32_FIND_DATA ffd;
int ret;
char path[1024];
sprintf(path,"%s\\*.png",screensSource);
while(1){
HANDLE dd=FindFirstFileA(path, &ffd);
if(dd!=INVALID_HANDLE_VALUE){
sprintf(path,"%s\\%s",screensSource,ffd.cFileName);
ret=MoveFileEx(path,outname,MOVEFILE_WRITE_THROUGH);
FindClose(dd);
if(ret!=0){
return;
}
}
Sleep(100);
}
}

void createPano(){


double fov=125.0;
int cols=360.0/fov*3.0+1.0;
int rows=180.0/fov*3.0+1.0;

//cols*=10;

int totalFrames=cols*rows;

char framename[32];

void *player_data=(void*)(0xB7CD98);
void *cped=*(void**)(player_data+0);
void *cped_xyz=*(void**)(cped+0x14); // matrix???
float player_x=*(float*)(cped_xyz+0x30);
float player_y=*(float*)(cped_xyz+0x34);
float player_z=*(float*)(cped_xyz+0x38);

double sx=player_x,sy=player_y,sz=player_z+10.0;
double tx=0,ty=0,tz=0;


removeTrash();
sprintf(panoName,"pano-%d-%dx%dx%d",(int)time(0),(int)player_x,(int)player_y,(int)player_z);
sprintf(path,"%s\\%s",panoRoot,panoName);
CreateDirectory(path,NULL);

sprintf(path,"%s\\%s\\pano-build.bat",panoRoot,panoName);
FILE *pto=fopen(path,"wt");
fprintf(pto,"\"V:\\h\\Hugin-2023.0.0-win64\\nona\" -o pano pano.pto");
fclose(pto);

sprintf(path,"%s\\%s\\pano.pto",panoRoot,panoName);
pto=fopen(path,"wt");
fprintf(pto,"p w2048 h1024 f2 v360 n\"PNG\" R0 T\"UINT8\"\n");
fprintf(pto,"m i6\n");



//sprintf(tmp,"me at: %.3fx%.3fx%.3f",player_x,player_y,player_z);
//MessageJumpQ(tmp, 2000, 0, false);


// When we start creating panorama?
int panoClockHours=*clockHours;
int panoClockMinutes=*clockMinutes;
int panoClockSeconds=0;

// Save current camera state to restore it again after screenshot session
void *camState=malloc(sizeof(CCamera));
memcpy(camState,theCamera,sizeof(CCamera));

// Save weather state
float panoWeatherInterpolator=*weatherInterpolationValue;
short panoWeatherForced=*weatherForcedType;
short panoWeatherNewType=*weatherNewType;
short panoWeatherOldType=*weatherOldType;

// Disable audio
setVolume(0);

double xrotth;
double yrotth;
double sphereRadius=10.0;
double radius1;
int q;


setAspectRatio(1.77777777777777777);
setWindynessForCurrentWeater(200);


// First pass, precache
*darkness=200;
*darknessEnable=1;
MessageJumpQ("Creating Panorama, please wait!", 1000, 0, false);

for(q=0;q<200;q+=1){
if(GetAsyncKeyState(VK_F7)&1){goto pano_finish;}

xrotth=(360.0*10.0*q/200.0)/180.0*M_PI;
yrotth=(89.0-178.0*q/200.0)/180.0*M_PI;

radius1=cos(yrotth)*sphereRadius;
tz=sz+sin(yrotth)*sphereRadius;
tx=sx+sin(xrotth)*radius1;
ty=sy+cos(xrotth)*radius1;

*clockHours=panoClockHours;
*clockMinutes=panoClockMinutes;
*clockSeconds=panoClockSeconds;

setCameraFromToFov(sx,sy,sz,tx,ty,tz,90.0);
Sleep(33);
}

// Second pass
*darkness=0;
*darknessEnable=0;

for(q=0;q<totalFrames;q+=1){
if(GetAsyncKeyState(VK_F7)&1){goto pano_finish;}
xrotth=(360.0*rows*q/totalFrames)/180.0*M_PI;
yrotth=(89.0-178.0*q/totalFrames)/180.0*M_PI;

sprintf(framename,"frame-%.7d.png",q);
fprintf(pto,"i f0 w%d h%d r%f p%f y%f v%f n\"%s\"\n",width,height,0.0,(float)(89.0-178.0*q/totalFrames),(float)(360.0*rows*q/totalFrames),fov,framename);

radius1=cos(yrotth)*sphereRadius;
tz=sz+sin(yrotth)*sphereRadius;
tx=sx+sin(xrotth)*radius1;
ty=sy+cos(xrotth)*radius1;

// Resync clock to initial panorama state
*clockHours=panoClockHours;
*clockMinutes=panoClockMinutes;
*clockSeconds=panoClockSeconds;

// restore/refresh weather state, like was in start
*weatherInterpolationValue=panoWeatherInterpolator;
*weatherForcedType=panoWeatherForced;
*weatherNewType=panoWeatherNewType;
*weatherOldType=panoWeatherOldType;

// Pause game time, we don't want any movements during screenshot session.
*timeScale=0.0;
*timeStep=0.0;

setCameraFromToFov(sx,sy,sz,tx,ty,tz,fov);

//sprintf(tmp,"%d/%d",q,totalFrames);
//MessageJumpQ(tmp, 400, 0, false);

// In early versions, I tried to save images via this call. But, unfortuanly, game always crashed after 178-180 saved images.
//char *take_photo=(char*)0x00C8A7C1;
//*take_photo=1;

// Wait for new redraw cycle
Sleep(80);

// Stop time even more, setting game to pause while creating screenshot
*codePause=1;
*userPause=1;

keybd_event(VK_MENU,0xb8,0,0);
keybd_event(VK_SNAPSHOT,0,0,0);
keybd_event(VK_SNAPSHOT,0,KEYEVENTF_KEYUP,0);
keybd_event(VK_MENU,0xb8,KEYEVENTF_KEYUP,0);

//sprintf(path,"%s\\tmp.png",screensTmp);
sprintf(path,"%s\\%s\\%s",panoRoot,panoName,framename);
waitForFile(path);

// launch ffmpeg?
//sprintf(cmd," -v 0 -i \"%s\" -s %dx%d -y \"%s\\%s\\%s\"",path,width,height,panoRoot,panoName,framename);
//ShellExecute(NULL,"open","ffmpeg",cmd,NULL,SW_HIDE);
//DeleteFile(path);

// Remove pause, let's game tick again
*codePause=0;
*userPause=0;
}

// Play finish sounds
setVolume(64);
CVector soundPos;
soundPos.x=sx;
soundPos.y=sy;
soundPos.z=sz;
for(q=0;q<20;q++){
playSoundId(1083,&soundPos);
Sleep(200);
}


pano_finish:

fclose(pto);

// Restore time speed
*timeScale=1.0;
*timeStep=1.0;
*codePause=0;
*userPause=0;

// Remove darkness, if any
*darkness=0;
*darknessEnable=0;

// Restore camera
memcpy(theCamera,camState,sizeof(CCamera));
free(camState);

// Restore audio, if any
setVolume(64);

// TODO: exit game at finish (menu option)


}









DWORD WINAPI MyASIThread(LPVOID lpParam){

while(1){

if(GetAsyncKeyState(VK_F4)&1){

CVector pos;
pos.x=181.1962;
pos.y=1266.635;
pos.z=22.0121;

playSoundId(1083,&pos);

static int is_pause=1;
is_pause^=1;
if(is_pause){
*timeScale=0.0;
*timeStep=0.0;

float *CTimer_game_FPS=(float*)0xB7CB50;
float *CTimer_ms_fTimeStepNonClipped=(float*)0xB7CB58;
float *CTimer_ms_fTimeStep=(float*)0xB7CB5C;
float *CTimer_ms_fTimeScale=(float*)0xB7CB64;
unsigned int *CTimer_m_snPreviousTimeInMilliseconds=(unsigned int*)0xB7CB78;
unsigned int *CTimer_m_snTimeInMillisecondsPauseMode=(unsigned int*)0xB7CB7C;
unsigned int *CTimer_m_snTimeInMillisecondsNonClipped=(unsigned int*)0xB7CB80;
unsigned int *CTimer_m_snTimeInMilliseconds=(unsigned int*)0xB7CB84;
unsigned int *CTimer_m_snPPPPreviousTimeInMilliseconds=(unsigned int*)0xB7CB6C;
unsigned int *CTimer_m_snPPPreviousTimeInMilliseconds=(unsigned int*)0xB7CB70;
unsigned int *CTimer_m_snPPreviousTimeInMilliseconds=(unsigned int*)0xB7CB74;
unsigned int *CTimer_m_snPreviousTimeInMillisecondsNonClipped=(unsigned int*)0xB7CB68;
float *CTimer_ms_fOldTimeStep=(float*)0xB7CB54;

int *_renderTimerPauseCoun=(int*)0xB7CB44;
int *_renderStartTimeLow=(int*)0xB7CB38;
int *_renderStartTimeHigh=(int*)0xB7CB3C;

*CTimer_ms_fTimeStepNonClipped=0;
*CTimer_ms_fTimeStep=0;
*CTimer_ms_fTimeScale=0;
*CTimer_m_snPreviousTimeInMilliseconds=0;
*CTimer_m_snTimeInMillisecondsPauseMode=0;
*CTimer_m_snTimeInMillisecondsNonClipped=0;
*CTimer_m_snTimeInMilliseconds=0;
*CTimer_m_snPPPPreviousTimeInMilliseconds=0;
*CTimer_m_snPPPreviousTimeInMilliseconds=0;
*CTimer_m_snPPreviousTimeInMilliseconds=0;
*CTimer_m_snPreviousTimeInMillisecondsNonClipped=0;
*CTimer_ms_fOldTimeStep=0;

*_renderTimerPauseCoun=0;
*_renderStartTimeLow=0;
*_renderStartTimeHigh=0;

setVolume(0);

} else {
*timeScale=1.0;
*timeStep=1.0;
setVolume(64);

}


}

if(GetAsyncKeyState(VK_F5)&1){
flyTo(181.1962, 1266.635, 22.0121, 346.0);
forceWeatherNow(19);
rollTime();
//*((float*)0xC812F0)=1000000.0;// bending value &CWeather::Wind 
//*((float*)0xC02DB8)=1000.0;
CVector pos;
pos.x=181.1962;
pos.y=1266.635;
pos.z=22.0121;

playSoundId(1083,&pos);

}

if(GetAsyncKeyState(VK_F6)&1){
rollTime();
}

if(GetAsyncKeyState(VK_F7)&1){
createPano();
}

if(GetAsyncKeyState(VK_F8)&1){
places_current++;
places_current%=places_count;
//float haveZ=findGroundZForCoord(places[places_current].x,places[places_current].y);
//requestCollision(&newpos,0xB72914);
flyTo(places[places_current].x,places[places_current].y,places[places_current].z,places[places_current].heading);
}

if(GetAsyncKeyState(VK_F9)&1){
flyTo(drand()*5000.0-2500.0,drand()*5000.0-2500.0,300.0,drand()*360.0);
}

if(GetAsyncKeyState(VK_F10)&1){
int weatherID=drand()*23.0;
forceWeatherNow(weatherID);
sprintf(tmp,"New weather ID: %d",weatherID);
MessageJumpQ(tmp, 10000, 0, false);
}


if(GetAsyncKeyState(VK_F11)&1){
/*
*sunCoreBlue=drand()*255.0;
*sunCoreGreen=drand()*255.0;
*sunCoreRed=drand()*255.0;*/



//MessageJumpQ(tmp, 1000, 0, false);

char filename[256];
static int screen_id=0;

sprintf(filename,"T:\\GTA-trains\\Screenshots\\screenshot-%d.jpg",screen_id++);
JPegCompressScreenToFile((void*)(0xC17038+4), filename);
MessageJumpQ(filename, 3000, 0, false);

continue;

setAspectRatio(1.77777777777777777);
setWindynessForCurrentWeater(200);
setDrawingDistance(0.01);

char *sunBlockedByClouds=(char *)0xC3E030;
int *sunChangeBrightnessImmediately=(int*)0xC3E034;
unsigned int *sunNumCoronas=(unsigned int*)0xC3E038;
float *sunLightsMult=(float*)0x8D4B5C;
unsigned int *sunMoonSize=(unsigned int *)0x8D4B60;

*sunBlockedByClouds=0;
*sunChangeBrightnessImmediately=1;
//*sunNumCoronas=drand()*64.0;
*sunLightsMult=drand()*20.0;
sprintf(tmp,"changed, moon:%d, h1:%.3f,h2:%.3f",*sunMoonSize,1.0,2.0);
//MessageJumpQ(tmp, 1000, 0, false);

float *CTimeCycle_m_BrightnessAddedToAmbientBlue_=(float*)0xB79E30;
float *CTimeCycle_m_BrightnessAddedToAmbientGreen_=(float*)0xB79E34;
float *CTimeCycle_m_BrightnessAddedToAmbientRed_=(float*)0xB79E38;
int *CTimeCycle_m_ExtraColourInter_=(int*)0xB79E3C;
int *CTimeCycle_m_ExtraColourWeatherType_=(int*)0xB79E40;
int *CTimeCycle_m_ExtraColour_=(int*)0xB79E44;
int *CTimeCycle_m_FogReduction_=(int*)0xB79E48;
float *CTimeCycle_m_fShadowDisplacementY_=(float*)0xB79E50;
float *CTimeCycle_m_fShadowDisplacementX_=(float*)0xB79E90;
float *CTimeCycle_m_fShadowSideY_=(float*)0xB79ED0;
float *CTimeCycle_m_fShadowSideX_=(float*)0xB79F10;
float *CTimeCycle_m_fShadowFrontY_=(float*)0xB79F50;
float *CTimeCycle_m_fShadowFrontX_=(float*)0xB79F90;
int *CTimeCycle_m_CurrentStoredValue_=(int*)0xB79FD0;
unsigned char *CTimeCycle_m_nDirectionalMult_=(unsigned char*)0xB79FD8;
unsigned char *CTimeCycle_m_nWaterFogAlpha_=(unsigned char*)0xB7A090;
unsigned char *CTimeCycle_m_nHighLightMinIntensity_=(unsigned char*)0xB7A148;
unsigned char *CTimeCycle_m_fCloudAlpha_=(unsigned char*)0xB7A200;
unsigned char *CTimeCycle_m_fPostFx2Alpha_=(unsigned char*)0xB7A2B8;
unsigned char *CTimeCycle_m_fPostFx2Blue_=(unsigned char*)0xB7A370;
unsigned char *CTimeCycle_m_fPostFx2Green_=(unsigned char*)0xB7A428;
unsigned char *CTimeCycle_m_fPostFx2Red_=(unsigned char*)0xB7A4E0;
unsigned char *CTimeCycle_m_fPostFx1Alpha_=(unsigned char*)0xB7A598;
unsigned char *CTimeCycle_m_fPostFx1Blue_=(unsigned char*)0xB7A650;
unsigned char *CTimeCycle_m_fPostFx1Green_=(unsigned char*)0xB7A708;
unsigned char *CTimeCycle_m_fPostFx1Red_=(unsigned char*)0xB7A7C0;
unsigned char *CTimeCycle_m_fWaterAlpha_=(unsigned char*)0xB7A878;
unsigned char *CTimeCycle_m_fWaterBlue_=(unsigned char*)0xB7A930;
unsigned char *CTimeCycle_m_fWaterGreen_=(unsigned char*)0xB7A9E8;
unsigned char *CTimeCycle_m_fWaterRed_=(unsigned char*)0xB7AAA0;
unsigned char *CTimeCycle_m_nFluffyCloudsBottomBlue_=(unsigned char*)0xB7AB58;
unsigned char *CTimeCycle_m_nFluffyCloudsBottomGreen_=(unsigned char*)0xB7AC10;
unsigned char *CTimeCycle_m_nFluffyCloudsBottomRed_=(unsigned char*)0xB7ACC8;
unsigned char *CTimeCycle_m_nLowCloudsBlue_=(unsigned char*)0xB7AD80;
unsigned char *CTimeCycle_m_nLowCloudsGreen_=(unsigned char*)0xB7AE38;
unsigned char *CTimeCycle_m_nLowCloudsRed_=(unsigned char*)0xB7AEF0;
unsigned char *CTimeCycle_m_fLightsOnGroundBrightness_=(unsigned char*)0xB7AFA8;
unsigned char *CTimeCycle_m_nPoleShadowStrength_=(unsigned char*)0xB7B340;
unsigned char *CTimeCycle_m_nLightShadowStrength_=(unsigned char*)0xB7B3F8;
unsigned char *CTimeCycle_m_nShadowStrength_=(unsigned char*)0xB7B4B0;
char *CTimeCycle_m_fSpriteBrightness_=(char*)0xB7B568;
char *CTimeCycle_m_fSpriteSize_=(char*)0xB7B620;
char *CTimeCycle_m_fSunSize_=(char*)0xB7B6D8;
unsigned char *CTimeCycle_m_nSunCoronaBlue_=(unsigned char*)0xB7B790;
unsigned char *CTimeCycle_m_nSunCoronaGreen_=(unsigned char*)0xB7B848;
unsigned char *CTimeCycle_m_nSunCoronaRed_=(unsigned char*)0xB7B900;
unsigned char *CTimeCycle_m_nSunCoreBlue_=(unsigned char*)0xB7B9B8;
unsigned char *CTimeCycle_m_nSunCoreGreen_=(unsigned char*)0xB7BA70;
unsigned char *CTimeCycle_m_nSunCoreRed_=(unsigned char*)0xB7BB28;
unsigned char *CTimeCycle_m_nSkyBottomBlue_=(unsigned char*)0xB7BBE0;
unsigned char *CTimeCycle_m_nSkyBottomGreen_=(unsigned char*)0xB7BC98;
unsigned char *CTimeCycle_m_nSkyBottomRed_=(unsigned char*)0xB7BD50;
unsigned char *CTimeCycle_m_nSkyTopBlue_=(unsigned char*)0xB7BE08;
unsigned char *CTimeCycle_m_nSkyTopGreen_=(unsigned char*)0xB7BEC0;
unsigned char *CTimeCycle_m_nSkyTopRed_=(unsigned char*)0xB7BF78;
unsigned char *CTimeCycle_m_nAmbientBlue_Obj_=(unsigned char*)0xB7C030;
unsigned char *CTimeCycle_m_nAmbientGreen_Obj_=(unsigned char*)0xB7C0E8;
unsigned char *CTimeCycle_m_nAmbientRed_Obj_=(unsigned char*)0xB7C1A0;
unsigned char *CTimeCycle_m_nAmbientBlue_=(unsigned char*)0xB7C258;
unsigned char *CTimeCycle_m_nAmbientGreen_=(unsigned char*)0xB7C310;
unsigned char *CTimeCycle_m_nAmbientRed_=(unsigned char*)0xB7C3C8;
unsigned int *CTimeCycle_m_NumBoxes_=(unsigned int*)0xB7C480;
unsigned int *CTimeCycle_m_bExtraColourOn_=(unsigned int*)0xB7C484;



*CTimeCycle_m_BrightnessAddedToAmbientBlue_=1.0*drand();
*CTimeCycle_m_BrightnessAddedToAmbientGreen_=1.0*drand();
*CTimeCycle_m_BrightnessAddedToAmbientRed_=1.0*drand();
*CTimeCycle_m_ExtraColourInter_=1024.0*drand();
*CTimeCycle_m_ExtraColourWeatherType_=1024.0*drand();
*CTimeCycle_m_ExtraColour_=1024.0*drand();
*CTimeCycle_m_FogReduction_=1024.0*drand();
*CTimeCycle_m_fShadowDisplacementY_=1.0*drand();
*CTimeCycle_m_fShadowDisplacementX_=1.0*drand();
*CTimeCycle_m_fShadowSideY_=1.0*drand();
*CTimeCycle_m_fShadowSideX_=1.0*drand();
*CTimeCycle_m_fShadowFrontY_=1.0*drand();
*CTimeCycle_m_fShadowFrontX_=1.0*drand();
*CTimeCycle_m_CurrentStoredValue_=1024.0*drand();
*CTimeCycle_m_nDirectionalMult_=255.0*drand();
*CTimeCycle_m_nWaterFogAlpha_=255.0*drand();
*CTimeCycle_m_nHighLightMinIntensity_=255.0*drand();
*CTimeCycle_m_fCloudAlpha_=255.0*drand();
*CTimeCycle_m_fPostFx2Alpha_=255.0*drand();
*CTimeCycle_m_fPostFx2Blue_=255.0*drand();
*CTimeCycle_m_fPostFx2Green_=255.0*drand();
*CTimeCycle_m_fPostFx2Red_=255.0*drand();
*CTimeCycle_m_fPostFx1Alpha_=255.0*drand();
*CTimeCycle_m_fPostFx1Blue_=255.0*drand();
*CTimeCycle_m_fPostFx1Green_=255.0*drand();
*CTimeCycle_m_fPostFx1Red_=255.0*drand();
*CTimeCycle_m_fWaterAlpha_=255.0*drand();
*CTimeCycle_m_fWaterBlue_=255.0*drand();
*CTimeCycle_m_fWaterGreen_=255.0*drand();
*CTimeCycle_m_fWaterRed_=255.0*drand();
*CTimeCycle_m_nFluffyCloudsBottomBlue_=255.0*drand();
*CTimeCycle_m_nFluffyCloudsBottomGreen_=255.0*drand();
*CTimeCycle_m_nFluffyCloudsBottomRed_=255.0*drand();
*CTimeCycle_m_nLowCloudsBlue_=255.0*drand();
*CTimeCycle_m_nLowCloudsGreen_=255.0*drand();
*CTimeCycle_m_nLowCloudsRed_=255.0*drand();
*CTimeCycle_m_fLightsOnGroundBrightness_=255.0*drand();
*CTimeCycle_m_nPoleShadowStrength_=255.0*drand();
*CTimeCycle_m_nLightShadowStrength_=255.0*drand();
*CTimeCycle_m_nShadowStrength_=255.0*drand();
*CTimeCycle_m_fSpriteBrightness_=255.0*drand();
*CTimeCycle_m_fSpriteSize_=25.0*drand();
*CTimeCycle_m_fSunSize_=25.0*drand();
*CTimeCycle_m_nSunCoronaBlue_=255.0*drand();
*CTimeCycle_m_nSunCoronaGreen_=255.0*drand();
*CTimeCycle_m_nSunCoronaRed_=255.0*drand();
*CTimeCycle_m_nSunCoreBlue_=255.0*drand();
*CTimeCycle_m_nSunCoreGreen_=255.0*drand();
*CTimeCycle_m_nSunCoreRed_=255.0*drand();
*CTimeCycle_m_nSkyBottomBlue_=255.0*drand();
*CTimeCycle_m_nSkyBottomGreen_=255.0*drand();
*CTimeCycle_m_nSkyBottomRed_=255.0*drand();
*CTimeCycle_m_nSkyTopBlue_=255.0*drand();
*CTimeCycle_m_nSkyTopGreen_=255.0*drand();
*CTimeCycle_m_nSkyTopRed_=255.0*drand();
*CTimeCycle_m_nAmbientBlue_Obj_=255.0*drand();
*CTimeCycle_m_nAmbientGreen_Obj_=255.0*drand();
*CTimeCycle_m_nAmbientRed_Obj_=255.0*drand();
*CTimeCycle_m_nAmbientBlue_=255.0*drand();
*CTimeCycle_m_nAmbientGreen_=255.0*drand();
*CTimeCycle_m_nAmbientRed_=255.0*drand();
*CTimeCycle_m_bExtraColourOn_=1.0*drand();


float *_CWeather_TrafficLightsBrightness=(float*)0xC812A8;
float *_CWeather_Earthquake=(float*)0xC81340;
unsigned int *_CWeather_CurrentRainParticleStrength=(unsigned int*)0xC812B0;
unsigned int *_CWeather_LightningStartY=(unsigned int*)0xC812B4;
unsigned int *_CWeather_LightningStartX=(unsigned int*)0xC812B8;
int *_CWeather_LightningFlashLastChange=(int*)0xC812BC;
int *_CWeather_WhenToPlayLightningSound=(int*)0xC812C0;
unsigned int *_CWeather_LightningDuration=(unsigned int*)0xC812C4;
unsigned int *_CWeather_LightningStart=(unsigned int*)0xC812C8;
float *_CWeather_HeadLightsSpectrum=(float*)0xC812D0;
float *_CWeather_WaterFogFXControl=(float*)0xC81338;
float *_CWeather_HeatHazeFXControl=(float*)0xC812D8;
float *_CWeather_HeatHaze=(float*)0xC812DC;
float *_CWeather_SunGlare=(float*)0xC812E0;
float *_CWeather_Rainbow=(float*)0xC812E4;
float *_CWeather_Wavyness=(float*)0xC812E8;
float *_CWeather_WindClipped=(float*)0xC812EC;
float *_CWeather_Wind=(float*)0xC812F0;
float *_CWeather_Sandstorm=(float*)0xC812F4;
float *_CWeather_Rain=(float*)0xC81324;
float *_CWeather_InTunnelness=(float*)0xC81334;
float *_CWeather_WaterDepth=(float*)0xC81330;
float *_CWeather_UnderWaterness=(float*)0xC8132C;
float *_CWeather_ExtraSunnyness=(float*)0xC812F8;
float *_CWeather_Foggyness_SF=(float*)0xC812FC;
float *_CWeather_Foggyness=(float*)0xC81300;
float *_CWeather_CloudCoverage=(float*)0xC81304;
float *_CWeather_WetRoads=(float*)0xC81308;
float *_CWeather_InterpolationValue=(float*)0xC8130C;

*_CWeather_TrafficLightsBrightness=1.0*drand();
*_CWeather_Earthquake=0;
*_CWeather_CurrentRainParticleStrength=1024.0*drand();
*_CWeather_LightningStartY=1024.0*drand();
*_CWeather_LightningStartX=1024.0*drand();
*_CWeather_LightningFlashLastChange=1024.0*drand();
*_CWeather_WhenToPlayLightningSound=1024.0*drand();
*_CWeather_LightningDuration=1024.0*drand();
*_CWeather_LightningStart=1024.0*drand();
*_CWeather_HeadLightsSpectrum=1.0*drand();
*_CWeather_WaterFogFXControl=1.0*drand();
*_CWeather_HeatHazeFXControl=1.0*drand();
*_CWeather_HeatHaze=100.0*drand();
*_CWeather_SunGlare=100.0*drand();
*_CWeather_Rainbow=100.0*drand();
*_CWeather_Wavyness=100000.0*drand();
*_CWeather_WindClipped=1.0*drand();
*_CWeather_Wind=100.0*drand();
*_CWeather_Sandstorm=100.0*drand();
*_CWeather_Rain=100.0*drand();
*_CWeather_InTunnelness=1.0*drand();
*_CWeather_WaterDepth=1.0*drand();
*_CWeather_UnderWaterness=100.0*drand();
*_CWeather_ExtraSunnyness=100.0*drand();
*_CWeather_Foggyness_SF=100.0*drand();
*_CWeather_Foggyness=100.0*drand();
*_CWeather_CloudCoverage=100.0*drand();
*_CWeather_WetRoads=100.0*drand();
*_CWeather_InterpolationValue=1.0*drand();

int *weatherType_int=(int*)0x72A590;
*weatherType_int=20.0*drand();

*_CWeather_Earthquake=0;
*_CWeather_Wind=0;
*_CWeather_LightningDuration=0;


mex=drand()*2000.0-1000.0;
mey=drand()*2000.0-1000.0;
mez=findGroundZForCoord(mex,mey);
//requestCollision(&newpos,0xB72914);




}


Sleep(20);
}

}


