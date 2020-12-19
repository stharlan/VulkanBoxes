
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "voxc.h"

void init_opengl_ext(HINSTANCE hInstance)
{
    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = GetModuleHandle(0);
    wc.lpszClassName = L"DUMMY_WSL_jdmcnghr";
    if (!RegisterClass(&wc))
        throw new std::runtime_error("failed to register window class");

    HWND dummy = CreateWindowEx(
        0,
        wc.lpszClassName,
        L"DUMMY OPENGL WINDOW",
        0,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0, wc.hInstance, 0);
    if(!dummy) throw new std::runtime_error("failed to create dummy window");

    HDC ddc = GetDC(dummy);
    if (!ddc) throw new std::runtime_error("failed to get dummy dc");

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
    if (!pf) throw new std::runtime_error("failed to get choose pixel format for dummy dc");

    if (FALSE == SetPixelFormat(ddc, pf, &pfd))
        throw new std::runtime_error("failed to set pixel format for dummy dc");

    HGLRC hrc = wglCreateContext(ddc);
    if (!hrc) throw new std::runtime_error("failed to create temp rc");

    if (FALSE == wglMakeCurrent(ddc, hrc))
        throw new std::runtime_error("failed to make dummy rc current");

    wglCreateContextAttribsARB = (wglCreateContextAttribsARBFN)wglGetProcAddress("wglCreateContextAttribsARB");
    if (!wglCreateContextAttribsARB) throw new std::runtime_error("failed to get proc address for wglCreateContextAttribsARB");

    wglChoosePixelFormatARB = (wglChoosePixelFormatARBFN)wglGetProcAddress("wglChoosePixelFormatARB");
    if (!wglChoosePixelFormatARB) throw new std::runtime_error("failed to get proc address for wglChoosePixelFormatARB");

    if (FALSE == wglMakeCurrent(ddc, 0))
        throw new std::runtime_error("failed to release dummy rc");
    if (FALSE == wglDeleteContext(hrc))
        throw new std::runtime_error("failed to delete dummy rc");
    if (!ReleaseDC(dummy, ddc))
        throw new std::runtime_error("failed to release dummy dc");
    if (!DestroyWindow(dummy))
        throw new std::runtime_error("failed to destroy dummy window");
    if (!UnregisterClass(L"DUMMY_WSL_jdmcnghr", hInstance))
        throw new std::runtime_error("failed to unregister dummy window class");

}

void setup(HINSTANCE hInstance, VOXC_WINDOW_CONTEXT* lpctx)
{
#ifdef _DEBUG
    if (!AllocConsole()) throw new std::runtime_error("allocconsole");
    FILE* f = nullptr;
    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
#endif

    init_opengl_ext(hInstance);

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
    if (!RegisterClass(&wc))
        throw new std::runtime_error("failed to register main window class");

    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // Window Extended Style
    //DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE; // Windows Style
    DWORD dwStyle = WS_OVERLAPPEDWINDOW;

    RECT wr = { 0,0,1024,768 };

    if (!AdjustWindowRect(&wr, dwStyle, FALSE))
        throw new std::runtime_error("failed to adjust main window rect");

    lpctx->screenWidth = wr.right;
    lpctx->screenHeight = wr.bottom;
    //block_allocate(lpctx, X_GRID_EXTENT, Y_GRID_EXTENT, Z_GRID_EXTENT);

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

    SetWindowLong(hwnd, GWL_STYLE, 0);
    ShowWindow(hwnd, SW_MAXIMIZE);

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
    if (!GetWindowRect(hwnd, &cr))
        throw new std::runtime_error("failed to get main window rect");
    if (!ClipCursor(&cr))
        throw new std::runtime_error("failed to clip cursor");
    ShowCursor(FALSE);
}

void begin_message_loop()
{
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

}

void cleanup(VOXC_WINDOW_CONTEXT* lpctx)
{
    ShowCursor(TRUE);
    if (!ClipCursor(nullptr))
        throw new std::runtime_error("failed to release clipped cursor");

    //block_cleanup(lpctx);

#ifdef _DEBUG
    int throw_away = getchar();
    FreeConsole();
#endif
}

void testSqlite()
{
    sqlite3* pdb = nullptr;
    sqlite3_stmt* pstmt = nullptr;
    const char* pzTail;
    int result = sqlite3_open("block.sdb", &pdb);
    //result = sqlite3_prepare_v2(pdb, "create table ttest (tval INTEGER)", 34, &pstmt, &pzTail);
    //result = sqlite3_prepare_v2(pdb, "insert into ttest values (9801)", 32, &pstmt, &pzTail);
    result = sqlite3_prepare_v2(pdb, "select * from ttest", 32, &pstmt, &pzTail);
    while (SQLITE_ROW == sqlite3_step(pstmt))
    {
        result = sqlite3_column_int(pstmt, 0);
    }
    sqlite3_finalize(pstmt);
    sqlite3_close(pdb);    
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PSTR szCmdLine,
    _In_ int iCmdShow)    
{   
    VOXC_WINDOW_CONTEXT* lpctx = new VOXC_WINDOW_CONTEXT();

    try {
        setup(hInstance, lpctx);
        begin_message_loop();
        cleanup(lpctx);
    }
    catch (const std::exception& e) {
        printf("ERROR: %s\n", e.what());
        delete lpctx;
        return EXIT_FAILURE;
    }

    delete lpctx;

    return EXIT_SUCCESS;
}