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

#include "bmp.h"
#include "ApiGtaSA.h"

#include "Screenshot.h"

SCREENSHOTER screenshoter={0};

int width=800,height=600;

IDirect3DSurface9* pDestTarget=NULL;
IDirect3DDevice9 *d3ddev;

void do_screenshot(){
screenshoter.delay=5;
screenshoter.active=1;
}


// borrowed from stackoverflow

void SavePixelsToFile32bppPBGRAasPNG(UINT width, UINT height, UINT stride, LPBYTE pixels, char* filePath, const GUID &format){
static IWICImagingFactory *factory = nullptr;
static IWICBitmapEncoder *encoder = nullptr;
static IWICBitmapFrameEncode *frame = nullptr;
static IWICStream *stream = nullptr;
static GUID pf = GUID_WICPixelFormat32bppPBGRA;

// init
if(!factory){
CoInitialize(nullptr);
CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
factory->CreateStream(&stream);
factory->CreateEncoder(format, nullptr, &encoder);
encoder->Initialize(stream, WICBitmapEncoderNoCache);
encoder->CreateNewFrame(&frame, nullptr); // we don't use options here
frame->Initialize(nullptr); // we dont' use any options here
frame->SetSize(width, height);
frame->SetPixelFormat(&pf);
}


wchar_t fname[2000];
mbstowcs(fname, filePath, strlen(filePath)+1);//Plus null

stream->InitializeFromFilename(fname, GENERIC_WRITE);
frame->WritePixels(height, stride, stride * height, pixels);
frame->Commit();
encoder->Commit();
}


void save_surf_as_png(){
D3DLOCKED_RECT lockedRect;
int ret=(int)pDestTarget->LockRect(&lockedRect,NULL,D3DLOCK_NO_DIRTY_UPDATE|D3DLOCK_NOSYSLOCK|D3DLOCK_READONLY);
SavePixelsToFile32bppPBGRAasPNG(width, height, lockedRect.Pitch, lockedRect.pBits, screenshoter.filename, GUID_ContainerFormatPng);
pDestTarget->UnlockRect();
}


void on_frame_screenshot(){

if(!screenshoter.active){return;}
if(screenshoter.delay>=0){
screenshoter.delay--;
}
if(screenshoter.delay!=0){return;}

d3ddev=*(IDirect3DDevice9 **)0xC97C28;
if(!pDestTarget){
width=*LastScreenWidth;
height=*LastScreenWidth;
d3ddev->CreateOffscreenPlainSurface(width,height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pDestTarget, NULL);
}

d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pDestTarget);
strcpy(screenshoter.writing_filename,screenshoter.filename);

if(strstr(screenshoter.writing_filename,".bmp")){
D3DXSaveSurfaceToFile(screenshoter.filename, D3DXIFF_BMP, pDestTarget, 0, NULL );
}

if(strstr(screenshoter.writing_filename,".png")){
//save_surf_as_png();
D3DXSaveSurfaceToFile(screenshoter.filename, D3DXIFF_PNG, pDestTarget, 0, NULL );
}

if(strstr(screenshoter.writing_filename,".jpg")){
D3DXSaveSurfaceToFile(screenshoter.filename, D3DXIFF_JPG, pDestTarget, 0, NULL );
}

screenshoter.taken++;

return;
}
