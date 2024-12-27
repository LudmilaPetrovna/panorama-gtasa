#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <windows.h>

#include "ApiGtaSA.h"
#include "Freeze.h"
#include "SpawnObject.h"
#include "hilbert.h"
#include "Screenshot.h"
#include "log.h"
#include "toolbox.h"

// TODO: add more fun points of game, game is so beautiful!
typedef struct{
float x,y,z,heading;
int interior;
}PLACES;


PLACES tower={
1573.125, -1242.125, 277,36
};


PLACES places[]={
{1932.6908, -1341.0327, 17.923, 90},
{1522.7098, -1660.6588, 13.5392, 12},
{1726.98, -1632.87, 20.21, 183, 0}, // Los Santos Atrium (door bug), before door
{1726.18, -1641.10, 20.22, 179, 18}, // Los Santos Atrium (door bug), before door
{2488.91, -1666.91, 13.34, 262, 14}, // Intro screen
{2215.83, -1731.19, 13.40, 302, 0}, // Ganton Gym
{-730.2343,494.6384,1371.997,0.0,1}, // liberty city
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

typedef struct{
char *name;
int id;
}NOISE;

NOISE noises[]={
//{"SOUND_EXPLOSION", 1159},
{"SOUND_SWEETS_HORN", 1147},
{"SOUND_ROULETTE_ADD_CASH", 1083},
{"SOUND_ROULETTE_REMOVE_CASH", 1084}
};

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



void precachePanoPlace(double sx, double sy, double sz){
double tx=0,ty=0,tz=0;
double xrotth;
double yrotth;
double sphereRadius=10.0;
double radius1;
int q;

*darkness=100;
*darknessEnable=1;
setGameFPSLimit(105);

for(q=0;q<300;q+=1){
if(GetAsyncKeyState(VK_F7)&1){return;}

xrotth=(360.0*10.0*q/200.0)/180.0*M_PI;
yrotth=(89.0-178.0*q/200.0)/180.0*M_PI;

radius1=cos(yrotth)*sphereRadius;
tz=sz+sin(yrotth)*sphereRadius;
tx=sx+sin(xrotth)*radius1;
ty=sy+cos(xrotth)*radius1;

refreshFreeze();
setCameraFromToFov(sx,sy,sz,tx,ty,tz,3.0);
Sleep(33);
}

*darkness=200;
*darknessEnable=1;
for(q=0;q<300;q+=1){
if(GetAsyncKeyState(VK_F7)&1){return;}

xrotth=(360.0*10.0*q/300.0)/180.0*M_PI;
yrotth=(89.0-178.0*q/300.0)/180.0*M_PI;

radius1=cos(yrotth)*sphereRadius;
tz=sz+sin(yrotth)*sphereRadius;
tx=sx+sin(xrotth)*radius1;
ty=sy+cos(xrotth)*radius1;

refreshFreeze();
setCameraFromToFov(sx,sy,sz,tx,ty,tz,90.0);
Sleep(33);
}


}


void createPano(){

setVolumeSfx(0);
double jumpTo=0.0;
double fov=60.0;
int cols=360.0/fov*3.0+1.0;
int rows=180.0/fov*3.0+1.0;

int totalFrames=cols*rows;

char framename[32];
void *cped=getPlayerCped();
CVector *player_pos=getPlayerVector();

double sx=player_pos->x,sy=player_pos->y,sz=player_pos->z+jumpTo;
double tx=0,ty=0,tz=0;
double xrotth;
double yrotth;
double sphereRadius=10.0;
double radius1;


removeTrash();

FILE *pto=NULL;


int q;
int is_cancelled=0;

prepareFreeze();
setAspectRatio((double)(*LastScreenWidth)/(*LastScreenHeight));
setWindynessForCurrentWeather(0);
setDrawingDistance(8000.0);
*CTheScripts__bDisplayHud=0;
*CHud__bScriptDontDisplayRadar=1;
cpedSetVisibility(cped, 0);


// First pass, precache
precachePanoPlace(sx,sy,sz);

// Second pass
*darkness=0;
*darknessEnable=0;
setGameFPSLimit(3);

sprintf(panoName,"pano-%d-%dx%dx%d",(int)time(0),(int)player_pos->x,(int)player_pos->y,(int)player_pos->z);
sprintf(path,"%s\\%s",panoRoot,panoName);
CreateDirectory(path,NULL);

sprintf(path,"%s\\%s\\pano-build.bat",panoRoot,panoName);
pto=fopen(path,"wt");
fprintf(pto,"\"V:\\h\\Hugin-2023.0.0-win64\\nona\" -o pano-%dx%dx%d pano.pto",(int)player_pos->x,(int)player_pos->y,(int)player_pos->z);
fclose(pto);

sprintf(path,"%s\\%s\\pano.pto",panoRoot,panoName);
pto=fopen(path,"wt");
fprintf(pto,"p w4096 h2048 f2 v360 n\"PNG\" R0 T\"UINT8\"\n");
fprintf(pto,"m i6\n");


for(q=0;q<totalFrames;q+=1){
if(GetAsyncKeyState(VK_F7)&1){is_cancelled=1;goto pano_finish;}
xrotth=(360.0*rows*q/totalFrames)/180.0*M_PI;
yrotth=(89.0-178.0*q/totalFrames)/180.0*M_PI;

sprintf(framename,"frame-%.7d.png",q);
fprintf(pto,"i f0 w%d h%d r%f p%f y%f v%f n\"%s\"\n",*LastScreenWidth,*LastScreenHeight,0.0,(float)(89.0-178.0*q/totalFrames),(float)(360.0*rows*q/totalFrames),fov,framename);

radius1=cos(yrotth)*sphereRadius;
tz=sz+sin(yrotth)*sphereRadius;
tx=sx+sin(xrotth)*radius1;
ty=sy+cos(xrotth)*radius1;

refreshFreeze();


// Remove pause (if any), let's game tick again
*codePause=0;
*userPause=0;

setCameraFromToFov(sx,sy,sz,tx,ty,tz,fov);

//sprintf(tmp,"%d/%d",q,totalFrames);
//MessageJumpQ(tmp, 400, 0, false);

// In early versions, I tried to save images via this call. But, unfortuanly, game always crashed after 178-180 saved images.
//char *take_photo=(char*)0x00C8A7C1;
//*take_photo=1;

// Wait for new redraw cycle
waitNFrames(2);
Sleep(50);

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

}



pano_finish:

if(pto){
fclose(pto);
}

restoreFreeze();

// Notify user
if(is_cancelled){
MessageJumpQ("Panorama is cancelled!", 1000, 0, false);
} else {
// Play finish sounds
setVolumeSfx(64);
int noise_count=sizeof(noises)/sizeof(noises[0]);
int noise_id=0;
int notify_count=100;
while(notify_count--){
if(GetAsyncKeyState(VK_F7)&1){break;}
MessageJumpQ("Press F7 to stop notify", 1000, 0, false);
playSoundId(noises[noise_id].id,player_pos);
Sleep(100);
noise_id++;
noise_id%=noise_count;
}

}

// second restore to restore sound volume again
restoreFreeze();

// TODO: exit game at finish (menu option)

}



void createPanoLevels(){

double jumpTo=0.0;
double fov=60.0;
double aspect=(double)(*LastScreenWidth)/(*LastScreenHeight);

double fov_v=fov/aspect;
int levels=ceil(180.0/fov_v);
double rad_per_level=180.0/(double)levels;
void *cped=getPlayerCped();
CVector *player_pos=getPlayerVector();

char framename[32];

double sx=player_pos->x,sy=player_pos->y,sz=player_pos->z+jumpTo;
double tx=0,ty=0,tz=0;

FILE *pto=NULL;


//prepareFreeze();
setAspectRatio(aspect);
setWindynessForCurrentWeather(0);
setDrawingDistance(8000.0);
*CTheScripts__bDisplayHud=0;
*CHud__bScriptDontDisplayRadar=1;
cpedSetVisibility(cped, 0);

*darkness=0;
*darknessEnable=0;
setGameFPSLimit(200);
/*
sprintf(panoName,"pano-%d-%dx%dx%d",(int)time(0),(int)player_pos->x,(int)player_pos->y,(int)player_pos->z);
sprintf(path,"%s\\%s",panoRoot,panoName);
CreateDirectory(path,NULL);

sprintf(path,"%s\\%s\\pano-build.bat",panoRoot,panoName);
pto=fopen(path,"wt");
fprintf(pto,"\"V:\\h\\Hugin-2023.0.0-win64\\nona\" -o pano-%dx%dx%d pano.pto",(int)player_pos->x,(int)player_pos->y,(int)player_pos->z);
fclose(pto);

sprintf(path,"%s\\%s\\pano.pto",panoRoot,panoName);
pto=fopen(path,"wt");
fprintf(pto,"p w4096 h2048 f2 v360 n\"PNG\" R0 T\"UINT8\"\n");
fprintf(pto,"m i6\n");
*/

int l;
for(l=0;l<levels;l++){
if(*menuActive){return;}

double level_center=rad_per_level*l+rad_per_level/2.0-90.0;
int cols_per_level_top=360.0*cos((level_center-rad_per_level/2.0)*M_PI/180.0)/fov;
int cols_per_level_bottom=360.0*cos((level_center+rad_per_level/2.0)*M_PI/180.0)/fov;
int cols_per_level=cols_per_level_top>cols_per_level_bottom?cols_per_level_top:cols_per_level_bottom;

//sprintf(framename,"frame-%.7d.png",q);
//fprintf(pto,"i f0 w%d h%d r%f p%f y%f v%f n\"%s\"\n",*LastScreenWidth,*LastScreenHeight,0.0,(float)(89.0-178.0*q/totalFrames),(float)(360.0*rows*q/totalFrames),fov,framename);
int xx;
double xrotth,yrotth;
double sphereRadius=10.0;
yrotth=level_center*M_PI/180.0;
for(xx=0;xx<cols_per_level;xx++){
xrotth=(double)xx*M_PI*2.0/cols_per_level;
double radius1=cos(yrotth)*sphereRadius;
tz=sz+sin(yrotth)*sphereRadius;
tx=sx+sin(xrotth)*radius1;
ty=sy+cos(xrotth)*radius1;

//refreshFreeze();

setCameraFromToFov(sx,sy,sz,tx,ty,tz,fov);

//sprintf(tmp,"%d/%d",q,totalFrames);
//MessageJumpQ(tmp, 400, 0, false);

// In early versions, I tried to save images via this call. But, unfortuanly, game always crashed after 178-180 saved images.
//char *take_photo=(char*)0x00C8A7C1;
//*take_photo=1;

// Wait for new redraw cycle
waitNFrames(2);
Sleep(50);

}
}

}


void placesShow(){
double sx,sy,sz;
double tx,ty,tz;
double jump;
int duration;
int weatherID;
int q;
double fov1,fov2,speed,tth,ph;
void *cped=getPlayerCped();
void *cped_xyz=*(void**)(cped+0x14); // matrix???

prepareFreeze();

// set volume to low
setVolumeSfx(5);

while(1){
// to new place and time
restoreFreeze();

weatherID=drand()*23.0;
forceWeatherNow(weatherID);
rollTime();


sx=drand()*5600.0-2800.0;
sy=drand()*5600.0-2800.0;
flyTo(sx,sy,1000.0,drand()*360.0,0,1);
fov1=90.0-drand()*drand()*89.0;
fov2=70.0-drand()*drand()*69.0;
if(rand()&1){
tth=fov1;
fov1=fov2;
fov2=tth;
}

// add moving across roads

jump=drand()*3.0;
sz=(*(float*)(cped_xyz+0x38))+jump;
duration=(drand()*15.0*30.0+5.0*30.0)*2.0; // from 5 to 20 seconds
speed=drand()*20.0;
if(rand()&1){
speed*=-1;
}


prepareFreeze();

// rotate cam and ticks clock
for(q=0;q<duration;q++){
if(GetAsyncKeyState(VK_F7)&1){goto finished;}
*clockSeconds++;

tth=speed*q/1000.0/2.0;
ph=(double)q/duration;

tx=sx+sin(tth)*10.0;
ty=sy+cos(tth)*10.0;
tz=sz+cos(tth+123.45)*2.0;

setCameraFromToFov(sx,sy,sz,tx,ty,tz,fov2*ph+(1.0-ph)*fov1);
Sleep(33);
}
}

finished:
MessageJumpQ("Show time is finished", 1000, 0, false);
restoreFreeze();
}





void cityScanner(){
// Летаем над городом по кривой Гильберта и строим карту высот. Часто вылетаем.
// Вообще, это был одноразовый таск, потому с терпением и такой-то матерью я снова
// и снова запускал игрушку, пока не сделал всю карту высот.

uint32_t q,w;
int a,s,t;
int step=200;
int npx,npy;
int mpx,mpy;
float *maptile=malloc(step*step*sizeof(float));
FILE *out;
float lastHeight=300.0;
float curHeight=0;
int level=5; // 32x32
int tiles=hilbert_points_at_level(level);
void *cped=getPlayerCped();

setGravity(0);

for(t=0;t<tiles;t++){
hilbert(t,level,&q,&w);
if(q>=30 || w>=30){continue;}

char filename[256];
char status[256];
sprintf(filename,"v:\\heightmap-%d-%d.bin",q,w);
sprintf(status,"%dx%d (%d/%d)",q,w,t,tiles);
out=fopen(filename,"rb");
if(out){
fclose(out);
continue;
}

setGameFPSLimit(105);
flyTo((int)q*step-3000+step/2,(int)w*step-3000+step/2,111,drand()*360.0,0,1);
cpedSetHealth(cped,99999.99); // health
MessageJumpQ(status, 2000, 0, false);
setVolumeSfx(64);
Sleep(4500);
setGameFPSLimit(3);
Sleep(500);
if(GetAsyncKeyState(VK_F9)&1){goto finish;}

// get data

for(s=0;s<step;s++){
for(a=0;a<step;a++){
npx=q*step+a-3000;
npy=w*step+s-3000;
mpx=a;
mpy=s;
if(npx>=3000 || npy>=3000){continue;}
if(mpx>=step || mpy>=step){continue;}
//curHeight=CWorld__FindGroundZForCoord(npx,npy);
curHeight=findGroundZForCoord(npx,npy);
maptile[mpx+mpy*step]=curHeight;
}
Sleep(1);
}

curHeight+=10.0;
if(curHeight>lastHeight){lastHeight=curHeight;}
if(curHeight<lastHeight){lastHeight-=5;}
if(curHeight>lastHeight){lastHeight=curHeight;}

out=fopen(filename,"wb");
fwrite(maptile,1,step*step*sizeof(float),out);
fclose(out);


}

finish:

MessageJumpQ("Map is done!", 1000, 0, false);
restoreGravity();
setGameFPSLimit(105);

}




void satellite_view(){

unsigned int tile_x,tile_y,tile_id;
unsigned int level=5;
int oldval;
int world_size=powl(2,level);

double wantFov=1.0;
double aspect=((double)*LastScreenWidth)/(*LastScreenHeight);
double satellite_cam_height=tan((90.0-wantFov/2.0)*M_PI/180.0)*6000.0/2.0*aspect;

// remove UI
*CTheScripts__bDisplayHud=0;
*CHud__bScriptDontDisplayRadar=1;
disable_clouds();

// find player position vector;
CVector *cj_pos=getPlayerVector();

// update aspect ratio for wide-screen monitors (if need)
setAspectRatio(aspect);
setCameraFromToFov(0,-0.1,satellite_cam_height,0,0,0,wantFov);

Sleep(5000);
unlock_recip_view_window();

RwCamDummy *scenecam=(RwCamDummy*)*(void**)(0xC17038+4);

double scale=pow(2.0,level);
int tiles_count=hilbert_points_at_level(level);

double scale_x=scale/scenecam->view_window_x;
double scale_y=scale/scenecam->view_window_y;

double tile_size=6000.0/(double)world_size;

scenecam->recip_view_window_x=scale_x;
scenecam->recip_view_window_y=scale_y/aspect;

screenshoter.taken=0;
screenshoter.delay=-1;
screenshoter.active=1;

prepareFreeze();
setGravity(0);

for(tile_id=0;tile_id<tiles_count && !*menuActive;tile_id++){
hilbert(tile_id,level,&tile_x,&tile_y);
sprintf(screenshoter.filename,"tile-%dx%d.jpg",tile_x,tile_y);
if(GetFileAttributes(screenshoter.filename)!=INVALID_FILE_ATTRIBUTES){continue;}

double pos_x=tile_size*tile_x-3000.0+tile_size/2.0;
double pos_y=3000.0-tile_size*tile_y+tile_size/2.0;
double pos_z=findGroundZForCoordByFile(pos_x,pos_y)+1.0;
if(pos_z<0){pos_z=0;}

cj_pos->x=pos_x;
cj_pos->y=pos_y;
cj_pos->z=pos_z;

theCamera->m_fNearClipScript=satellite_cam_height-pos_z-500.0; // 500 not enough for Chilliad mountain, but seens okay
theCamera->m_bUseNearClipScript=1;

// look to sky
setCameraFromToFov(0,0,30,0,0,31,90);waitNFrames(2);

// look to land
setGameFPSLimit(105);
refreshFreeze();

setCameraFromToFov(0,-0.1,satellite_cam_height,0,0,0,wantFov);
scenecam->view_offset_x=(scale-1.0-tile_x*2.0)/aspect/scale_x;
scenecam->view_offset_y=(scale-1.0-(world_size-1.0-tile_y)*2.0)/scale_y;

//logme("Height in sky: %d, pos: %fx%fx%f, clip %f\n",(int)satellite_cam_height,pos_x,pos_y,pos_z,theCamera->m_fNearClipScript);

Sleep(100);

oldval=screenshoter.taken;
screenshoter.delay=7;
while(screenshoter.taken==oldval){ // wait to take screenshot
Sleep(10);
}

}
restoreFreeze();
}



DWORD WINAPI MyASIThread(LPVOID lpParam){
int q;

HWND activeWindow;
while(1){

activeWindow=GetForegroundWindow();
if(*userPause || *codePause || *menuActive || *CTimer_m_FrameCounter<50 || activeWindow!=*(void**)0xC97C1C){ // game not active

//getPlayerCped()
Sleep(100);
continue;
}

if(GetAsyncKeyState(VK_F4)&1){

CVector pos;
pos.x=181.1962;
pos.y=1266.635;
pos.z=22.0121;

playSoundId(1083,&pos);

static int is_pause=1;
is_pause^=1;

// patch game code
*timeStepMin=0.0;
*timeStepNonClippedMin=0.0;
*(float*)(0x00858C14)=0.000000001;

if(is_pause){
*timeScale=0.0;
*timeStep=0.0;

} else {
*timeScale=1.0;
*timeStep=1.0;

}


}

/*
if(GetAsyncKeyState(VK_F5)&1){
flyTo(181.1962, 1266.635, 22.0121, 346.0,0,0);
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
*/

if(GetAsyncKeyState(VK_NUMPAD1)&1){
satellite_view();
}

if(GetAsyncKeyState(VK_NUMPAD5)&1){
MessageJumpQ("add light", 1000, 0, false);
CVector *pos=getPlayerVector();


//CTheScripts__AddScriptSearchLight(pos->x,pos->y,pos->z+2.0,0,pos->x,pos->y,pos->z, 10.0, 0.0);

/*bool CShadows::StoreStaticShadow(unsigned int id, unsigned char type, RwText
ure* texture, CVector* posn, float frontX, float frontY, float sideX, float s
ideY, short intensity, unsigned char red, unsigned char green, unsig
ned char blue, float zDistane, float scale, float drawDistance, bool temporary
Shadow, float upDistance) {
*/
CShadows__StoreStaticShadow(
                0,
                2,
                (void*)0xC403F4,
                pos,
                2.0,
                0.0,
                0.0,
                -2.0,
                128,
                255,
                0,
                0,
                4.0,
                1.0,
                40.0,
                0,
                0.0);

//AddAnExtraDirectionalLight((void*)0xC17038, pos->x,pos->y,pos->z+2.0, 1.0, 0.0, 0.0);


/*
void CCoronas::RegisterCorona(unsigned int nID, CEntity* pAttachTo, unsigned char R,.
unsigned char G, unsigned char B, unsigned char A, const CVector& Position, float Size,
 float Range, RwTexture* pTex, unsigned char flareType, unsigned char reflectionType,
 unsigned char LOSCheck, unsigned char unused, float normalAngle, bool bNeonFade,
 float PullTowardsCam, bool bFadeIntensity, float FadeSpeed, bool bOnlyFromBelow,
 bool bWhiteCore)
*/


CVector *corona_pos=malloc(sizeof(CVector));
corona_pos->x=drand()*10.0-5.0;
corona_pos->y=drand()*10.0-5.0;
corona_pos->z=drand()*10.0;

static int corona_id=5000;

CCoronas__RegisterCorona(
        corona_id++, // uniq id
        getPlayerCped(), // attach to
        255,
        0,
        0,
        0xFF,
        corona_pos,
        5.5,
        9999.88,
        0, // CORONATYPE_SHINYSTAR
        0,
        1,
        0,
        0, // not used
        0.0, // not used
        0,
        .5,
        0,
        150.0,
        0,
        0);

/*
CPointLights__AddLight(
0,
        pos->x,
        pos->y,
        pos->z+drand()*3.0,
        0, // dir
        0,
        0,
        5.0, // radius
        1.0, // rgb
        0,
        0,
        0, // fog
        1,
        0);
*/
}

if(GetAsyncKeyState(VK_NUMPAD6)&1){
MessageJumpQ("add light random", 1000, 0, false);
CVector *pos=getPlayerVector();

CPointLights__AddLight(
0,
        pos->x+drand()*3.0,
        pos->y+drand()*3.0,
        pos->z+drand()*3.0,
        drand(), // dir
        drand(),
        drand(),
        drand()*5.0, // radius
        drand(), // rgb
        drand(),
        drand(),
        0, // fog
        0,
        0);

}


if(GetAsyncKeyState(VK_F6)&1){
rollTime();
}

if(GetAsyncKeyState(VK_F7)&1){
int weatherID=drand()*23.0;
forceWeatherNow(weatherID);
sprintf(tmp,"New weather ID: %d",weatherID);
MessageJumpQ(tmp, 10000, 0, false);
}

if(GetAsyncKeyState(VK_F8)&1){
CVector *cj_pos=getPlayerVector();
do{
cj_pos->x=drand()*6000.0-3000.0;
cj_pos->y=drand()*6000.0-3000.0;
cj_pos->z=findGroundZForCoordByFile(cj_pos->x,cj_pos->y)+1.0;
} while(cj_pos->z<0.0);
continue;

flyTo(places[places_current].x,places[places_current].y,places[places_current].z,places[places_current].heading,places[places_current].interior,0);
places_current++;
places_current%=places_count;


//flyTo(drand()*5000.0-2500.0,drand()*5000.0-2500.0,1000.0,drand()*360.0,0,1);

}


if(GetAsyncKeyState(VK_NUMPAD2)&1){
flyTo(-944.0,2224.0,40.6,90,0,0);
}

if(GetAsyncKeyState(VK_NUMPAD3)&1){
void *water=(void*)0xC22910;
int water_vertex_size=20;
int q;
float newlevel=drand()*50.0+20.0;
for(q=0;q<1200;q++){
*(float*)(water+q*water_vertex_size+4)=newlevel;
}
MessageJumpQ("sea level changed", 1000, 0, false);
}



if(GetAsyncKeyState(VK_NUMPAD9)&1){
CVector *pos=getPlayerVector();
pos->x=drand()*6000.0-3000.0;
pos->y=drand()*6000.0-3000.0;
pos->z=100.0;
static char newposlabel[256];
sprintf(newposlabel,"CJ moved to %.3fx%.3fx%.3f",pos->x,pos->y,pos->z);
MessageJumpQ(newposlabel, 1000, 0, false);
}

if(GetAsyncKeyState(VK_NUMPAD8)&1){

static char newposlabelplcped[256];
sprintf(newposlabelplcped,"Cped: %p",getPlayerCped());
MessageBox(NULL,"CPED",newposlabelplcped,0);

continue;
flyTo(1184.3,-1313.4,19.7,0,0,1);
setCameraFromToFov(1184.3,-1313.4,19.7,1183.6,-1314.0,19.32,50.0);
MessageJumpQ("very long distance", 1000, 0, false);
}


if(GetAsyncKeyState(VK_NUMPAD7)&1){
static int is_player_frozen=0;
is_player_frozen^=1;

memset((void*)0x531155,0x90,5);

CPlayerInfo__MakePlayerSafe(&CWorld__Players[*CWorld__PlayerInFocus],is_player_frozen,10.0);

if(is_player_frozen){
*(uint8_t*)(0x6194A0)=0xC3;// ret
memset((void*)0x541DD7,0x90,5);
DIReleaseMouse();

void *pad=CPad__GetPad(0);
if(pad){
*(int*)(pad+0x10E)=1;
}

} else {
*(uint8_t*)(0x6194A0)=0xE9;// jmp
memcpy((void*)0x541DD7, (char*)"\xE8\xE4\xD5\xFF\xFF", 5);
}


static char msg[256];
sprintf(msg,"now is_player_frozen: %d\n",is_player_frozen);
MessageJumpQ(msg, 1000, 0, false);

continue;

}



if(GetAsyncKeyState(VK_F9)&1){
static int type_projection=0;
type_projection^=1;
setCameraProjection(type_projection+1);
}

if(GetAsyncKeyState(VK_F10)&1){

double magicFoV=120.0;

// remove UI
*CTheScripts__bDisplayHud=0;
*CHud__bScriptDontDisplayRadar=1;
disable_clouds();
No_more_haze();

int level=6;
int world_size=powl(2,level);
double tile_size=6000.0/(double)world_size;
double target_x,target_y,target_z;

screenshoter.taken=0;
screenshoter.delay=-1;
screenshoter.active=1;

setCameraProjection(2);
waitNFrames(2);

setCameraFromToFov(target_x,target_y-.0001,target_z+30.0,target_x,target_y,target_z,magicFoV);
waitNFrames(2);

unlock_recip_view_window();
prepareFreeze();

RwCamDummy *scenecam=(RwCamDummy*)*(void**)(0xC17038+4);

double aspect=((double)*LastScreenWidth)/(*LastScreenHeight);
double view_window_x=scenecam->view_window_x;
double view_window_y=view_window_x/aspect;
double magic=75.0*32.0/world_size;
double scale_x=1.0/view_window_x/magic;
double scale_y=1.0/view_window_y/magic;

scenecam->recip_view_window_x=scale_x;
scenecam->recip_view_window_y=scale_y;


int tiles_count=hilbert_points_at_level(level);
int tile_id;
uint32_t tile_x,tile_y;

for(tile_id=333;tile_id<tiles_count && !*menuActive;tile_id++){
hilbert(tile_id,level,&tile_x,&tile_y);
sprintf(screenshoter.filename,"tile-%dx%d.jpg",tile_x,tile_y);
if(GetFileAttributes(screenshoter.filename)!=INVALID_FILE_ATTRIBUTES){continue;}

target_x=tile_size*(double)tile_x+tile_size/2.0-3000.0;
target_y=3000.0-tile_size*(double)tile_y+tile_size/2.0;
target_z=findGroundZForCoordByFile(target_x,target_y);
if(target_z<0){target_z=0;}

//setGravity(0);
CVector *player=getPlayerVector();
player->x=target_x;
player->y=target_y;
player->z=target_z+1.0;


refreshFreeze();
addTwoStars();
setGameFPSLimit(105);

int q;
double ph,iph;
for(q=0;q<=20;q++){
ph=(double)q/20.0;
iph=1.0-ph;
setCameraFromToFov(target_x,target_y-.01,target_z+30.0*ph,target_x+sin(q)*iph*200.0,target_y+cos(q)*iph*200.0,target_z,magicFoV);
Sleep(100);
}
setCameraFromToFov(target_x,target_y-.01,target_z+30,target_x,target_y,target_z,magicFoV);

int oldval=screenshoter.taken;
screenshoter.delay=7;
while(screenshoter.taken==oldval){ // wait to take screenshot
Sleep(10);
}

}

restoreFreeze();
}

if(GetAsyncKeyState(VK_F11)&1){
//satellite_view();

}


if(GetAsyncKeyState(VK_F12)&1){


createPanoLevels();

MessageJumpQ("levels", 1000, 0, false);

}


Sleep(20);
}

}





/*
void *ccamera=(void*)theCamera; // == 0xB6F028
void *rwcamera=*(void**)(ccamera+0x954); //rwcamera(!)
float *cam_matrix=*(void**)(rwcamera+4);
int *cam_matrix_int=(int*)cam_matrix;
cam_matrix[0]=0;
cam_matrix[1]=0;
cam_matrix[2]=6.626173192E-37;
cam_matrix[3]=6.626173192E-37;
cam_matrix[4]=-1;
cam_matrix[5]=0;
cam_matrix[6]=0;
cam_matrix[7]=4.203895393E-45;
cam_matrix[8]=0;
cam_matrix[9]=1;
cam_matrix[10]=4.882812391E-6;
cam_matrix[11]=1.968934005E-19;
cam_matrix[12]=0;
cam_matrix[13]=4.882812391E-6;
cam_matrix[14]=-1;
cam_matrix[15]=-7.681843486E31;
cam_matrix[19]=2.866497357E32;
cam_matrix[20]=-1;
cam_matrix[21]=0;
cam_matrix[22]=0;
cam_matrix_int[23]=3; // ???
*/

/* //remove ped shadow?
void *cped=getPlayerCped();
int model_id=*(short*)(cped+0x22);
void *models_info=(void*)0xA9B0C8;
void *model=(void*)(models_info+model_id*0x308);
int16_t *model_flags=(int16_t*)(model+0x12);
*model_flags|=1<<5;

*(float*)(model+0x18)=0.01;
*/

