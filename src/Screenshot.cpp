#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include "bmp.h"

#include "Screenshot.h"

void do_screenshot(){

int width=800,height=600;

IDirect3DSurface9* pRenderTarget=NULL;
IDirect3DSurface9* pDestTarget=NULL;

IDirect3DDevice9 *d3ddev = *(IDirect3DDevice9 **)0xC97C28;

d3ddev->CreateOffscreenPlainSurface(width,height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pDestTarget, NULL);

char filename[32];
sprintf(filename,"screenshot-%d_.bmp",(int)time(0));

FILE *t=fopen("debug.txt","w");
fprintf(t,"offscreen: %p\n",pDestTarget);
fflush(t);

//d3ddev->GetFrontBufferData(0, pDestTarget);
fprintf(t,"ret: %d\n",d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pDestTarget));
fflush(t);

D3DXSaveSurfaceToFile(filename, D3DXIFF_BMP, pDestTarget, 0, NULL );
fflush(t);
   pDestTarget->Release();
fclose(t);

return;


//RECT r;
//r.left=0;
//r.top=0;
//r.right=width;
//r.bottom=height;

D3DLOCKED_RECT lockedRect;
fprintf(t,"lock: %d\n",(int)pDestTarget->LockRect(&lockedRect,NULL,D3DLOCK_NO_DIRTY_UPDATE|D3DLOCK_NOSYSLOCK|D3DLOCK_READONLY));
fprintf(t,"pitch: %d\n",lockedRect.Pitch);

fclose(t);

sprintf(filename,"screenshot-%d.bmp",(int)time(0));


FILE *out=fopen(filename,"wb");
bmp_write_rgba(out, (uint32_t *)lockedRect.pBits, width, height);
fclose(out);
    pDestTarget->UnlockRect();

}
