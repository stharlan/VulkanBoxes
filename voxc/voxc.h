
#pragma once

#pragma comment(lib, "C:\\VulkanSDK\\1.2.154.1\\Lib\\vulkan-1.lib")
#pragma comment(lib, "C:\\Library\\glfw-3.3.2.bin.WIN64\\lib-vc2019\\glfw3dll.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysX_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXCommon_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXFoundation_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXExtensions_static_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXCharacterKinematic_static_64.lib")

#define GETPROC(_t,_v,_vs) {_v = (_t)wglGetProcAddress(_vs);if(_v == NULL){printf("ERR: wglproc %s",_vs);goto cleanup;}}

#define DEG2RAD(d) (d * 3.14159f / 180.0f)

#define FCLAMP(v,i,x) (v < i ? i : (v > x ? x : v))

#define X_GRID_EXTENT 512
#define Y_GRID_EXTENT 512
#define Z_GRID_EXTENT 256

#define GRIDIDX(ix,iy,iz) (((iz) * X_GRID_EXTENT * Y_GRID_EXTENT) + ((iy) * X_GRID_EXTENT) + (ix))

#define TEX_DIRT 0
#define TEX_DIRTGRASS 1
#define TEX_GRASS 2

#include <Windows.h>
#include <gl/GL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <chrono>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PVD_HOST "127.0.0.1"
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include <PxFoundation.h>
#include <PxPhysicsVersion.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <extensions/PxDefaultAllocator.h>
#include <pvd/PxPvd.h>
#include <pvd/PxPvdTransport.h>
#include <characterkinematic/PxControllerManager.h>
#include <characterkinematic/PxCapsuleController.h>

#include "stb_image.h"
#include "glext.h"


typedef struct _VERTEX
{
    glm::vec3 vertex;
    glm::vec2 texc;
} VERTEX;

typedef struct _VERTEX_BUFFER_GROUP1
{
    GLuint tid;
    GLuint vbo;
    std::vector<VERTEX> vertices;
} VERTEX_BUFFER_GROUP1;

typedef struct _VOXC_WINDOW_CONTEXT
{
    HANDLE hQuitEvent = 0;
    HANDLE hRenderThread = 0;
    BYTE* rawBuffer[48];
    float ex = 10.0f;
    float ey = 10.0f;
    float ez = 50.0f;
    float elevation = 0.0f;
    float azimuth = 0.0f;
    int8_t* pBlockArray; // [X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT] ;
    std::vector<VERTEX_BUFFER_GROUP1> groups;
    float viewportRatio = 0.0f;
    bool isFullscreen = 0;
    int screenWidth = 0;
    int screenHeight = 0;
} VOXC_WINDOW_CONTEXT;

extern PFNGLDISPATCHCOMPUTEPROC glDispatchCompute;
extern PFNGLMEMORYBARRIERPROC glMemoryBarrier;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLBINDIMAGETEXTUREPROC glBindImageTexture;
extern PFNGLGETINTEGERI_VPROC glGetIntegeri_v;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM2FVPROC glUniform2fv;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

DWORD WINAPI RenderThread(LPVOID parm);
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL loadExtensionFunctions();
void CreateVertexBuffer(VOXC_WINDOW_CONTEXT*);