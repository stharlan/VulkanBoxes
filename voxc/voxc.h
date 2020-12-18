
#pragma once

#define WGL_NUMBER_PIXEL_FORMATS_ARB            0x2000
#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_DRAW_TO_BITMAP_ARB                  0x2002
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_NEED_PALETTE_ARB                    0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB             0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB              0x2006
#define WGL_SWAP_METHOD_ARB                     0x2007
#define WGL_NUMBER_OVERLAYS_ARB                 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB                0x2009
#define WGL_TRANSPARENT_ARB                     0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB           0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB         0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB          0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB         0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB         0x203B
#define WGL_SHARE_DEPTH_ARB                     0x200C
#define WGL_SHARE_STENCIL_ARB                   0x200D
#define WGL_SHARE_ACCUM_ARB                     0x200E
#define WGL_SUPPORT_GDI_ARB                     0x200F
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_STEREO_ARB                          0x2012
#define WGL_PIXEL_TYPE_ARB                      0x2013
#define WGL_COLOR_BITS_ARB                      0x2014
#define WGL_RED_BITS_ARB                        0x2015
#define WGL_RED_SHIFT_ARB                       0x2016
#define WGL_GREEN_BITS_ARB                      0x2017
#define WGL_GREEN_SHIFT_ARB                     0x2018
#define WGL_BLUE_BITS_ARB                       0x2019
#define WGL_BLUE_SHIFT_ARB                      0x201A
#define WGL_ALPHA_BITS_ARB                      0x201B
#define WGL_ALPHA_SHIFT_ARB                     0x201C
#define WGL_ACCUM_BITS_ARB                      0x201D
#define WGL_ACCUM_RED_BITS_ARB                  0x201E
#define WGL_ACCUM_GREEN_BITS_ARB                0x201F
#define WGL_ACCUM_BLUE_BITS_ARB                 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB                0x2021
#define WGL_DEPTH_BITS_ARB                      0x2022
#define WGL_STENCIL_BITS_ARB                    0x2023
#define WGL_AUX_BUFFERS_ARB                     0x2024

#define WGL_NO_ACCELERATION_ARB                 0x2025
#define WGL_GENERIC_ACCELERATION_ARB            0x2026
#define WGL_FULL_ACCELERATION_ARB               0x2027

#define WGL_SWAP_EXCHANGE_ARB                   0x2028
#define WGL_SWAP_COPY_ARB                       0x2029
#define WGL_SWAP_UNDEFINED_ARB                  0x202A

#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_TYPE_COLORINDEX_ARB                 0x202C

#define WGL_SAMPLE_BUFFERS_ARB               0x2041
#define WGL_SAMPLES_ARB                      0x2042

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126

#if _DEBUG
    #pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\debug\\PhysX_64.lib")
    #pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\debug\\PhysXCommon_64.lib")
    #pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\debug\\PhysXFoundation_64.lib")
    #pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\debug\\PhysXExtensions_static_64.lib")
    #pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\debug\\PhysXPvdSDK_static_64.lib")
    #pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\debug\\PhysXCharacterKinematic_static_64.lib")
    #pragma comment(lib, "C:\\Library\\freetype-windows-binaries\\release dll\\win64\\freetype.lib")
#else
    #pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\release\\PhysX_64.lib")
    #pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\release\\PhysXCommon_64.lib")
    #pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\release\\PhysXFoundation_64.lib")
    #pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\release\\PhysXExtensions_static_64.lib")
    #pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\release\\PhysXPvdSDK_static_64.lib")
    #pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\release\\PhysXCharacterKinematic_static_64.lib")
    #pragma comment(lib, "C:\\Library\\freetype-windows-binaries\\release dll\\win64\\freetype.lib")
#endif

#ifdef _DEBUG
#define HANDLE_GL_ERROR() {GLenum glerr=glGetError();if(glerr!=GL_NO_ERROR) throw new std::runtime_error(fmt::format("opengl error {}",glerr));}
#else
#define HANDLE_GL_ERROR() {}
#endif

#define GETPROC(_t,_v,_vs) {_v = (_t)wglGetProcAddress(_vs);if(_v == NULL){throw new std::runtime_error(fmt::format("error loading proc {}",_vs));}}

#define DEG2RAD(d) (d * 3.14159f / 180.0f)

#define FCLAMP(v,i,x) (v < i ? i : (v > x ? x : v))

// g, x, y and z range from 0 to 255
// g is the grid number
// +-----------+
// |   0 -  15 |
// | ...       |
// | 240 - 255 |
// +-----------+
//#define COMPUTE_BLOCK_ID(g,x,y,z) ((g << 24) + (x << 16) + (y << 8) + z)

#define CLEAR_BIT(t,b) (t &= ~(b))
#define SET_BIT(t,b) (t |= b)
#define TOGL_BIT(t,b) (t ^= b)
#define IS_BITSET(t,b) (t & b)

#define X_GRID_EXTENT 256ll
#define Y_GRID_EXTENT 256ll
#define Z_GRID_EXTENT 256ll

#define GRIDIDX(ix,iy,iz) (((iz) * X_GRID_EXTENT * Y_GRID_EXTENT) + ((iy) * X_GRID_EXTENT) + (ix))
//#define VERTEX_BLOCK_ID(X,Y,Z) (((X / 8) << 10) + ((Y / 8) << 5) + (Z / 8))

#define EXISTS_ON_TOP    0x01
#define EXISTS_PLUS_X    0x02
#define EXISTS_MINUS_X   0x04
#define EXISTS_PLUS_Y    0x08
#define EXISTS_MINUS_Y   0x10
#define EXISTS_ON_BOTTOM 0x20
#define ALPHA_ON_TOP    0x40
#define ALPHA_PLUS_X    0x80
#define ALPHA_MINUS_X   0x100
#define ALPHA_PLUS_Y    0x200
#define ALPHA_MINUS_Y   0x400
#define ALPHA_ON_BOTTOM 0x800
#define FACE_ON_TOP    0x1000
#define FACE_PLUS_X    0x2000
#define FACE_MINUS_X   0x4000
#define FACE_PLUS_Y    0x8000
#define FACE_MINUS_Y   0x10000
#define FACE_ON_BOTTOM 0x20000

#define EXISTS_ALL 0x3F // all the exists bits
#define ALPHA_ALL 0xFC0 // all the alpha bits
#define FACES_ALL 0x3F000 // all the face bits

#include <Windows.h>
#include <Psapi.h>
#include <gl/GL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <chrono>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <future>
#include <fmt/format.h>
#include <iostream>
#include <fstream>
#include <random>

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
#include "tiny_obj_loader.h"
#include "glext.h"

typedef struct _VBO_DATA
{
    GLuint vboId;
    GLsizei numVerts;
    glm::vec4 diffuseColor;
} VBO_DATA;

typedef struct _TEXTURE_SPEC
{
    const char* name;
    BOOL isTransparent;
} TEXTURE_SPEC;

typedef struct _VERTEX
{
    glm::vec3 vertex;
    glm::vec2 texc;
    glm::vec3 norm;
    //glm::u8vec4 blockId;
    //int64_t userData[2];
} VERTEX4;

// each vertex block will handle
// a 8x8x8 block area (origin at 0,0,0)
// each map block will hold 256x256x256 blocks in the world
// so, the vertex block size is 256/8 x 256/8 x 256/8
// or, 32x32x32
// that's a 5-bit number
// the ID is composed of the x/y/z of that 32x32x32 grid
// x << 10 bits
// y << 5 bits
// z << 0 bits
// that fits into a 16 bit number
//typedef struct _VERTEX_BLOCK
//{
//    std::vector<VERTEX3> vertices;
//    uint16_t gridLocationId;
//    GLuint vbo;
//    int64_t vsize;
//} VERTEX_BLOCK;

//typedef struct _VERTEX_BUFFER_GROUP1
//{
//    GLuint tid;
//    std::map<uint16_t, VERTEX_BLOCK> vertexBlocks;
//    //GLuint vbo;
//    //std::vector<VERTEX2> vertices;
//    //int64_t vsize;
//} VERTEX_BUFFER_GROUP1;

// [X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT] ;
typedef struct _BLOCK_ENTITY
{
    int8_t regType = 0; 
    physx::PxRigidStatic* rigidStatic = NULL;
    uint16_t flags;
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
#define TEXTURE_IMG_WOODBARK 4
#define TEXTURE_IMG_WOODRINGS 5

#define REG_AIR 0
#define REG_DIRT 1
#define REG_DIRTGRASS 2
#define REG_TREETRUNK 3
#define REG_TREELEAVES 4

typedef struct _BLOCK_REG
{
    uint8_t regType = 0;
    uint32_t textureIndex[6] = { 0,0,0,0,0,0 };
    BOOL isTransparent = FALSE;
} BLOCK_REG, * PBLOCK_REG;

//typedef struct _VERTEX_BUFFER_CHUNK
//{
//    GLuint texture_id = 0;
//    int64_t texture_const = 0;
//    int64_t boffset = 0;
//    int64_t blength = 0;
//    int64_t bfree = 0;
//    GLsizei num_vertices = 0;
//} VERTEX_BUFFER_CHUNK;
//
//typedef struct _VERTEX_BUFFER
//{
//    GLuint vbo = 0;
//    BYTE* mem = nullptr;
//    int64_t mem_size = 0;
//    std::vector<VERTEX_BUFFER_CHUNK> chunks;
//} VERTEX_BUFFER;

typedef struct _VBLOCK_SUBVERT
{
    GLuint tex_id = 0;
    uint32_t num_vertices = 0;
    glm::vec2 centroid;
} VBLOCK_SUBVERT;

typedef struct _VBLOCK_16 {
    GLuint vbo = 0;
    std::vector<VBLOCK_SUBVERT> subverts;
} VBLOCK_16;

typedef std::pair<uint32_t, BLOCK_ENTITY> BLOCK_MAP_ENTRY;

typedef struct _VOXC_WINDOW_CONTEXT
{
    HGLRC hglrc;
    HGLRC hglrcAlt;
    HANDLE hQuitEvent = 0;
    HANDLE hRenderThread = 0;
    BYTE* rawBuffer[48];
    int keys[8] = { 0,0,0,0,0,0,0,0 };
    float vz = 0.0f;
    float elevation = 0.0f;
    float azimuth = 0.0f;
    //BLOCK_ENTITY* lpBlockEntities = NULL;
    std::unordered_map<uint32_t,BLOCK_ENTITY> blockVector;
    //int64_t numEntities = 0;
    //std::vector<VERTEX_BUFFER_GROUP1> groups;
    //std::vector<VERTEX_BUFFER> vertex_buffers;
    std::map<int, int> tex_const_id_map;
    std::vector<VBLOCK_16> drawVector;
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

    HANDLE RebuildVerticesThread = NULL;
} VOXC_WINDOW_CONTEXT;

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

typedef HGLRC (*wglCreateContextAttribsARBFN)(HDC, HGLRC, const int*);
typedef BOOL (*wglChoosePixelFormatARBFN)(HDC , const int*, const FLOAT*, UINT, int*, UINT*);

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
extern PFNGLNAMEDBUFFERSUBDATAPROC glNamedBufferSubData;
extern wglCreateContextAttribsARBFN wglCreateContextAttribsARB;
extern wglChoosePixelFormatARBFN wglChoosePixelFormatARB;

DWORD WINAPI RenderThread(LPVOID parm);
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void loadExtensionFunctions();
void update_surrounding_blocks(VOXC_WINDOW_CONTEXT* lpctx, int64_t xc, int64_t yc, int64_t zc); 
void initPhysics(VOXC_WINDOW_CONTEXT* lpctx, glm::vec3 startingPosition);
void cleanupPhysics(VOXC_WINDOW_CONTEXT* lpctx);
void getZeroCubeVertices(std::vector<VERTEX4>& zeroCubeVerts);

//void block_set_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z, int8_t type);
//void block_set_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int8_t type);
//int8_t block_get_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z,bool);
//int8_t block_get_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t index,bool);
//void block_create_new_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int64_t xc, int64_t yc, int64_t zc);
//physx::PxRigidStatic* block_get_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index);
//void block_release_all_actors(VOXC_WINDOW_CONTEXT* lpctx);

//void block_allocate(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z);
//void block_cleanup(VOXC_WINDOW_CONTEXT* lpctx);
//void block_set_hash_code(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int64_t hashCode);
//void block_release_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index);
//uint64_t block_get_flags(VOXC_WINDOW_CONTEXT* lpctx, uint64_t index);
//void block_set_flags(VOXC_WINDOW_CONTEXT* lpctx, uint64_t index, uint16_t value);

void block_set_regtype(VOXC_WINDOW_CONTEXT* lpctx, uint32_t x, uint32_t y, uint32_t z, uint8_t type);
uint8_t block_get_regtype(VOXC_WINDOW_CONTEXT* lpctx, uint32_t x, uint32_t y, uint32_t z);
uint8_t block_get_regtype(VOXC_WINDOW_CONTEXT* lpctx, uint32_t blockId);
void block_create_new_actor(VOXC_WINDOW_CONTEXT* lpctx,
    uint32_t x, uint32_t y, uint32_t z,
    float xc, float yc, float zc);
void block_release_actor(VOXC_WINDOW_CONTEXT* lpctx, uint32_t x, uint32_t y, uint32_t z);
void block_release_all_actors(VOXC_WINDOW_CONTEXT* lpctx);
uint16_t block_get_flags(VOXC_WINDOW_CONTEXT* lpctx, uint32_t x, uint32_t y, uint32_t z);
void block_set_flags(VOXC_WINDOW_CONTEXT* lpctx, uint32_t x, uint32_t y, uint32_t z, uint16_t flags);
physx::PxRigidStatic* block_get_actor(VOXC_WINDOW_CONTEXT* lpctx, uint32_t x, uint32_t y, uint32_t z);
void block_set_flags(VOXC_WINDOW_CONTEXT* lpctx, uint32_t blockId, uint16_t flags);
uint16_t block_get_flags(VOXC_WINDOW_CONTEXT* lpctx, uint32_t blockId);
uint32_t block_compute_block_id(uint32_t x, uint32_t y, uint32_t z);
glm::u32vec3 block_compute_position(uint32_t blockId);

void generate_terrain(BYTE* pixels);

#include "OpenGlProgram.h"
#include "OpenGlVertexBuffer.h"

typedef struct _MESSAGE_CONTEXT
{
    VOXC_WINDOW_CONTEXT* lpctx = nullptr;
    OpenGlProgram* prog = nullptr;
    GLuint fontVAO = 0;
    GLuint fontVBO = 0;
    std::map<char, Character>* Characters = nullptr;
    HDC hdc = 0;
} MESSAGE_CONTEXT;

typedef void (*RENDER_MSG_ON_SCREEN_FNPTR)(MESSAGE_CONTEXT* mctx, const char* msg);

void create_vertex_buffer(VOXC_WINDOW_CONTEXT* lpctx, RENDER_MSG_ON_SCREEN_FNPTR msgfn, MESSAGE_CONTEXT* mctx);

typedef struct _RENDER_LOOP_CONTEXT
{
    HDC hdc;
    glm::mat4& lightProjection;
    GLsizei shadowWidth;
    GLsizei shadowHeight;
    GLuint depthMapFBO;
    OpenGlProgram& shadowProg;
    glm::mat4& modelMatrix;
    std::vector<VBO_DATA>& modelVboData;
    OpenGlProgram& voxcProgram;
    GLuint depthMap;
    OpenGlProgram& selCubeProg;
    OpenGlVertexBuffer<VERTEX4>& zeroCubeBuffer;
    OpenGlProgram& fontProg;
    GLuint fontVAO;
    GLuint fontVBO;
    std::map<char, Character>& Characters;
    OpenGlProgram& ddProg;
    OpenGlVertexBuffer<VERTEX4>& quadBuffer;
} RENDER_LOOP_CONTEXT;

//void vmm_allocate_buffer(VOXC_WINDOW_CONTEXT* lpctx, std::vector<GLuint> textureIds);
//int vmm_add_vertex(VOXC_WINDOW_CONTEXT* lpctx, int64_t textureConst, VERTEX4* vertex);
//void vmm_allocate_single_buffer(VOXC_WINDOW_CONTEXT* lpctx, GLuint textureId, int64_t texture_const);
