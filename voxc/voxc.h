
#pragma once

#pragma comment(lib, "C:\\VulkanSDK\\1.2.154.1\\Lib\\vulkan-1.lib")
#pragma comment(lib, "C:\\Library\\glfw-3.3.2.bin.WIN64\\lib-vc2019\\glfw3dll.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysX_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXCommon_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXFoundation_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXExtensions_static_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "C:\\Library\\freetype-windows-binaries\\release dll\\win64\\freetype.lib")

#define GETPROC(_t,_v,_vs) {_v = (_t)wglGetProcAddress(_vs);if(_v == NULL){printf("ERR: wglproc %s",_vs);goto cleanup;}}

#define DEG2RAD(d) (d * 3.14159f / 180.0f)

#define FCLAMP(v,i,x) (v < i ? i : (v > x ? x : v))

#define X_GRID_EXTENT 512ll
#define Y_GRID_EXTENT 512ll
#define Z_GRID_EXTENT 256ll

#define GRIDIDX(ix,iy,iz) (((iz) * X_GRID_EXTENT * Y_GRID_EXTENT) + ((iy) * X_GRID_EXTENT) + (ix))

#define TEX_DIRT 0
#define TEX_DIRTGRASS 1
#define TEX_GRASS 2
#define TEX_LEAVES 3

#define SURR_ON_TOP    0x01
#define SURR_PLUS_X    0x02
#define SURR_MINUS_X   0x04
#define SURR_PLUS_Y    0x08
#define SURR_MINUS_Y   0x10
#define SURR_ON_BOTTOM 0x20

#include <Windows.h>
#include <gl/GL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <chrono>
#include <map>
#include <string>

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
#include <ft2build.h>
#include <freetype/freetype.h>

#include "stb_image.h"
#include "glext.h"

typedef struct _VERTEX
{
    glm::vec3 vertex;
    glm::vec2 texc;
    glm::vec3 norm;
} VERTEX1;

typedef struct _VERTEX_BUFFER_GROUP1
{
    GLuint tid;
    GLuint vbo;
    std::vector<VERTEX1> vertices;
} VERTEX_BUFFER_GROUP1;

// [X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT] ;
typedef struct _BLOCK_ENTITY
{
    int8_t type = 0; 
    physx::PxRigidStatic* rigidStatic = NULL;
    uint8_t surround;
} BLOCK_ENTITY, * PBLOCK_ENTITY;

#define TEXTURE_INDEX_TOP 0
#define TEXTURE_INDEX_BOTTOM 1
#define TEXTURE_INDEX_PLUSX 2
#define TEXTURE_INDEX_MINUSX 3
#define TEXTURE_INDEX_PLUSY 4
#define TEXTURE_INDEX_MINUSY 5

#define TEXTURE_IMG_DIRT 0
#define TEXTURE_IMG_DIRTGRASS 1
#define TEXTURE_IMG_GRASS 2
#define TEXTURE_IMG_LEAVES 3

#define REG_AIR 0
#define REG_DIRT 1
#define REG_DIRTGRASS 2
#define REG_TREETRUNK 3
#define REG_TREELEAVES 4

typedef struct _BLOCK_REG
{
    int64_t regType = 0;
    int64_t textureIndex[6] = { 0,0,0,0,0,0 };
} BLOCK_REG, * PBLOCK_REG;

typedef struct _VOXC_WINDOW_CONTEXT
{
    HANDLE hQuitEvent = 0;
    HANDLE hRenderThread = 0;
    BYTE* rawBuffer[48];
    int keys[6] = { 0,0,0,0,0,0 };
    float vz = 0.0f;
    float elevation = 0.0f;
    float azimuth = 0.0f;
    std::vector<BLOCK_ENTITY> blockEntities;
    std::vector<VERTEX_BUFFER_GROUP1> groups;
    float viewportRatio = 0.0f;
    bool isFullscreen = 0;
    int screenWidth = 0;
    int screenHeight = 0;
    GLuint vao = 0;

    physx::PxFoundation* mFoundation = NULL;
    physx::PxPvd* mPvd = NULL;
    physx::PxPhysics* mPhysics = NULL;
    physx::PxDefaultCpuDispatcher* mDispatcher = NULL;
    physx::PxScene* mScene = NULL;
    physx::PxMaterial* pMaterial = NULL;
    physx::PxControllerManager* mManager = NULL;
    physx::PxCapsuleController* mController = NULL; // don't release this
    physx::PxRigidDynamic* mPlayerCapsuleActor = NULL; // released with manager?
    physx::PxShape* mBlockShape = NULL;
    std::vector<physx::PxActor*> blocksAroundMe;
    int64_t xblock = 0;
    int64_t yblock = 0;
    int64_t zblock = 0;
} VOXC_WINDOW_CONTEXT;

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

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
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;
extern PFNGLENABLEVERTEXARRAYATTRIBPROC glEnableVertexArrayAttrib;
extern PFNGLVERTEXARRAYATTRIBFORMATPROC glVertexArrayAttribFormat;
extern PFNGLVERTEXARRAYATTRIBBINDINGPROC glVertexArrayAttribBinding;
extern PFNGLVERTEXARRAYVERTEXBUFFERPROC glVertexArrayVertexBuffer;
extern PFNGLCREATEBUFFERSPROC glCreateBuffers;
extern PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage;
extern PFNGLVERTEXARRAYBINDINGDIVISORPROC glVertexArrayBindingDivisor;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLUNIFORM3FPROC glUniform3f;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;

DWORD WINAPI RenderThread(LPVOID parm);
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL loadExtensionFunctions();
void CreateVertexBuffer(VOXC_WINDOW_CONTEXT*);
void initPhysics(VOXC_WINDOW_CONTEXT* lpctx, glm::vec3 startingPosition);
void cleanupPhysics(VOXC_WINDOW_CONTEXT* lpctx);
GLuint CreateZeroCube();

void block_set_type(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z, int8_t type);
int8_t block_get_type(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z);
int8_t block_get_type(VOXC_WINDOW_CONTEXT* lpctx, int64_t index);
void block_create_new_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int64_t xc, int64_t yc, int64_t zc);
physx::PxRigidStatic* block_get_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index);
void block_release_all_actors(VOXC_WINDOW_CONTEXT* lpctx);

typedef void(*CALLBACK_BLOCKS_FOREACH)(BLOCK_ENTITY*);
void blocks_foreach(VOXC_WINDOW_CONTEXT*, CALLBACK_BLOCKS_FOREACH);
uint8_t block_get_surround_mask(VOXC_WINDOW_CONTEXT* lpctx, int64_t index);

#include "OpenGlProgram.h"
