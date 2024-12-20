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
// Летаем над городом по кривой Гильберта и строим карту высот. Часто вылетаем. Вообще, это был одноразовый таск, потому с терпением и такой-то матерью я снова и снова запускал игрушку, пока не сделал всю карту высот.

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
unsigned int level=7;
int oldval;
int world_size=powl(2,level);

double wantFov=90.0;
double aspect=((double)*LastScreenWidth)/(*LastScreenHeight);
double satellite_cam_height=tan((90.0-wantFov/2.0)*M_PI/180.0)*6000.0/2.0*aspect;

// remove UI
*CTheScripts__bDisplayHud=0;
*CHud__bScriptDontDisplayRadar=1;

// remove clouds
*(uint8_t*)0x713950=0xc3; // disable clouds
*(uint8_t*)0x716380=0xc3; // disable volumetric clouds
*(uint8_t*)0x716C90=0xc3; // disable moving fog
*(uint8_t*)0x7154B0=0xc3; // disable bottom from height
memset((void*)0x53E121,0x90,5);

// don't scale image to full window
//memset((void*)0x7EE432,90,15);
//*(uint8_t*)0x7EE410=0xc3;
*(uint8_t*)0x7EE432=0x90;
*(uint8_t*)0x7EE433=0x90;
*(uint8_t*)0x7EE434=0x90;
*(uint8_t*)0x7EE43E=0x90;
*(uint8_t*)0x7EE43F=0x90;
*(uint8_t*)0x7EE440=0x90;

setAspectRatio(aspect);
setCameraFromToFov(0,-1,satellite_cam_height,0,0,0,wantFov);
Sleep(5000);

RwCamDummy *scenecam=(RwCamDummy*)*(void**)(0xC17038+4);

double scale=pow(2.0,level);
int tiles_count=hilbert_points_at_level(level);

double scale_x=scale/scenecam->view_window_x;
double scale_y=scale/scenecam->view_window_y;

scenecam->recip_view_window_x=scale_x;
scenecam->recip_view_window_y=scale_y;

screenshoter.taken=0;
screenshoter.delay=-1;
screenshoter.active=1;

//prepareFreeze();

for(tile_id=0;tile_id<tiles_count && !*menuActive;tile_id++){
setGameFPSLimit(105);
//refreshFreeze();
hilbert(tile_id,level,&tile_x,&tile_y);
scenecam->view_offset_x=(scale-1.0-tile_x*2.0)/aspect/scale_x;
scenecam->view_offset_y=(scale-1.0-(world_size-1.0-tile_y)*2.0)/scale_y;

sprintf(screenshoter.filename,"tile-%dx%d.jpg",tile_x,tile_y);
oldval=screenshoter.taken;
screenshoter.delay=70;
while(screenshoter.taken==oldval){ // wait to take screenshot
Sleep(100);
}

}
//restoreFreeze();
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

if(GetAsyncKeyState(VK_NUMPAD1)&1){
//cityScanner();
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
//createPano();
}

if(GetAsyncKeyState(VK_F8)&1){
flyTo(places[places_current].x,places[places_current].y,places[places_current].z,places[places_current].heading,places[places_current].interior,0);
places_current++;
places_current%=places_count;
}

if(GetAsyncKeyState(VK_F9)&1){
flyTo(drand()*5000.0-2500.0,drand()*5000.0-2500.0,1000.0,drand()*360.0,0,1);
}

if(GetAsyncKeyState(VK_F10)&1){
int weatherID=drand()*23.0;
forceWeatherNow(weatherID);
//sprintf(tmp,"New weather ID: %d",weatherID);
//MessageJumpQ(tmp, 10000, 0, false);

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

// add green color to skybox
static uint8_t green_color_to_skybox1[]={0xB9, 0x00, 0xFF, 0x00, 0xFF, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
static uint8_t green_color_to_skybox2[]={0xB9, 0x00, 0xFF, 0x00, 0xFF, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};

memcpy((void*)0x713060,green_color_to_skybox1,sizeof(green_color_to_skybox1));
memcpy((void*)0x7130FF,green_color_to_skybox2,sizeof(green_color_to_skybox2));

// remove clouds from bottom
memset((void*)0x53E121,0x90,5);

continue;

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

continue;

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


if(GetAsyncKeyState(VK_NUMPAD4)&1){
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



if(GetAsyncKeyState(VK_F11)&1){

satellite_view();
//do_screenshot();

continue;


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






}


if(GetAsyncKeyState(VK_F12)&1){
placesShow();
MessageJumpQ("Show must go on!", 1000, 0, false);
}


Sleep(20);
}

}





