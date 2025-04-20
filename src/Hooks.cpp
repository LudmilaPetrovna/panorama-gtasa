#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <time.h>

#include <memory>
#include <kthook/kthook.hpp>

#include "log.h"
#include "Screenshot.h"
#include "CamRecord.h"
#include "Hooks.h"

using CTimer__UpdateSignature = void(__cdecl*)();
using EndSceneSignature = HRESULT(__stdcall*)(IDirect3DDevice9*);
using ResetSignature = HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
using WndProcSignature = HRESULT(__stdcall*)(HWND, UINT, WPARAM, LPARAM);
kthook::kthook_simple<CTimer__UpdateSignature> CTimerHook{};
kthook::kthook_signal<EndSceneSignature> EndSceneHook{};
kthook::kthook_signal<ResetSignature> ResetHook{};
kthook::kthook_simple<WndProcSignature> WndProcHook{};

std::optional<HRESULT> D3D9EndScene(const decltype(EndSceneHook)& hook, IDirect3DDevice9* pDevice) {
on_frame_screenshot();
on_frame_camrecord();

    return std::nullopt;
}

std::optional<HRESULT> D3D9Lost(const decltype(ResetHook)& hook, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentParams) {
    return std::nullopt;
}


void D3D9Reset(const decltype(ResetHook)& hook, HRESULT& return_value, IDirect3DDevice9* device_ptr, D3DPRESENT_PARAMETERS* parameters) {
}

void setD3D9Hooks() {
    DWORD pDevice = *reinterpret_cast<DWORD*>(0xC97C28);

    void** vTable = *reinterpret_cast<void***>(pDevice);

    EndSceneHook.set_dest(vTable[42]);
    EndSceneHook.before.connect(&D3D9EndScene);
    EndSceneHook.install();

    ResetHook.set_dest(vTable[16]);
    ResetHook.before.connect(&D3D9Lost);
    ResetHook.after.connect(&D3D9Reset);
    ResetHook.install();
logme("hooks install4");

}

void CTimer__Update(const decltype(CTimerHook)& hook) {
    static bool init{};
    if (!init) {
        setD3D9Hooks();
        init = { true };
    }

    hook.get_trampoline()();
}

void hooks_install(){
        CTimerHook.set_dest(0x561B10);
        CTimerHook.set_cb(&CTimer__Update);
        CTimerHook.install();
}
