#include "core/Patcher.h"
#include "main.h"
#include <Windows.h>

static GH3P::Patcher g_patcher = GH3P::Patcher(__FILE__);

// borderless window stuff from fastgh3

static HWND* hWnd = (HWND*)0x00C5B8F8;

void ForceWindowTopmost()
{
    HWND hwnd = *hWnd;

    int screenX = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int screenY = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    SetWindowPos(
        hwnd,
        HWND_TOPMOST,
        0, 0,
        screenW, screenH,
        SWP_FRAMECHANGED |
        SWP_NOACTIVATE
    );
}

static void* hWndDetour = (void*)0x0057BA6F;

__declspec(naked) void hWndHack()
{
    static const uint32_t returnAddress = 0x0057BA75;

    __asm {
        pushad
        call ForceWindowTopmost
        popad
        jmp returnAddress
    }
}

void BorderlessWindowHack()
{
    // from the old exe mod
    static int* wndStyleAddress = (int*)0x0057BB4C;
    static int* wndStyleValue = (int*)0x0057BB50;
    g_patcher.WriteInt32(wndStyleAddress, 0x00C5B920);
    g_patcher.WriteInt32(wndStyleValue, 1);

    // always on top calls
    g_patcher.WriteInt32((void*)0x0057BA5D, WS_POPUP);
    g_patcher.WriteJmp(hWndDetour, hWndHack);
}

void UncappedFPSHack()
{
    // from wes/the old plugin
    static void* D3DPPpi = (void*)0x0057BB79;
    static int* presint = (int*)0x00C5B934;
    g_patcher.WriteNOPs(D3DPPpi, 6);
    g_patcher.WriteInt32(presint, 0x80000000);
}
