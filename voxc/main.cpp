  
#define STB_IMAGE_IMPLEMENTATION
#include "voxc.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
)
{

    ShowCursor(FALSE);
    AllocConsole();
    FILE* f = nullptr;
    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);

    const LPCWSTR WNDCLASS_VNAME = L"VOXC";
    WNDCLASS wc = {};

    // create window class
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = NULL;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = WNDCLASS_VNAME;
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(WNDCLASS_VNAME, L"GLCS View", 
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
        NULL, NULL, GetModuleHandle(NULL), 0);
    if (!hwnd) {
        printf("Failed to create visible window\n");
        return 0;
    }

    RAWINPUTDEVICE Rid[2];

    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x02;
    Rid[0].dwFlags = RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
    Rid[0].hwndTarget = hwnd;

    Rid[1].usUsagePage = 0x01;
    Rid[1].usUsage = 0x06;
    Rid[1].dwFlags = RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
    Rid[1].hwndTarget = hwnd;

    if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) {
        //registration failed. Call GetLastError for the cause of the error
    }

    // start two message loops for two different windows
    {
        BOOL bRet;
        MSG msg;
        while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
        {
            if (bRet == -1)
            {
                // handle the error and possibly exit
                break;
            }
            else if (bRet == 0) {
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    FreeConsole();
    ShowCursor(TRUE);

    return 0;
}