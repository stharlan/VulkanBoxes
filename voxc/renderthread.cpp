
#include "voxc.h"

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

DWORD WINAPI RenderThread(LPVOID parm)
{
    HWND hwnd = (HWND)parm;
    VOXC_WINDOW_CONTEXT* lpctx = (VOXC_WINDOW_CONTEXT*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    HDC hdc = GetDC(hwnd);
    HGLRC hglrc = createRenderingContext(hdc);
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    GLuint shaderProg = 0;

    glViewport(0, 0, 1024, 768);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

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

    CreateVertexBuffer(lpctx);

    glUseProgram(shaderProg);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * lpctx->vertices4.size(),
        lpctx->vertices4.data(), GL_STATIC_DRAW);

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
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    stbi_image_free(pixels);

    glClearColor(0, 0, 0, 1);

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    LARGE_INTEGER ct;
    QueryPerformanceCounter(&ct);
    //LARGE_INTEGER ct1;

    while (TRUE)
    {
        if (WAIT_OBJECT_0 == WaitForSingleObject(lpctx->hQuitEvent, 0)) break;

        float sinfElevation = sinf(DEG2RAD(lpctx->elevation));
        sinfElevation = FCLAMP(sinfElevation, -0.99f, 0.99f);
        float invSinfElevation = 1.0f - fabs(sinfElevation);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(lpctx->ex, lpctx->ey, lpctx->ez),
            glm::vec3(
                lpctx->ex + (cosf(DEG2RAD(lpctx->azimuth)) * invSinfElevation),
                lpctx->ey + (sinf(DEG2RAD(lpctx->azimuth)) * invSinfElevation),
                lpctx->ez + sinfElevation),
            glm::vec3(0, 0, 1)
        );
        glm::mat4 projMatrix = glm::perspective(
            glm::radians(45.0f),
            1024.0f / 768.0f,
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

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)lpctx->vertices4.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glFlush();

        SwapBuffers(hdc);

        //QueryPerformanceCounter(&ct1);

        //printf("%.1f fps; %lli counts\n",
            //(double)freq.QuadPart / (double)(ct1.QuadPart - ct.QuadPart),
            //ct1.QuadPart - ct.QuadPart);
        //ct = ct1;

    }

    glUseProgram(0);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProg);

    wglMakeCurrent(hdc, nullptr);
    ReleaseDC(hwnd, hdc);
    wglDeleteContext(hglrc);

    free(lpctx->pBlockArray);

    return 0;
}

