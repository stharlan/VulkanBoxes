
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

#include <Windows.h>
#include <gl/GL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

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

PFNGLDISPATCHCOMPUTEPROC glDispatchCompute = NULL;
PFNGLMEMORYBARRIERPROC glMemoryBarrier = NULL;
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLBINDIMAGETEXTUREPROC glBindImageTexture = NULL;
PFNGLGETINTEGERI_VPROC glGetIntegeri_v = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLUNIFORM4FVPROC glUniform4fv = NULL;
PFNGLUNIFORM3FVPROC glUniform3fv = NULL;
PFNGLUNIFORM2FVPROC glUniform2fv = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = NULL;