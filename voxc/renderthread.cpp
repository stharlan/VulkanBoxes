
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

//#define noiseWidth 64
//#define noiseHeight 64
//double noise[noiseHeight][noiseWidth]; //the noise array
//
//void generateNoise()
//{
//    for (int y = 0; y < noiseHeight; y++)
//        for (int x = 0; x < noiseWidth; x++)
//        {
//            noise[y][x] = (rand() % 32768) / 32768.0;
//        }
//}
//
//double smoothNoise(double x, double y)
//{
//    //get fractional part of x and y
//    double fractX = x - int(x);
//    double fractY = y - int(y);
//
//    //wrap around
//    int x1 = (int(x) + noiseWidth) % noiseWidth;
//    int y1 = (int(y) + noiseHeight) % noiseHeight;
//
//    //neighbor values
//    int x2 = (x1 + noiseWidth - 1) % noiseWidth;
//    int y2 = (y1 + noiseHeight - 1) % noiseHeight;
//
//    //smooth the noise with bilinear interpolation
//    double value = 0.0;
//    value += fractX * fractY * noise[y1][x1];
//    value += (1 - fractX) * fractY * noise[y1][x2];
//    value += fractX * (1 - fractY) * noise[y2][x1];
//    value += (1 - fractX) * (1 - fractY) * noise[y2][x2];
//
//    return value;
//}
//
//double turbulence(double x, double y, double size)
//{
//    double value = 0.0, initialSize = size;
//
//    while (size >= 1)
//    {
//        value += smoothNoise(x / size, y / size) * size;
//        size /= 2.0;
//    }
//
//    return(128.0 * value / initialSize);
//}
//
//GLuint GenerateDirtTexture()
//{
//    srand((unsigned int)time(NULL));
//
//    GLuint textureID;
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//
//    // create pixels here
//    uint64_t npixels = 64 * 64;
//    uint64_t nbytes = npixels * 4;
//    BYTE* pixels = (BYTE*)malloc(nbytes);
//    memset(pixels, 0, nbytes);
//
//    uint32_t b1 = 0xff052940;
//    uint32_t bc[4] = {
//        0xff2b5576,
//        0xff28556b,
//        0xff659eb5,
//        0xffa9d1eb
//    };
//    //for (uint64_t i = 0; i < nbytes; i+=4)
//    //{
//    //    if (rand() % 10 == 1) {
//    //        memcpy(pixels + i, &bc[rand() % 4], 4);
//    //    }
//    //    else {
//    //        memcpy(pixels + i, &b1, 4);
//    //    }        
//    //}
//    generateNoise();
//    for (int y = 0; y < 64; y++) {
//        for (int x = 0; x < 64; x++) {
//            BYTE b = turbulence(x, y, 64);
//            uint32_t clr = 0xff000000 + (b << 16) + (b << 8) + b;
//            memcpy(pixels + (y * 64 * 4) + (x * 4), &clr, 4);
//        }
//    }
//
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
//    glGenerateMipmap(GL_TEXTURE_2D);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
//
//    free(pixels);
//
//    return textureID;
//}

void LoadTextures(const char* filename[], int numFilenames, VOXC_WINDOW_CONTEXT* lpctx)
{
    std::vector<GLuint> texids(numFilenames);
    lpctx->groups.resize(numFilenames);
    glGenTextures(numFilenames, texids.data());
    int texWidth, texHeight, texChannels;
    for (int i = 0; i < numFilenames; i++) {
        stbi_uc* pixels = stbi_load(filename[i], &texWidth, &texHeight,
            &texChannels, STBI_rgb_alpha);
        glBindTexture(GL_TEXTURE_2D, texids[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        stbi_image_free(pixels);
        lpctx->groups[i] = { texids[i], 0, {} };
    }
}

void RenderScene(VOXC_WINDOW_CONTEXT* lpctx)
{
    std::vector<VERTEX_BUFFER_GROUP1>::iterator iter = lpctx->groups.begin();
    for (; iter != lpctx->groups.end(); ++iter)
    {
        glBindBuffer(GL_ARRAY_BUFFER, iter->vbo);
        glBindTexture(GL_TEXTURE_2D, iter->tid);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2),
            (void*)sizeof(glm::vec3));

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)iter->vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
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
    GLuint vShadowShader = 0;
    GLuint fShadowShader = 0;
    GLuint shadowProg = 0;

    glViewport(0, 0, lpctx->screenWidth, lpctx->screenHeight);
    glFrontFace(GL_CCW);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_TEXTURE_2D);

    if (FALSE == loadExtensionFunctions())
    {
        printf("Failed to load opengl extension functions.\n");
        return 0;
    }

    char* vshaderSource = readShaderSource("vshader.txt");
    CreateAndCompile(vshaderSource, GL_VERTEX_SHADER, &vertexShader);
    free(vshaderSource);
    char* fshaderSource = readShaderSource("fshader.txt");
    CreateAndCompile(fshaderSource, GL_FRAGMENT_SHADER, &fragmentShader);
    free(fshaderSource);
    shaderProg = glCreateProgram();
    LinkShader(shaderProg, vertexShader, fragmentShader);

    const char* fnArray[] = {
        "c:\\temp\\vocxdirt.png",
        "c:\\temp\\vocxdirtgrass.png",
        "c:\\temp\\vocxgrass.png"
    };
    LoadTextures(fnArray, 3, lpctx);

    CreateVertexBuffer(lpctx);

    std::vector<GLuint> vbos(3);
    glGenBuffers(3, vbos.data());
    for(int i=0; i<3; i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * lpctx->groups[i].vertices.size(),
            lpctx->groups[i].vertices.data(), GL_STATIC_DRAW);
        lpctx->groups[i].vbo = vbos[i];
    }

    glUseProgram(shaderProg);
    GLuint modelm = glGetUniformLocation(shaderProg, "model");
    GLuint viewm = glGetUniformLocation(shaderProg, "view");
    GLuint projm = glGetUniformLocation(shaderProg, "proj");

    glClearColor(0, 0, 0, 1);

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    while (TRUE)
    {
        if (WAIT_OBJECT_0 == WaitForSingleObject(lpctx->hQuitEvent, 0)) break;

        glUseProgram(shaderProg);

        float sinfElevation = sinf(DEG2RAD(lpctx->elevation));
        sinfElevation = FCLAMP(sinfElevation, -0.99f, 0.99f);
        float invSinfElevation = 1.0f - fabs(sinfElevation);

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
            lpctx->viewportRatio,
            0.01f,
            500.0f);

        glUniformMatrix4fv(modelm, 1, false, &modelMatrix[0][0]);
        glUniformMatrix4fv(viewm, 1, false, &viewMatrix[0][0]);
        glUniformMatrix4fv(projm, 1, false, &projMatrix[0][0]);

        glViewport(0, 0, lpctx->screenWidth, lpctx->screenHeight);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        RenderScene(lpctx);

        //glUseProgram(0);

        //glBegin(GL_TRIANGLES);
        //glTexCoord2f(0, 0);
        //glVertex2f(0, 0);
        //glTexCoord2f(1, 0);
        //glVertex2f(1, 0);
        //glTexCoord2f(1, 1);
        //glVertex2f(1, 1);
        //glTexCoord2f(0, 0);
        //glVertex2f(0, 0);
        //glTexCoord2f(1, 1);
        //glVertex2f(1, 1);
        //glTexCoord2f(0, 1);
        //glVertex2f(0, 1);
        //glEnd();
        //glBindTexture(GL_TEXTURE_2D, 0);

        glFlush();

        SwapBuffers(hdc);

    }

    glUseProgram(0);

    std::vector<VERTEX_BUFFER_GROUP1>::iterator iter = lpctx->groups.begin();
    for (; iter != lpctx->groups.end(); ++iter)
    {
        glDeleteTextures(1, &iter->tid);
        glDeleteBuffers(1, &iter->vbo);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProg);

    wglMakeCurrent(hdc, nullptr);
    ReleaseDC(hwnd, hdc);
    wglDeleteContext(hglrc);

    free(lpctx->pBlockArray);

    return 0;
}

