 
#include "voxc.h"

void addActorsForCurrentLocation(VOXC_WINDOW_CONTEXT* lpctx, int64_t xint, int64_t yint, int64_t zint)
{

    printf("add actors\n");

    int64_t idx = 0;

    // create all the rigid static actors
    // as an array in the crate vertex buffer algo
    // then, just pull them out of the array in this method

    //printf("location has changed; add new actors\n");

    // remove current actors
    if (lpctx->blocksAroundMe.size() > (size_t)0) {
        lpctx->mScene->removeActors(lpctx->blocksAroundMe.data(), (unsigned int)lpctx->blocksAroundMe.size());
    }

    // clear array
    lpctx->blocksAroundMe.clear();

    for (int64_t xbl = xint - 4; xbl < xint + 5; xbl++)
    {
        if (xbl >= 0 && xbl < X_GRID_EXTENT)
        {
            for (int64_t ybl = yint - 4; ybl < yint + 5; ybl++)
            {
                if (ybl >= 0 && ybl < Y_GRID_EXTENT)
                {
                    for (int64_t zbl = zint - 8; zbl < zint + 1; zbl++)
                    {
                        if (zbl >= 0 && zbl < Z_GRID_EXTENT)
                        {
                            idx = GRIDIDX(xbl, ybl, zbl);
                            if (lpctx->pBlockArray[idx] == 1) {
                                if (lpctx->pStaticBlockArray[idx]) lpctx->blocksAroundMe.push_back(lpctx->pStaticBlockArray[idx]);
                            }
                        }
                    }
                }
            }
        }
    }

    lpctx->mScene->addActors(lpctx->blocksAroundMe.data(), (unsigned int)lpctx->blocksAroundMe.size());

    lpctx->xblock = xint;
    lpctx->yblock = yint;
    lpctx->zblock = zint;
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
        glActiveTexture(GL_TEXTURE0);
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
    glActiveTexture(GL_TEXTURE0);
    for (; iter != lpctx->groups.end(); ++iter)
    {
        glVertexArrayVertexBuffer(lpctx->vao, 0, iter->vbo, 0, 8 * sizeof(GLfloat));
        glBindTexture(GL_TEXTURE_2D, iter->tid);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)iter->vertices.size());
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

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
    voxcProgram.Use();
    voxcProgram.SetUniform1i("texs", 0);
    voxcProgram.SetUniform1i("shadowMap", 1);
    OpenGlProgram shadowProg("vshadow.txt", "fshadow.txt");
    OpenGlProgram ddProg("vsh2d.txt", "fsh2d.txt");

    // if adding a new texture
    // increase the cound in load textures
    // also, go to voxc.h and add another define
    // also, in vertex buffer creation below, increase count
    const char* fnArray[] = {
        "vocxdirt.png",
        "vocxdirtgrass.png",
        "vocxgrass.png",
        "vocxleaves_t.png"

    };
    // after this, there will be one block group
    // for each texture 
    LoadTextures(fnArray, 4, lpctx); 

    // physics: must be done before create vertex buffer
    glm::vec3 startingPosition(10, 10, 50);
    initPhysics(lpctx, startingPosition);
    // end physics

    CreateVertexBuffer(lpctx);

    // vertex buffer
    std::vector<GLuint> vbos(4);
    glCreateBuffers(4, vbos.data());
    for(int i=0; i<4; i++)
    {
        glNamedBufferStorage(vbos[i], sizeof(VERTEX1) * lpctx->groups[i].vertices.size(),
            lpctx->groups[i].vertices.data(), 0);
        lpctx->groups[i].vbo = vbos[i];
    }

    // vbo for 2d quad
    GLuint quadVbo1 = 0;
    std::vector<VERTEX1> quadVerts = {
        { { 0.5, 0.5, 0 },{ 0, 0 }, {0,0,0} },
        { { 1, 0.5, 0 },{ 1, 0 }, {0,0,0} },
        { { 1, 1, 0 },{ 1, 1 }, {0,0,0} },
        { { 0.5, 0.5, 0 },{ 0, 0 }, {0,0,0} },
        { { 1, 1, 0 },{ 1, 1 }, {0,0,0} },
        { { 0.5, 1, 0 },{ 0, 1 }, {0,0,0} }
    };

    glCreateBuffers(1, &quadVbo1);
    glNamedBufferStorage(quadVbo1, sizeof(VERTEX1) * 6, quadVerts.data(), 0);

    // create the vertex array
    glCreateVertexArrays(1, &lpctx->vao);
    glVertexArrayAttribBinding(lpctx->vao, 0, 0);
    glVertexArrayAttribBinding(lpctx->vao, 1, 0);
    glVertexArrayAttribBinding(lpctx->vao, 2, 0);
    glVertexArrayAttribFormat(lpctx->vao, 0, 3, GL_FLOAT, FALSE, 0);
    glVertexArrayAttribFormat(lpctx->vao, 1, 2, GL_FLOAT, FALSE, 3 * sizeof(GLfloat));
    glVertexArrayAttribFormat(lpctx->vao, 2, 3, GL_FLOAT, FALSE, 5 * sizeof(GLfloat));
    glVertexArrayBindingDivisor(lpctx->vao, 0, 0);
    glEnableVertexArrayAttrib(lpctx->vao, 0);
    glEnableVertexArrayAttrib(lpctx->vao, 1);
    glEnableVertexArrayAttrib(lpctx->vao, 2);
    // end vertex array 

    // create a frame buffer for shadows 
    GLuint depthMapFBO = 0;
    glGenFramebuffers(1, &depthMapFBO);

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    GLuint depthMap = 0;
    glGenTextures(1, &depthMap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
     
    float near_plane = 0.1f, far_plane = 500.0f;
    glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
    glm::vec3 eye1 = glm::vec3(0, 0, 200);
    glm::vec3 cntr1 = glm::vec3(X_GRID_EXTENT / 3, Y_GRID_EXTENT / 3, 50);
    glm::mat4 lightView1 = glm::lookAt(eye1, cntr1, glm::vec3(0.0f, 0.0f, 1.0f)); 
    //glm::mat4 lightSpaceMatrix1 = lightProjection * lightView1;
    //glm::vec3 lightDir = glm::normalize(cntr1 - eye1);
    // end shadows

    glClearColor(0.9f, 0.9f, 1.0f, 1.0f);

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    LARGE_INTEGER perfCount = { 0 };
    LARGE_INTEGER perfFreq = { 0 };
    LARGE_INTEGER lastCount = { 0 };
    int64_t elapsedTicks = 0;

    QueryPerformanceCounter(&perfCount);
    lastCount = perfCount;
    QueryPerformanceFrequency(&perfFreq); // counts per second

    physx::PxControllerFilters mCCFilters = { 0 };
    physx::PxExtendedVec3 pos;

    while (TRUE)
    {
        if (WAIT_OBJECT_0 == WaitForSingleObject(lpctx->hQuitEvent, 0)) break;

        // calculate elapsed seconds
        QueryPerformanceCounter(&perfCount);
        elapsedTicks = perfCount.QuadPart - lastCount.QuadPart;
        float elapsed = (float)elapsedTicks / (float)perfFreq.QuadPart;

        // physics here

        if (lpctx->ActorsAdded == false) {
            addActorsForCurrentLocation(lpctx, 
                (int64_t)startingPosition.x, 
                (int64_t)startingPosition.y, 
                (int64_t)startingPosition.z);
            lpctx->ActorsAdded = true;
        }

        // apply gravity to vz
        if (lpctx->keys[4] == 1)
        {
            lpctx->vz = 0.5f;
            lpctx->keys[4] = 0;
        }
        else {
            lpctx->vz += (float)elapsed / -1000.0f;
        }

        //case 17: // w
        ////lpctx->mx += cosf(DEG2RAD(lpctx->azimuth));
        ////lpctx->my += sinf(DEG2RAD(lpctx->azimuth));
        //lpctx->keys[0] = 1;
        //break;
        //case 30: // a
        ////lpctx->mx += sinf(DEG2RAD(lpctx->azimuth)) * -1.0f;
        ////lpctx->my += cosf(DEG2RAD(lpctx->azimuth));
        //lpctx->keys[1] = 1;
        //break;
        //case 31: // s
        ////lpctx->mx += cosf(DEG2RAD(lpctx->azimuth)) * -1.0f;
        ////lpctx->my += sinf(DEG2RAD(lpctx->azimuth)) * -1.0f;
        //lpctx->keys[2] = 1;
        //break;
        //case 32: // d
        ////lpctx->mx += sinf(DEG2RAD(lpctx->azimuth));
        ////lpctx->my += cosf(DEG2RAD(lpctx->azimuth)) * -1.0f;
        //lpctx->keys[3] = 1;
        //break;

        float speed = elapsed / 20.0f;
        float mx = 0.0f, my = 0.0f;
        if (lpctx->keys[0] == 1) { // w
            mx += cosf(DEG2RAD(lpctx->azimuth)) * speed;
            my += sinf(DEG2RAD(lpctx->azimuth)) * speed;
        }
        else if (lpctx->keys[1] == 1) // a
        {
            mx += cosf(DEG2RAD(lpctx->azimuth)) * -speed;
            my += sinf(DEG2RAD(lpctx->azimuth)) * -speed;
        }
        if (lpctx->keys[2] == 1) // s
        {
            mx += sinf(DEG2RAD(lpctx->azimuth)) * -speed;
            my += cosf(DEG2RAD(lpctx->azimuth)) * speed;
        }
        else if (lpctx->keys[3] == 1) // d
        {
            mx += sinf(DEG2RAD(lpctx->azimuth)) * speed;
            my += cosf(DEG2RAD(lpctx->azimuth)) * -speed;
        }

        physx::PxExtendedVec3 posb = lpctx->mController->getPosition();
        physx::PxControllerCollisionFlags cflags = lpctx->mController->move(
            physx::PxVec3(mx, my, lpctx->vz * elapsed),
            0.00f, // min dist
            elapsed, mCCFilters);

        if (cflags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN))
        {
            lpctx->vz = 0.0f;
        }
        lpctx->mScene->simulate(elapsed);
        lpctx->mScene->fetchResults(true);

        pos = lpctx->mController->getPosition();

        if (abs(lpctx->xblock - (int64_t)pos.x) > 4) {
            addActorsForCurrentLocation(lpctx, (int64_t)pos.x, (int64_t)pos.y, (int64_t)pos.z);
        }
        else if (abs(lpctx->yblock - (int64_t)pos.y) > 4) {
            addActorsForCurrentLocation(lpctx, (int64_t)pos.x, (int64_t)pos.y, (int64_t)pos.z);
        }
        else if (abs(lpctx->zblock - (int64_t)pos.z) > 4)
        {
            addActorsForCurrentLocation(lpctx, (int64_t)pos.x, (int64_t)pos.y, (int64_t)pos.z);
        }

        // physics done

        float sinfElevation = sinf(DEG2RAD(lpctx->elevation));
        sinfElevation = FCLAMP(sinfElevation, -0.99f, 0.99f);
        float invSinfElevation = 1.0f - fabs(sinfElevation);

        shadowProg.Use();
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        //glm::vec3 eye = glm::vec3(lpctx->ex - 100, lpctx->ey - 100, lpctx->ez + 100);
        glm::vec3 lightLooking = glm::vec3(pos.x - 100, pos.y - 100, pos.z + 100);
        //glm::vec3 cntr = glm::vec3(lpctx->ex, lpctx->ey, lpctx->ez);
        glm::vec3 posLocation3 = glm::vec3(pos.x, pos.y, pos.z);
        glm::mat4 lightView = glm::lookAt(lightLooking, posLocation3, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        glm::vec3 lightDir = glm::normalize(posLocation3 - lightLooking);

        shadowProg.SetUniformMatrix4fv("lightSpaceMatrix", &lightSpaceMatrix[0][0]);
        shadowProg.SetUniformMatrix4fv("model", &modelMatrix[0][0]);
        RenderScene(lpctx);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
         
        voxcProgram.Use();

        glm::mat4 viewMatrix = glm::lookAt(
            //glm::vec3(lpctx->ex, lpctx->ey, lpctx->ez),
            posLocation3,
            glm::vec3(
                pos.x + (cosf(DEG2RAD(lpctx->azimuth)) * invSinfElevation),
                pos.y + (sinf(DEG2RAD(lpctx->azimuth)) * invSinfElevation),
                pos.z + sinfElevation),
            glm::vec3(0, 0, 1)
        );
        glm::mat4 projMatrix = glm::perspective(
            glm::radians(45.0f),
            lpctx->viewportRatio,
            0.01f,
            500.0f);
        
        voxcProgram.SetUniformMatrix4fv("model", &modelMatrix[0][0]);
        voxcProgram.SetUniformMatrix4fv("view", &viewMatrix[0][0]);
        voxcProgram.SetUniformMatrix4fv("proj", &projMatrix[0][0]);
        voxcProgram.SetUniformMatrix4fv("lightSpaceMatrix", &lightSpaceMatrix[0][0]);
        voxcProgram.SetUniform3fv("lightDir", &lightDir[0]);
         
        glViewport(0, 0, lpctx->screenWidth, lpctx->screenHeight);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glBindVertexArray(lpctx->vao);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        RenderScene(lpctx);
          
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
         
        ddProg.Use();
        glVertexArrayVertexBuffer(lpctx->vao, 0, quadVbo1, 0, 8 * sizeof(GLfloat));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, 0);

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
    for (int64_t xc = 0; xc < X_GRID_EXTENT; xc++) {
        for (int64_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
            for (int64_t zc = 0; zc < Z_GRID_EXTENT; zc++) {
                int64_t idx = GRIDIDX(xc, yc, zc);
                if (lpctx->pStaticBlockArray[idx]) lpctx->pStaticBlockArray[idx]->release();
            }
        }
    }
    free(lpctx->pStaticBlockArray);

    cleanupPhysics(lpctx);

    return 0;
}

