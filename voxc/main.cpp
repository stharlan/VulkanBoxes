
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "voxc.h"

void init_opengl_ext()
{
    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = GetModuleHandle(0);
    wc.lpszClassName = L"DUMMY_WSL_jdmcnghr";
    RegisterClass(&wc);

    HWND dummy = CreateWindowEx(
        0,
        wc.lpszClassName,
        L"DUMMY OPENGL WINDOW",
        0,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0, wc.hInstance, 0);

    HDC ddc = GetDC(dummy);

    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pf = ChoosePixelFormat(ddc, &pfd);

    SetPixelFormat(ddc, pf, &pfd);

    HGLRC hrc = wglCreateContext(ddc);

    wglMakeCurrent(ddc, hrc);

    wglCreateContextAttribsARB = (wglCreateContextAttribsARBFN)wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (wglChoosePixelFormatARBFN)wglGetProcAddress("wglChoosePixelFormatARB");

    //printf("wglCreateContextAttribsARB = %8x\n", wglCreateContextAttribsARB);
    //printf("wglChoosePixelFormatARB = %8x\n", wglChoosePixelFormatARB);

    wglMakeCurrent(ddc, 0);
    wglDeleteContext(hrc);
    ReleaseDC(dummy, ddc);
    DestroyWindow(dummy);

}

int run(HINSTANCE hInstance)
{
    //if (!AllocConsole()) throw new std::runtime_error("allocconsole");
    //FILE* f = nullptr;
    //freopen_s(&f, "CONIN$", "r", stdin);
    //freopen_s(&f, "CONOUT$", "w", stdout);
    //freopen_s(&f, "CONOUT$", "w", stderr);

    init_opengl_ext();

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

    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // Window Extended Style
    DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE; // Windows Style

    RECT wr = { 0,0,1024,768 };

    AdjustWindowRect(&wr, dwStyle, FALSE);

    VOXC_WINDOW_CONTEXT* lpctx = new VOXC_WINDOW_CONTEXT();
    lpctx->screenWidth = wr.right;
    lpctx->screenHeight = wr.bottom;
    block_allocate(lpctx, X_GRID_EXTENT, Y_GRID_EXTENT, Z_GRID_EXTENT);

    HWND hwnd = CreateWindowEx(
        dwExStyle,
        WNDCLASS_VNAME,
        L"GLCS View",
        dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        wr.right,
        wr.bottom,
        NULL,
        NULL,
        hInstance,
        lpctx);

    if (!hwnd) throw new std::runtime_error("failed to create window");

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
        throw new std::runtime_error("failed to register raw input devices");
    }

    RECT cr;
    GetWindowRect(hwnd, &cr);
    ClipCursor(&cr);
    ShowCursor(FALSE);

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

    ShowCursor(TRUE);
    ClipCursor(nullptr);

    block_cleanup(lpctx);
    delete lpctx;

    //int throw_away = getchar();

    //FreeConsole();

    return EXIT_SUCCESS;
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PSTR szCmdLine,
    _In_ int iCmdShow)    
{   
    try {
        run(hInstance);
    }
    catch (const std::exception& e) {
        printf("ERROR: %s\n", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}