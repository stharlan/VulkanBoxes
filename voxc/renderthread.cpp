
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
        glVertexArrayVertexBuffer(lpctx->vao, 0, iter->vbo, 0, 5 * sizeof(GLfloat));
        glBindTexture(GL_TEXTURE_2D, iter->tid);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)iter->vertices.size());
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

//DWORD WINAPI RenderThread(LPVOID parm)
//{
//    HWND hwnd = (HWND)parm;
//    HDC hdc = GetDC(hwnd);
//    HGLRC hglrc = createRenderingContext(hdc);
//
//    if (FALSE == loadExtensionFunctions())
//    {
//        printf("Failed to load opengl extension functions.\n");
//        return 0;
//    }
//
//    OpenGlProgram ddProg("vsh2d.txt", "fsh2d.txt");
//
//    GLuint quadVbo1 = 0;
//    //std::vector<VERTEX> quadVerts = {
//    //    { { -0.5, -0.5, 0 },{ 0, 0 } },
//    //    { {  0.5, -0.5, 0 },{ 1, 0 } },
//    //    { {  0.5,  0.5, 0 },{ 1, 1 } },
//    //    { { -0.5, -0.5, 0 },{ 0, 0 } },
//    //    { {  0.5,  0.5, 0 },{ 1, 1 } },
//    //    { { -0.5,  0.5, 0 },{ 0, 1 } }
//    //};
//    std::vector<glm::vec3> quadVerts = {
//        { -0.5, -0.5, 0 },
//        {  0.5, -0.5, 0 },
//        {  0.5,  0.5, 0 },
//        { -0.5, -0.5, 0 },
//        {  0.5,  0.5, 0 },
//        { -0.5,  0.5, 0 }
//    };
//    glCreateBuffers(1, &quadVbo1);
//    printf("%i\n", glGetError());
//    glNamedBufferStorage(quadVbo1, sizeof(glm::vec3) * 6, quadVerts.data(), 0);
//
//    // create the vertex array
//    GLuint vao = 0;
//    glCreateVertexArrays(1, &vao);
//    glVertexArrayAttribBinding(vao, 0, 0);
//    //glVertexArrayAttribBinding(vao, 1, 0);
//    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, FALSE, 0);
//    //glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, FALSE, 3 * sizeof(GLfloat));
//    glVertexArrayBindingDivisor(vao, 0, 0);
//    glEnableVertexArrayAttrib(vao, 0);
//    //glEnableVertexArrayAttrib(vao, 1);
//    // end vertex array
//    
//    ddProg.Use();
//    glClear(GL_COLOR_BUFFER_BIT);
//    glBindVertexArray(vao);
//    glVertexArrayVertexBuffer(vao, 0, quadVbo1, 0, 3 * sizeof(GLfloat));
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    //glBegin(GL_TRIANGLES);
//    //glVertex3f(0, 0, 0);
//    //glVertex3f(0, 1, 0);
//    //glVertex3f(1, 1, 0);
//    //glEnd();
//    SwapBuffers(hdc);
//    return 0;
//}

DWORD WINAPI RenderThread(LPVOID parm)
{
    HWND hwnd = (HWND)parm;
    VOXC_WINDOW_CONTEXT* lpctx = (VOXC_WINDOW_CONTEXT*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    HDC hdc = GetDC(hwnd);
    HGLRC hglrc = createRenderingContext(hdc);

    if (FALSE == loadExtensionFunctions())
    {
        printf("Failed to load opengl extension functions.\n");
        return 0;
    }

    glViewport(0, 0, lpctx->screenWidth, lpctx->screenHeight);
    glFrontFace(GL_CCW);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_TEXTURE_2D);

    OpenGlProgram voxcProgram("vshader.txt", "fshader.txt");
    OpenGlProgram shadowProg("vshadow.txt", "fshadow.txt");
    OpenGlProgram ddProg("vsh2d.txt", "fsh2d.txt");

    const char* fnArray[] = {
        "c:\\temp\\vocxdirt.png",
        "c:\\temp\\vocxdirtgrass.png",
        "c:\\temp\\vocxgrass.png"
    };
    LoadTextures(fnArray, 3, lpctx); 

    CreateVertexBuffer(lpctx);

    // vertex buffer
    std::vector<GLuint> vbos(3);
    glCreateBuffers(3, vbos.data());
    for(int i=0; i<3; i++)
    {
        glNamedBufferStorage(vbos[i], sizeof(VERTEX) * lpctx->groups[i].vertices.size(),
            lpctx->groups[i].vertices.data(), 0);
        lpctx->groups[i].vbo = vbos[i];
    }

    // vbo for 2d quad
    GLuint quadVbo1 = 0;
    std::vector<VERTEX> quadVerts = {
        { { -0.5, -0.5, 0 },{ 0, 0 } },
        { { 0.5, -0.5, 0 },{ 1, 0 } },
        { { 0.5, 0.5, 0 },{ 1, 1 } },
        { { -0.5, -0.5, 0 },{ 0, 0 } },
        { { 0.5, 0.5, 0 },{ 1, 1 } },
        { { -0.5, 0.5, 0 },{ 0, 1 } }
    };
    glCreateBuffers(1, &quadVbo1);
    glNamedBufferStorage(quadVbo1, sizeof(VERTEX) * 6, quadVerts.data(), 0);

    // create the vertex array
    glCreateVertexArrays(1, &lpctx->vao);
    glVertexArrayAttribBinding(lpctx->vao, 0, 0);
    glVertexArrayAttribBinding(lpctx->vao, 1, 0);
    glVertexArrayAttribFormat(lpctx->vao, 0, 3, GL_FLOAT, FALSE, 0);
    glVertexArrayAttribFormat(lpctx->vao, 1, 2, GL_FLOAT, FALSE, 3 * sizeof(GLfloat));
    glVertexArrayBindingDivisor(lpctx->vao, 0, 0);
    glEnableVertexArrayAttrib(lpctx->vao, 0);
    glEnableVertexArrayAttrib(lpctx->vao, 1);
    // end vertex array

    // create a frame buffer for shadows
    GLuint depthMapFBO = 0;
    glGenFramebuffers(1, &depthMapFBO);

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    GLuint depthMap = 0;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float near_plane = 0.1f, far_plane = 500.0f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(
        glm::vec3(0,0,Z_GRID_EXTENT / 2),
        glm::vec3(1, 1, 0),
        glm::vec3(0.0f, 0.0f, 1.0f)); 
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    // end shadows

    glClearColor(0.9f, 0.9f, 1.0f, 1.0f);

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    while (TRUE)
    {
        if (WAIT_OBJECT_0 == WaitForSingleObject(lpctx->hQuitEvent, 0)) break;

        float sinfElevation = sinf(DEG2RAD(lpctx->elevation));
        sinfElevation = FCLAMP(sinfElevation, -0.99f, 0.99f);
        float invSinfElevation = 1.0f - fabs(sinfElevation);

        shadowProg.Use();
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        lightView = glm::lookAt(
            glm::vec3(lpctx->ex, lpctx->ey, lpctx->ez),
            glm::vec3(
                lpctx->ex + (cosf(DEG2RAD(lpctx->azimuth)) * invSinfElevation),
                lpctx->ey + (sinf(DEG2RAD(lpctx->azimuth)) * invSinfElevation),
                lpctx->ez + sinfElevation),
            glm::vec3(0, 0, 1));
        lightSpaceMatrix = lightProjection * lightView;

        shadowProg.SetUniform("lightSpaceMatrix", &lightSpaceMatrix[0][0]);
        shadowProg.SetUniform("model", &modelMatrix[0][0]);
        RenderScene(lpctx);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        voxcProgram.Use();

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
        
        voxcProgram.SetUniform("model", &modelMatrix[0][0]);
        voxcProgram.SetUniform("view", &viewMatrix[0][0]);
        voxcProgram.SetUniform("proj", &projMatrix[0][0]);

        glViewport(0, 0, lpctx->screenWidth, lpctx->screenHeight);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glBindVertexArray(lpctx->vao);

        RenderScene(lpctx);

        ddProg.Use();
        glVertexArrayVertexBuffer(lpctx->vao, 0, quadVbo1, 0, 5 * sizeof(GLfloat));
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glFlush();

        SwapBuffers(hdc);

        glUseProgram(0);

    }

    glUseProgram(0);
   
    glDeleteBuffers(1, &quadVbo1);

    glDeleteTextures(1, &depthMap);
    glDeleteFramebuffers(1, &depthMapFBO);

    std::vector<VERTEX_BUFFER_GROUP1>::iterator iter = lpctx->groups.begin();
    for (; iter != lpctx->groups.end(); ++iter)
    {
        glDeleteTextures(1, &iter->tid);
        glDeleteBuffers(1, &iter->vbo);
    }

    glDeleteVertexArrays(1, &lpctx->vao);

    wglMakeCurrent(hdc, nullptr);
    ReleaseDC(hwnd, hdc);
    wglDeleteContext(hglrc);

    free(lpctx->pBlockArray);

    return 0;
}

