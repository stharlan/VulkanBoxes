
#include "voxc.h"

void OnResize(LPARAM lParam, VOXC_WINDOW_CONTEXT* lpctx)
{
    //LOWORD(lParam) w
    //HIWORD(lParam) h
    int w = LOWORD(lParam);
    int h = HIWORD(lParam);
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    lpctx->viewportRatio = (float)w / (float)h;
}

LRESULT CALLBACK WndProc(
    _In_ HWND   hwnd,
    _In_ UINT   uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    UINT dwSize = 0;
    VOXC_WINDOW_CONTEXT* lpctx = (VOXC_WINDOW_CONTEXT*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    RAWINPUT* raw = NULL;
    CREATESTRUCT* lpCS = NULL;
    switch (uMsg)
    {
    case WM_INPUT:
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &lpctx->rawBuffer, &dwSize, sizeof(RAWINPUTHEADER));
        raw = (RAWINPUT*)lpctx->rawBuffer;
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
                    DestroyWindow(hwnd);
                    break;
                default:
                    printf("kb %i\n", raw->data.keyboard.MakeCode);
                    break;
                }
            }
            else if(raw->data.keyboard.Flags & 0x1) {
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
        return 0;
    case WM_CREATE:
        lpCS = (CREATESTRUCT*)lParam;
        lpctx = (VOXC_WINDOW_CONTEXT*)lpCS->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)lpctx);
        lpctx->hQuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        lpctx->hRenderThread = CreateThread(NULL, 0, RenderThread, (LPVOID)hwnd, 0, NULL);
        return 0;
    case WM_DESTROY:
        SetEvent(lpctx->hQuitEvent);
        WaitForSingleObject(lpctx->hRenderThread, INFINITE);
        CloseHandle(lpctx->hQuitEvent);
        CloseHandle(lpctx->hRenderThread);
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        OnResize(lParam, lpctx);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
