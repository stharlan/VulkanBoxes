  
#define STB_IMAGE_IMPLEMENTATION
#include "voxc.h"

HANDLE hQuitEvent = 0;
HANDLE hRenderThread = 0;

BYTE* rawBuffer[48];

float ex = 0.0f;
float ey = 10.0f;
float ez = 0.0f;
float elevation = 0.0f;
float azimuth = 270.0f;

GLuint topVertexIndices[6] = { 0, 1, 2, 2, 3, 0 };
GLuint plusxVertexIndices[6] = { 4, 5, 6, 6, 7, 4 };
GLuint minusxVertexIndices[6] = { 8, 9, 10, 10, 11, 8 };
GLuint plusyVertexIndices[6] = { 12, 13, 14, 14, 15, 12 };
GLuint minusyVertexIndices[6] = { 16, 17, 18, 18, 19, 16 };
GLuint bottomVertexIndices[6] = { 20, 21, 22, 22, 23, 20 };
 
typedef struct _VERTEX
{
    glm::vec3 vertex;
    glm::vec2 texc;
} VERTEX;

VERTEX vertices[36];
        
// cube vertices
const glm::vec4 locs[40] = {
    // +z
    { 0, 0, 1,1},
    {1, 0, 1,1},
    {1, 1, 1,1},
    {0, 1, 1,1},

    // +x
    {1, 0, 0,1},
    {1, 1, 0,1},
    {1, 1, 1,1},
    {1, 0, 1,1},

    // -x
    {0, 0, 0,1},
    {0, 0, 1,1},
    {0, 1, 1,1},
    {0, 1, 0,1},

    // +y
    {0, 1, 0,1},
    {0, 1, 1,1},
    {1, 1, 1,1},
    {1, 1, 0,1},

    // -y
    {0, 0, 0,1},
    {1, 0, 0,1},
    {1, 0, 1,1},
    {0, 0, 1,1},

    // -z
    {0, 0, 0,1},
    {0, 1, 0,1},
    {1, 1, 0,1},
    {1, 0, 0,1},

    // diag 1a
    {0, 0, 0,1},
    {1, 1, 0,1},
    {1, 1, 1,1},
    {0, 0, 1,1},

    // diag 1b
    {0, 0, 0,1},
    {0, 0, 1,1},
    {1, 1, 1,1},
    {1, 1, 0,1},

    // diag 1a
    {1, 0, 0,1},
    {0, 1, 0,1},
    {0, 1, 1,1},
    {1, 0, 1,1},

    // diag 1b
    {1, 0, 0,1},
    {1, 0, 1,1},
    {0, 1, 1,1},
    {0, 1, 0,1}
};

const glm::vec4 texcrds[40] = {

    {0.125f, 0.0f,0,0},
    {0.249f, 0.0f,0,0},
    {0.249f, 0.124f,0,0},
    {0.125f, 0.124f,0,0},

    {0.25f, 0.0f,0,0},
    {0.374f, 0.0f,0,0},
    {0.374f, 0.124f,0,0},
    {0.25f, 0.124f,0,0},

    {0.0f, 0.0f,0,0},
    {0.124f, 0.0f,0,0},
    {0.124f, 0.124f,0,0},
    {0.0f, 0.124f,0,0},

    {0.625f, 0.0f,0,0},
    {0.749f, 0.0f,0,0},
    {0.749f, 0.124f,0,0},
    {0.625f, 0.124f,0,0},

    {0.5f, 0.0f,0,0},
    {0.624f, 0.0f,0,0},
    {0.624f, 0.124f,0,0},
    {0.5f, 0.124f,0,0},

    {0.375f, 0.0f,0,0},
    {0.499f, 0.0f,0,0},
    {0.499f, 0.124f,0,0},
    {0.375f, 0.124f,0,0},

    {0.75f, 0.124f,0,0},
    {0.874f, 0.124f,0,0},
    {0.874f, 0.0f,0,0},
    {0.75f, 0.0f,0,0},

    {0.75f, 0.124f,0,0},
    {0.75f, 0.0f,0,0},
    {0.874f, 0.0f,0,0},
    {0.874f, 0.124f,0,0},

    {0.75f, 0.124f,0,0},
    {0.874f, 0.124f,0,0},
    {0.874f, 0.0f,0,0},
    {0.75f, 0.0f,0,0},

    {0.75f, 0.124f,0,0},
    {0.75f, 0.0f,0,0},
    {0.874f, 0.0f,0,0},
    {0.874f, 0.124f,0,0}
};

BOOL loadExtensionFunctions()
{
    GETPROC(PFNGLACTIVETEXTUREPROC, glActiveTexture, "glActiveTexture");
    GETPROC(PFNGLBINDIMAGETEXTUREPROC, glBindImageTexture, "glBindImageTexture");
    GETPROC(PFNGLGETINTEGERI_VPROC, glGetIntegeri_v, "glGetIntegeri_v");
    GETPROC(PFNGLCREATESHADERPROC, glCreateShader, "glCreateShader");
    GETPROC(PFNGLSHADERSOURCEPROC, glShaderSource, "glShaderSource");
    GETPROC(PFNGLCOMPILESHADERPROC, glCompileShader, "glCompileShader");
    GETPROC(PFNGLCREATEPROGRAMPROC, glCreateProgram, "glCreateProgram");
    GETPROC(PFNGLATTACHSHADERPROC, glAttachShader, "glAttachShader");
    GETPROC(PFNGLLINKPROGRAMPROC, glLinkProgram, "glLinkProgram");
    GETPROC(PFNGLUSEPROGRAMPROC, glUseProgram, "glUseProgram");
    GETPROC(PFNGLDISPATCHCOMPUTEPROC, glDispatchCompute, "glDispatchCompute");
    GETPROC(PFNGLMEMORYBARRIERPROC, glMemoryBarrier, "glMemoryBarrier");
    GETPROC(PFNGLGETSHADERIVPROC, glGetShaderiv, "glGetShaderiv");
    GETPROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog, "glGetShaderInfoLog");
    GETPROC(PFNGLDELETESHADERPROC, glDeleteShader, "glDeleteShader");
    GETPROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv, "glGetProgramiv");
    GETPROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog, "glGetProgramInfoLog");
    GETPROC(PFNGLDELETEPROGRAMPROC, glDeleteProgram, "glDeleteProgram");
    GETPROC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation, "glGetUniformLocation");
    GETPROC(PFNGLUNIFORM4FVPROC, glUniform4fv, "glUniform4fv");
    GETPROC(PFNGLUNIFORM3FVPROC, glUniform3fv, "glUniform3fv");
    GETPROC(PFNGLUNIFORM1IPROC, glUniform1i, "glUniform1i");
    GETPROC(PFNGLUNIFORM2FVPROC, glUniform2fv, "glUniform2fv");
    GETPROC(PFNGLGENBUFFERSPROC, glGenBuffers, "glGenBuffers");
    GETPROC(PFNGLBINDBUFFERPROC, glBindBuffer, "glBindBuffer");
    GETPROC(PFNGLBUFFERDATAPROC, glBufferData, "glBufferData");
    GETPROC(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation, "glGetAttribLocation");
    GETPROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer, "glVertexAttribPointer");
    GETPROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray, "glEnableVertexAttribArray");
    GETPROC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays, "glGenVertexArrays");
    GETPROC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray, "glBindVertexArray");
    GETPROC(PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation, "glBindAttribLocation");
    GETPROC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv, "glUniformMatrix4fv");
    GETPROC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray, "glDisableVertexAttribArray");

    return TRUE;
cleanup:
    return FALSE;
}

char* readShaderSource(const char* filename)
{
    char* shaderSource = NULL;
    FILE* f = NULL;
    long l = 0;
    fopen_s(&f, filename, "r");
    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        l = ftell(f);
        fseek(f, 0, SEEK_SET);
        shaderSource = (char*)malloc(l);
        if (shaderSource != NULL) {
            memset(shaderSource, 0, l);
            fread(shaderSource, 1, l, f);
            fclose(f);
            f = NULL;
        }
        else {
            printf("Failed to allocate memory for shader source code\n");
            fclose(f);
            f = NULL;
            return NULL;
        }
    }
    else {
        printf("Failed to open shader source file\n");
        return NULL;
    }
    return shaderSource;
}

BOOL CreateAndCompile(const char* src, GLenum type, GLuint* pShader)
{
    GLuint shader = glCreateShader(type);
    GLint isCompiled = 0;
    GLint maxLength = 0;
    std::vector<GLchar> errorLog;

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    // check for compilation errors as per normal here
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        printf("Error compiling shader\n");

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        errorLog.resize(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        for (size_t i = 0; i < errorLog.size(); i++) {
            printf("%c", errorLog[i]);
        }
        printf("\n");

        // Provide the infolog in whatever manor you deem best. 
        // Exit with failure .
        glDeleteShader(shader); // Don't leak the shader.
        *pShader = 0;
        return FALSE;
    }
    else {
        *pShader = shader;
        return TRUE;
    }

}

BOOL LinkShader(GLuint prog, GLuint shader1, GLuint shader2)
{
    GLint isLinked = 0;
    GLint maxLength = 0;
    std::vector<GLchar> infoLog;

    glAttachShader(prog, shader1);
    if (shader2 > 0) glAttachShader(prog, shader2);
    glLinkProgram(prog);
    // check for linking errors and validate program as per normal here
    glGetProgramiv(prog, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        infoLog.resize(maxLength);
        glGetProgramInfoLog(prog, maxLength, &maxLength, &infoLog[0]);

        for (size_t i = 0; i < infoLog.size(); i++) {
            printf("%c", infoLog[i]);
        }
        printf("\n");
        return FALSE;
    }
    else {
        return TRUE;
    }

}

HGLRC createRenderingContext(HDC hdc)
{
    HGLRC hglrc = NULL;
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    int pf = ChoosePixelFormat(hdc, &pfd);

    if (pf == 0) {
        printf("Failed to find pixel format\n");
        return hglrc;
    }
    if (!SetPixelFormat(hdc, pf, &pfd))
    {
        printf("Failed to set pixel format\n");
        return hglrc;
    }

    hglrc = wglCreateContext(hdc);
    if (hglrc == NULL) {
        printf("Failed to create gl context\n");
        return hglrc;
    }
    if (!wglMakeCurrent(hdc, hglrc))
    {
        printf("Failed to make gl context current\n");
    }

    return hglrc;
}

DWORD WINAPI RenderThread(LPVOID parm)
{
    HWND hwnd = (HWND)parm;
    HDC hdc = GetDC(hwnd);
    HGLRC hglrc = createRenderingContext(hdc);
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    GLuint shaderProg = 0;

    glViewport(0, 0, 640, 480);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);

    if (FALSE == loadExtensionFunctions())
    {
        printf("Failed to load opengl extension functions.\n");
        return 0;
    }

    char* vshaderSource = readShaderSource("vshader.txt");
    if (!CreateAndCompile(vshaderSource, GL_VERTEX_SHADER, &vertexShader))
    {
        printf("Failed to create and compile vertex shader\n");
        free(vshaderSource);
        return 0;
    }
    free(vshaderSource);

    char* fshaderSource = readShaderSource("fshader.txt");
    if (!CreateAndCompile(fshaderSource, GL_FRAGMENT_SHADER, &fragmentShader))
    {
        printf("Failed to create and compile fragment shader\n");
        free(fshaderSource);
        return 0;
    }
    free(fshaderSource);

    shaderProg = glCreateProgram();
    if (!LinkShader(shaderProg, vertexShader, fragmentShader))
    {
        printf("Failed to link shader program\n");
        glDeleteProgram(shaderProg);
    }

    for (int i = 0; i < 6; i++) {
        vertices[i].vertex = locs[topVertexIndices[i]];
        vertices[i].texc = texcrds[topVertexIndices[i]];
    }
    for (int i = 6; i < 12; i++) {
        vertices[i].vertex = locs[bottomVertexIndices[i - 6]];
        vertices[i].texc = texcrds[bottomVertexIndices[i-6]];
    }
    for (int i = 12; i < 18; i++) {
        vertices[i].vertex = locs[plusxVertexIndices[i - 12]];
        vertices[i].texc = texcrds[plusxVertexIndices[i-12]];
    }
    for (int i = 18; i < 24; i++) {
        vertices[i].vertex = locs[minusxVertexIndices[i - 18]];
        vertices[i].texc = texcrds[minusxVertexIndices[i-18]];
    }
    for (int i = 24; i < 30; i++) {
        vertices[i].vertex = locs[plusyVertexIndices[i - 24]];
        vertices[i].texc = texcrds[plusyVertexIndices[i-24]];
    }
    for (int i = 30; i < 36; i++) {
        vertices[i].vertex = locs[minusyVertexIndices[i - 30]];
        vertices[i].texc = texcrds[minusyVertexIndices[i-30]];
    }
      
    glUseProgram(shaderProg);
     
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint modelm = glGetUniformLocation(shaderProg, "model");
    GLuint viewm = glGetUniformLocation(shaderProg, "view");
    GLuint projm = glGetUniformLocation(shaderProg, "proj");
         
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load("c:\\temp\\grassbox512tg1.png", &texWidth, &texHeight,
        &texChannels, STBI_rgb_alpha);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    free(pixels);

    glClearColor(0, 0, 0, 1);

    while(TRUE) 
    {
        if (WAIT_OBJECT_0 == WaitForSingleObject(hQuitEvent, 0)) break;

        float sinfElevation = sinf(DEG2RAD(elevation));
        sinfElevation = FCLAMP(sinfElevation, -0.99f, 0.99f);
        float invSinfElevation = 1.0f - fabs(sinfElevation);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(ex, ey, ez),
            glm::vec3(
                ex + (cosf(DEG2RAD(azimuth)) * invSinfElevation),
                ey + (sinf(DEG2RAD(azimuth)) * invSinfElevation),
                ez + sinfElevation),
            //glm::vec3(
                //ex + (cosf(DEG2RAD(azimuth)) * invSinfElevation),
                //ey + (sinf(DEG2RAD(azimuth)) * invSinfElevation),
                //ez + sinfElevation
            //),
            glm::vec3(0, 0, 1)
        );
        glm::mat4 projMatrix = glm::perspective(
            glm::radians(45.0f),
            640.0f / 480.0f,
            0.01f,
            500.0f);

        glUniformMatrix4fv(modelm, 1, false, &modelMatrix[0][0]);
        glUniformMatrix4fv(viewm, 1, false, &viewMatrix[0][0]);
        glUniformMatrix4fv(projm, 1, false, &projMatrix[0][0]);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2),
            (void*)sizeof(glm::vec3));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glFlush();

        SwapBuffers(hdc);
    }

    glUseProgram(0);
   
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProg);

    wglMakeCurrent(hdc, nullptr);
    ReleaseDC(hwnd, hdc);
    wglDeleteContext(hglrc);
    return 0;
}

LRESULT CALLBACK WndProc(
    _In_ HWND   hwnd,
    _In_ UINT   uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    UINT dwSize = 0;
    RAWINPUT* raw = (RAWINPUT*)rawBuffer;
    switch (uMsg)
    {
    case WM_INPUT:
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
        //printf("%i\n", dwSize);
        //LPBYTE lpb = new BYTE[dwSize];
        //if (lpb == NULL)
        //{
        //    return 0;
        //}

        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &rawBuffer, &dwSize, sizeof(RAWINPUTHEADER));

        if (raw->header.dwType == RIM_TYPEKEYBOARD)
        {
            if (raw->data.keyboard.Flags == 0) {
                switch (raw->data.keyboard.MakeCode)
                {
                case 17: // w
                    ex += cosf(DEG2RAD(azimuth));
                    ey += sinf(DEG2RAD(azimuth));
                    break;
                case 30: // a
                    ex += sinf(DEG2RAD(azimuth)) * -1.0f;
                    ey += cosf(DEG2RAD(azimuth));
                    break;
                case 31: // s
                    ex += cosf(DEG2RAD(azimuth)) * -1.0f;
                    ey += sinf(DEG2RAD(azimuth)) * -1.0f;
                    break;
                case 32: // d
                    ex += sinf(DEG2RAD(azimuth));
                    ey += cosf(DEG2RAD(azimuth)) * -1.0f;
                    break;
                case 16: // q
                    ez += 0.1f;
                    break;
                case 44: // z
                    ez -= 0.1f;
                    break;
                case 1:
                    DestroyWindow(hwnd);
                    break;
                default:
                    printf("kb %i\n", raw->data.keyboard.MakeCode);
                }
            }
            //printf("kb %i, %i, %i\n", raw->data.keyboard.MakeCode,
                //raw->data.keyboard.Flags == 0,
                //raw->data.keyboard.Flags & RI_KEY_BREAK);
        //    //hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT(" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n"),
        //    //    raw->data.keyboard.MakeCode,
        //    //    raw->data.keyboard.Flags,
        //    //    raw->data.keyboard.Reserved,
        //    //    raw->data.keyboard.ExtraInformation,
        //    //    raw->data.keyboard.Message,
        //    //    raw->data.keyboard.VKey);

        }
        else if (raw->header.dwType == RIM_TYPEMOUSE)
        {
            elevation -= (raw->data.mouse.lLastY / 20.0f);
            azimuth -= (raw->data.mouse.lLastX / 20.0f);
            //printf("mouse %i ,%i, %i\n", raw->data.mouse.usFlags,
                //raw->data.mouse.lLastX,
                //raw->data.mouse.lLastY);
        //    //hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
        //    //    raw->data.mouse.usFlags,
        //    //    raw->data.mouse.ulButtons,
        //    //    raw->data.mouse.usButtonFlags,
        //    //    raw->data.mouse.usButtonData,
        //    //    raw->data.mouse.ulRawButtons,
        //    //    raw->data.mouse.lLastX,
        //    //    raw->data.mouse.lLastY,
        //    //    raw->data.mouse.ulExtraInformation);
        }

        //delete[] lpb;
        return 0;
    case WM_CREATE:
        hQuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        hRenderThread = CreateThread(NULL, 0, RenderThread, (LPVOID)hwnd, 0, NULL);
        return 0;
    case WM_DESTROY:
        SetEvent(hQuitEvent);
        WaitForSingleObject(hRenderThread, INFINITE);
        CloseHandle(hQuitEvent);
        CloseHandle(hRenderThread);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

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
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
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