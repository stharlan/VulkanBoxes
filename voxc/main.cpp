  
#define STB_IMAGE_IMPLEMENTATION
#include "voxc.h"

HANDLE hQuitEvent = 0;
HANDLE hRenderThread = 0;

BYTE* rawBuffer[48];

float ex = 10.0f;
float ey = 10.0f;
float ez = 50.0f;
float elevation = 0.0f;
float azimuth = 0.0f;

GLuint topVertexIndices[6] = { 0, 1, 2, 2, 3, 0 };
GLuint plusxVertexIndices[6] = { 4, 5, 6, 6, 7, 4 };
GLuint minusxVertexIndices[6] = { 8, 9, 10, 10, 11, 8 };
GLuint plusyVertexIndices[6] = { 12, 13, 14, 14, 15, 12 };
GLuint minusyVertexIndices[6] = { 16, 17, 18, 18, 19, 16 };
GLuint bottomVertexIndices[6] = { 20, 21, 22, 22, 23, 20 };
 
int8_t* pBlockArray; // [X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT] ;

typedef struct _VERTEX
{
    glm::vec3 vertex;
    glm::vec2 texc;
} VERTEX;

//VERTEX vertices[36];
std::vector<VERTEX> vertices4;
        
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
    GETPROC(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap, "glGenerateMipmap");

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

void CreateVertexBuffer()
{
    int texWidth = 0;
    int texHeight = 0;
    int texChannels = 0;
    stbi_uc* pixels = stbi_load("c:\\temp\\height8.png", &texWidth, &texHeight,
        &texChannels, 1);

    pBlockArray = (int8_t*)malloc(X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT * sizeof(int8_t));
    memset(pBlockArray, 0, X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT * sizeof(int8_t));

    // assign blocks to level 0
    for (int64_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
        for (int64_t xc = 0; xc < X_GRID_EXTENT; xc++) {

            int h = pixels[(yc * texWidth) + xc];

            if (h < 1) h = 1;
            for (int64_t zc = 0; zc < h; zc++) {
                pBlockArray[GRIDIDX(xc, yc, zc)] = 1;
            }

            // if the block is less than the z extent
            // put veg at the top
            if (h < Z_GRID_EXTENT) {
                if (rand() % 100 == 1)
                {
                    // flower
                    pBlockArray[GRIDIDX(xc, yc, h)] = 2;
                }
                else if (rand() % 1000 == 1) {
                    // tree trunk
                    for (uint64_t th = 0; th < 6; th++) {
                        if ((h + th) < Z_GRID_EXTENT) {
                            if (th == 5) {
                                pBlockArray[GRIDIDX(xc, yc, h + th)] = 4;
                            }
                            else {
                                pBlockArray[GRIDIDX(xc, yc, h + th)] = 3;
                            }
                        }
                    }
                }
            }
        }
    }

    stbi_image_free(pixels);

    for (int64_t zc = 0; zc < Z_GRID_EXTENT; zc++) {
        for (int64_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
            for (int64_t xc = 0; xc < X_GRID_EXTENT; xc++) {

                glm::mat4 xlate = glm::translate(glm::mat4(1.0f), glm::vec3(xc, yc, zc));

                int64_t idx = GRIDIDX(xc, yc, zc);

                if (pBlockArray[idx] == 1
                    || pBlockArray[idx] == 3
                    || pBlockArray[idx] == 4) {

                    float texOffset = 0.0f;
                    if (pBlockArray[idx] == 3) texOffset = 0.125f;
                    if (pBlockArray[idx] == 4) texOffset = 0.25f;

                    size_t numVerts = vertices4.size();

                    // check for block on bottom (-z)
                    if (zc > 0) {
                        if (pBlockArray[GRIDIDX(xc, yc, zc - 1)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                //vertices4.push_back({ bottomVertexIndices[v], {xc, yc, zc}, texOffset });
                                vertices4.push_back({xlate * locs[bottomVertexIndices[v]],texcrds[bottomVertexIndices[v]]});
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            //vertices4.push_back({ bottomVertexIndices[v], {xc, yc, zc}, texOffset });
                            vertices4.push_back({ xlate * locs[bottomVertexIndices[v]],texcrds[bottomVertexIndices[v]] });
                        }
                    }

                    // check for block on top (+z)
                    if (zc < Z_GRID_EXTENT - 1)
                    {
                        if (pBlockArray[GRIDIDX(xc, yc, zc + 1)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                //vertices4.push_back({ topVertexIndices[v], {xc, yc, zc}, texOffset });
                                vertices4.push_back({ xlate * locs[topVertexIndices[v]],texcrds[topVertexIndices[v]] });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            //vertices4.push_back({ topVertexIndices[v], {xc, yc, zc}, texOffset });
                            vertices4.push_back({ xlate * locs[topVertexIndices[v]],texcrds[topVertexIndices[v]] });
                        }
                    }

                    // check for block on +x
                    if (xc < X_GRID_EXTENT - 1)
                    {
                        if (pBlockArray[GRIDIDX(xc + 1, yc, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                //vertices4.push_back({ plusxVertexIndices[v], {xc, yc, zc}, texOffset });
                                vertices4.push_back({ xlate * locs[plusxVertexIndices[v]],texcrds[plusxVertexIndices[v]] });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            //vertices4.push_back({ plusxVertexIndices[v], {xc, yc, zc}, texOffset });
                            vertices4.push_back({ xlate * locs[plusxVertexIndices[v]],texcrds[plusxVertexIndices[v]] });
                        }
                    }

                    // check for block on -x
                    if (xc > 0)
                    {
                        if (pBlockArray[GRIDIDX(xc - 1, yc, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                //vertices4.push_back({ minusxVertexIndices[v], {xc, yc, zc}, texOffset });
                                vertices4.push_back({ xlate * locs[minusxVertexIndices[v]],texcrds[minusxVertexIndices[v]] });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            //vertices4.push_back({ minusxVertexIndices[v], {xc, yc, zc}, texOffset });
                            vertices4.push_back({ xlate * locs[minusxVertexIndices[v]],texcrds[minusxVertexIndices[v]] });
                        }
                    }

                    // check for block on +y
                    if (yc < Y_GRID_EXTENT - 1)
                    {
                        if (pBlockArray[GRIDIDX(xc, yc + 1, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                //vertices4.push_back({ plusyVertexIndices[v], {xc, yc, zc}, texOffset });
                                vertices4.push_back({ xlate * locs[plusyVertexIndices[v]],texcrds[plusyVertexIndices[v]] });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            //vertices4.push_back({ plusyVertexIndices[v], {xc, yc, zc}, texOffset });
                            vertices4.push_back({ xlate * locs[plusyVertexIndices[v]],texcrds[plusyVertexIndices[v]] });
                        }
                    }

                    // check for block on -y
                    if (yc > 0)
                    {
                        if (pBlockArray[GRIDIDX(xc, yc - 1, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                //vertices4.push_back({ minusyVertexIndices[v], {xc, yc, zc}, texOffset });
                                vertices4.push_back({ xlate * locs[minusyVertexIndices[v]],texcrds[minusyVertexIndices[v]] });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            //vertices4.push_back({ minusyVertexIndices[v], {xc, yc, zc}, texOffset });
                            vertices4.push_back({ xlate * locs[minusyVertexIndices[v]],texcrds[minusyVertexIndices[v]] });
                        }
                    }

                    if (vertices4.size() > numVerts)
                    {
                        // create new actors
                        //this->pStaticBlockArray[idx] = this->mPhysics->createRigidStatic(
                            //physx::PxTransform(xc + 0.5f, yc + 0.5f, zc + 0.5f));
                        //this->pStaticBlockArray[idx]->attachShape(*this->mBlockShape);
                    }
                    else {
                        //this->pStaticBlockArray[idx] = NULL;

                    }
                }
                else {
                    //this->pStaticBlockArray[idx] = NULL;
                }
            }
        }
    }

    vertices4.shrink_to_fit();
}

DWORD WINAPI RenderThread(LPVOID parm)
{
    HWND hwnd = (HWND)parm;
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

    CreateVertexBuffer();
      
    glUseProgram(shaderProg);
     
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * vertices4.size(), vertices4.data(), GL_STATIC_DRAW);

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
    LARGE_INTEGER ct1;

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

        glDrawArrays(GL_TRIANGLES, 0, vertices4.size());

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

    free(pBlockArray);

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
        }
        else if (raw->header.dwType == RIM_TYPEMOUSE)
        {
            elevation -= (raw->data.mouse.lLastY / 20.0f);
            azimuth -= (raw->data.mouse.lLastX / 20.0f);
        }
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