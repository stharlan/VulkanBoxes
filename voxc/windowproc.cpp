
#include "voxc.h"

void handle_resize(HWND hwnd, LPARAM lParam)
{
    VOXC_WINDOW_CONTEXT* lpctx = (VOXC_WINDOW_CONTEXT*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (!lpctx) throw new std::runtime_error("failed to get window lng ptr");
    int w = LOWORD(lParam);
    int h = HIWORD(lParam);
    if (h == 0) h = 1;
    lpctx->screenWidth = w;
    lpctx->screenHeight = h;
    lpctx->viewportRatio = (float)w / (float)h;
}

void handle_input(HWND hwnd, LPARAM lParam)
{
    VOXC_WINDOW_CONTEXT* lpctx = (VOXC_WINDOW_CONTEXT*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (!lpctx) throw new std::runtime_error("failed to get window lng ptr");

    RAWINPUT* raw = (RAWINPUT*)lpctx->rawBuffer;
    UINT dwSize = 0;
    
    UINT result = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
    if (result == -1) throw new std::runtime_error("failed to get raw input data size");
    
    result = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &lpctx->rawBuffer, &dwSize, sizeof(RAWINPUTHEADER));
    if (result == -1) throw new std::runtime_error("failed to get raw input data size");

    if (raw->header.dwType == RIM_TYPEKEYBOARD)
    {
        if (raw->data.keyboard.Flags == 0) {
            switch (raw->data.keyboard.MakeCode)
            {
            case 17: // w
                lpctx->keys[0] = 1;
                break;
            case 30: // a
                lpctx->keys[1] = 1;
                break;
            case 31: // s
                lpctx->keys[2] = 1;
                break;
            case 32: // d
                lpctx->keys[3] = 1;
                break;
            case 57:
                lpctx->keys[4] = 1;
                break;
            case 16: // q
                //lpctx->ez += 1.0f;
                break;
            case 44: // z
                //lpctx->ez -= 1.0f;
                break;
            case 42: // shift
                lpctx->keys[5] = 1;
                break;
            case 1:
                if (!DestroyWindow(hwnd))
                    throw new std::runtime_error("failed to destroy window");
                break;
            default:
                printf("kb %i\n", raw->data.keyboard.MakeCode);
                break;
            }
        }
        else if (raw->data.keyboard.Flags & 0x1) {
            switch (raw->data.keyboard.MakeCode)
            {
            case 17: // w
                lpctx->keys[0] = 0;
                break;
            case 30: // a
                lpctx->keys[1] = 0;
                break;
            case 31: // s
                lpctx->keys[2] = 0;
                break;
            case 32: // d
                lpctx->keys[3] = 0;
                break;
            case 57:
                lpctx->keys[4] = 0;
                break;
            case 42: // shift
                lpctx->keys[5] = 0;
                break;
            }
        }
    }
    else if (raw->header.dwType == RIM_TYPEMOUSE)
    {
        lpctx->elevation -= (raw->data.mouse.lLastY / 20.0f);
        lpctx->elevation = FCLAMP(lpctx->elevation, -90.0f, 90.0f);
        lpctx->azimuth -= (raw->data.mouse.lLastX / 20.0f);
        if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) lpctx->keys[6] = 1;
        if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) lpctx->keys[6] = 0;
        if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) lpctx->keys[7] = 1;
        if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) lpctx->keys[7] = 0;
    }

}

void handle_create(HWND hwnd, LPARAM lParam)
{
    CREATESTRUCT* lpCS = (CREATESTRUCT*)lParam;
    VOXC_WINDOW_CONTEXT* lpctx = (VOXC_WINDOW_CONTEXT*)lpCS->lpCreateParams;

    // This will return zero the first time it is called.
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)lpctx);

    lpctx->hQuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (lpctx->hQuitEvent == nullptr)
        throw new std::runtime_error("failed to create quit event");

    lpctx->hRenderThread = CreateThread(NULL, 0, RenderThread, (LPVOID)hwnd, 0, NULL);
    if (lpctx->hRenderThread == nullptr)
        throw new std::runtime_error("failed to create render thread");

}

void handle_destroy(HWND hwnd)
{
    VOXC_WINDOW_CONTEXT* lpctx = (VOXC_WINDOW_CONTEXT*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (!lpctx) throw new std::runtime_error("failed to get window lng ptr");

    if (!SetEvent(lpctx->hQuitEvent))
        throw new std::runtime_error("failed to set quit event");

    if (WAIT_FAILED == WaitForSingleObject(lpctx->hRenderThread, INFINITE))
        throw new std::runtime_error("failed to wait for render thread to quit");

    if (!CloseHandle(lpctx->hQuitEvent)) throw new std::runtime_error("failed to close quit event");
    if (!CloseHandle(lpctx->hRenderThread)) throw new std::runtime_error("failed to close render thread handle");
    PostQuitMessage(0);
}

LRESULT CALLBACK WndProc(
    _In_ HWND   hwnd,
    _In_ UINT   uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    switch (uMsg)
    {
    case WM_INPUT:
        handle_input(hwnd, lParam);
        break;
    case WM_SIZE:
        handle_resize(hwnd, lParam);
        break;
    case WM_CREATE:
        handle_create(hwnd, lParam);
        break;
    case WM_DESTROY:
        handle_destroy(hwnd);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
