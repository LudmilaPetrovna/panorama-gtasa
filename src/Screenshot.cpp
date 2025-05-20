#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <initguid.h>
#include <wincodec.h>

#define BUFSIZE 4096


#include "bmp.h"
#include "ApiGtaSA.h"

#include "Screenshot.h"
#include "log.h"


void screenshoter_init();

SCREENSHOTER screenshoter={0};

int width=-1,height=-1;

IDirect3DSurface9* pDestTarget=NULL;
IDirect3DDevice9 *d3ddev;
IDirect3DSurface9* renderTarget=NULL;
D3DLOCKED_RECT lockedRect;
HANDLE g_hChildStd_IN_Rd=NULL;
HANDLE g_hChildStd_IN_Wr=NULL;
HANDLE g_hChildStd_OUT_Rd=NULL;
HANDLE g_hChildStd_OUT_Wr=NULL;

void ffmpeg_spawn(){
static char exec[1024];
sprintf(exec,"ffmpeg -v 0 -f rawvideo -s %dx%d -r 60 -pix_fmt bgra -i - -vcodec hevc_nvenc -b:v 3000k -y \"%s\"",width,height,screenshoter.filename);
PROCESS_INFORMATION piProcInfo; 
STARTUPINFO siStartInfo;
BOOL bSuccess=FALSE;

SECURITY_ATTRIBUTES saAttr; 
saAttr.nLength=sizeof(SECURITY_ATTRIBUTES); 
saAttr.bInheritHandle=TRUE; 
saAttr.lpSecurityDescriptor=NULL; 

if(!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0)){abort();}
if(!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)){abort();}
if(!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)){abort();}
if(!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)){abort();}

ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
ZeroMemory(&siStartInfo, sizeof(STARTUPINFO) );
siStartInfo.cb = sizeof(STARTUPINFO); 
siStartInfo.hStdError=g_hChildStd_OUT_Wr;
siStartInfo.hStdOutput=g_hChildStd_OUT_Wr;
siStartInfo.hStdInput=g_hChildStd_IN_Rd;
siStartInfo.dwFlags|=STARTF_USESTDHANDLES|SW_HIDE;
 
screenshoter.ffmpeg=CreateProcessA(NULL,
      exec,     // command line
      NULL,          // process security attributes
      NULL,          // primary thread security attributes
      TRUE,          // handles are inherited
      CREATE_NO_WINDOW,             // creation flags
      NULL,          // use parent's environment 
      NULL,          // use parent's current directory 
      &siStartInfo,  // STARTUPINFO pointer 
      &piProcInfo);  // receives PROCESS_INFORMATION 

if(!screenshoter.ffmpeg){
logme("can't spawn ffmpeg");
abort();
}

logme("we spawned ffmpeg");

CloseHandle(piProcInfo.hProcess);
CloseHandle(piProcInfo.hThread);
CloseHandle(g_hChildStd_OUT_Wr);
CloseHandle(g_hChildStd_IN_Rd);
}

void ffmpeg_write(void *pixels, int size){
int written;
if(!screenshoter.ffmpeg){ffmpeg_spawn();}
if(!WriteFile(g_hChildStd_IN_Wr,pixels,size,&written,NULL)){
logme("can't write to ffmpeg");
abort();
}

if(size!=written){
logme("can't write to ffmpeg: written %d, expect %d",written,size);
abort();
}

logme("ffmpeg written %d",written);

if(!screenshoter.is_stream){
screenshot_stop();
}
}


void ffmpeg_push_frame(){
d3ddev->GetRenderTargetData(renderTarget,pDestTarget);
pDestTarget->LockRect(&lockedRect,NULL,D3DLOCK_READONLY);
ffmpeg_write(lockedRect.pBits,width*height*4);
pDestTarget->UnlockRect();
}

void on_frame_screenshot(){
if(!screenshoter.active){return;}

if(!screenshoter.inited){
screenshoter_init();
}

/*
if(screenshoter.is_stream){
ffmpeg_push_frame();
return;
}
*/

if(screenshoter.delay>0){
screenshoter.delay--;
return;
}
if(screenshoter.delay!=0){return;}
screenshoter.delay--;

logme("targets: %p / %p",renderTarget,pDestTarget);

// take data
//d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pDestTarget);
HRESULT hr=d3ddev->GetRenderTargetData(renderTarget,pDestTarget);
if(FAILED(hr)){
logme("Can't get render target data");
exit(0);
}

logme("got capture");

if(strstr(screenshoter.filename,".mp4")){
ffmpeg_push_frame();
}

if(strstr(screenshoter.filename,".bmp")){
D3DXSaveSurfaceToFile(screenshoter.filename, D3DXIFF_BMP, pDestTarget, 0, NULL );
}

if(strstr(screenshoter.filename,".png")){
logme("is png1");
D3DXSaveSurfaceToFile(screenshoter.filename, D3DXIFF_PNG, pDestTarget, 0, NULL );
logme("is png2");
}

if(strstr(screenshoter.filename,".jpg")){
D3DXSaveSurfaceToFile(screenshoter.filename, D3DXIFF_JPG, pDestTarget, 0, NULL );
}

screenshoter.taken++;
logme("is done");
return;
}

void screenshoter_init(){
d3ddev=*(IDirect3DDevice9 **)0xC97C28;
width=*LastScreenWidth;
height=*LastScreenHeight;
d3ddev->GetRenderTarget(0, &renderTarget);

D3DSURFACE_DESC desc;
renderTarget->GetDesc(&desc);

if(desc.Width!=width || desc.Height!=height || desc.Format!=D3DFMT_A8R8G8B8){
logme("Can't init screenshoter: we expect %dx%d (%d), but got %dx%d (%d)",width, height, (int)D3DFMT_A8R8G8B8, desc.Width, desc.Height, desc.Format);
abort();
}
d3ddev->CreateOffscreenPlainSurface(width, height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pDestTarget, NULL);

screenshoter.taken=0;
screenshoter.inited=1;
}

void screenshot_start(char *filename, int delay, int is_stream){
if(filename){
strcpy(screenshoter.filename,filename);
}
screenshoter.delay=delay;
screenshoter.is_stream=is_stream;
}

void screenshot_stop(){
logme("we closing ffmpeg");
screenshoter.is_stream=0;
CloseHandle(g_hChildStd_IN_Wr);
CloseHandle(g_hChildStd_OUT_Rd);
logme("we closed ffmpeg");

}

