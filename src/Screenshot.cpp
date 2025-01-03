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




SCREENSHOTER screenshoter={0};

int width=800,height=600;

IDirect3DSurface9* pDestTarget=NULL;
IDirect3DDevice9 *d3ddev;
IDirect3DSurface9* renderTarget=NULL;


void do_screenshot(){
screenshoter.delay=5;
screenshoter.active=1;
sprintf(screenshoter.filename,"screenshot-%d.png",(int)time(0));
}

HANDLE g_hChildStd_IN_Rd=NULL;
HANDLE g_hChildStd_IN_Wr=NULL;
HANDLE g_hChildStd_OUT_Rd=NULL;
HANDLE g_hChildStd_OUT_Wr=NULL;

void ffmpeg_spawn(){

char szCmdline[]="ffmpeg -v 0 -f rawvideo -s 800x600 -r 60 -pix_fmt bgra -i - -vcodec hevc_nvenc -b:v 1000k -y ffmpeg-stream.mp4";
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

// Create the child process

ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
 
   ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
   siStartInfo.cb = sizeof(STARTUPINFO); 
   siStartInfo.hStdError = g_hChildStd_OUT_Wr;
   siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
   siStartInfo.hStdInput = g_hChildStd_IN_Rd;
   siStartInfo.dwFlags |= STARTF_USESTDHANDLES|SW_HIDE;
 
// Create the child process. 
    
   bSuccess = CreateProcessA(NULL,
      szCmdline,     // command line
      NULL,          // process security attributes
      NULL,          // primary thread security attributes
      TRUE,          // handles are inherited
      CREATE_NO_WINDOW,             // creation flags
      NULL,          // use parent's environment 
      NULL,          // use parent's current directory 
      &siStartInfo,  // STARTUPINFO pointer 
      &piProcInfo);  // receives PROCESS_INFORMATION 
   
   // If an error occurs, exit the application. 
   if ( ! bSuccess ){
logme("can't spawn ffmpeg");
abort();
}

      CloseHandle(piProcInfo.hProcess);
      CloseHandle(piProcInfo.hThread);
      CloseHandle(g_hChildStd_OUT_Wr);
      CloseHandle(g_hChildStd_IN_Rd);
screenshoter.is_stream=1;
}

void ffmpeg_write(void *pixels, int size){
DWORD dwWritten;

int bSuccess=WriteFile(g_hChildStd_IN_Wr, pixels, size, &dwWritten, NULL);
if(!bSuccess){
logme("can't write to ffmpeg");
abort();
}

if((int)size!=(int)dwWritten){
logme("can't write to ffmpeg: written %d, expect %d",(int)dwWritten,size);
abort();
}
 
logme("ffmpeg written %d",(int)dwWritten);
//CloseHandle(g_hChildStd_IN_Wr);
}


void save_surf_as_png(){
}


void on_frame_screenshot(){

HRESULT hr;
D3DLOCKED_RECT lockedRect;

if(!screenshoter.active){return;}

if(screenshoter.is_stream){
hr=d3ddev->GetRenderTargetData(renderTarget,pDestTarget);
int ret=(int)pDestTarget->LockRect(&lockedRect,NULL,/*D3DLOCK_NO_DIRTY_UPDATE|D3DLOCK_NOSYSLOCK|*/D3DLOCK_READONLY);
ffmpeg_write(lockedRect.pBits,800*600*4);
pDestTarget->UnlockRect();
return;

}

if(screenshoter.delay>=0){
screenshoter.delay--;
}
if(screenshoter.delay!=0){return;}

d3ddev=*(IDirect3DDevice9 **)0xC97C28;

//d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pDestTarget);

// alternative
if(!renderTarget){
hr=d3ddev->GetRenderTarget(0, &renderTarget);
}
if(FAILED(hr)||!renderTarget){
logme("Can't get render target");
exit(0);abort();}




if(!pDestTarget){
width=*LastScreenWidth;
height=*LastScreenWidth;

D3DSURFACE_DESC desc;
hr = renderTarget->GetDesc(&desc);
if(FAILED(hr)){
logme("Can't get descr");
exit(0);abort();
}
logme("We created offscreen buffer %dx%d (%d/%d)",desc.Width, desc.Height, desc.Format,D3DFMT_A8R8G8B8);
d3ddev->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format /*D3DFMT_A8R8G8B8*/, D3DPOOL_SYSTEMMEM, &pDestTarget, NULL);
}


hr=d3ddev->GetRenderTargetData(renderTarget,pDestTarget);
if(FAILED(hr)){
logme("Can't get render target data");
exit(0);
}


strcpy(screenshoter.writing_filename,screenshoter.filename);

if(strstr(screenshoter.writing_filename,".bmp")){
D3DXSaveSurfaceToFile(screenshoter.filename, D3DXIFF_BMP, pDestTarget, 0, NULL );
}

if(strstr(screenshoter.writing_filename,".png")){
ffmpeg_spawn();

int ret=(int)pDestTarget->LockRect(&lockedRect,NULL,/*D3DLOCK_NO_DIRTY_UPDATE|D3DLOCK_NOSYSLOCK|*/D3DLOCK_READONLY);
ffmpeg_write(lockedRect.pBits,800*600*4);
pDestTarget->UnlockRect();


//D3DXSaveSurfaceToFile(screenshoter.filename, D3DXIFF_PNG, pDestTarget, 0, NULL );
}

if(strstr(screenshoter.writing_filename,".jpg")){
D3DXSaveSurfaceToFile(screenshoter.filename, D3DXIFF_JPG, pDestTarget, 0, NULL );
}

screenshoter.taken++;

return;
}
