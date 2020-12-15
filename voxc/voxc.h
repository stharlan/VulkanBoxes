
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

#define HANDLE_GL_ERROR() {GLenum glerr=glGetError();if(glerr!=GL_NO_ERROR) throw new std::runtime_error(fmt::format("opengl error {}",glerr));}

#define GETPROC(_t,_v,_vs) {_v = (_t)wglGetProcAddress(_vs);if(_v == NULL){printf("ERR: wglproc %s",_vs);goto cleanup;}}

#define DEG2RAD(d) (d * 3.14159f / 180.0f)

#define FCLAMP(v,i,x) (v < i ? i : (v > x ? x : v))

#define CLEAR_BIT(t,b) (t &= ~(b))
#define SET_BIT(t,b) (t |= b)
#define TOGL_BIT(t,b) (t ^= b)
#define IS_BITSET(t,b) (t & b)

#define X_GRID_EXTENT 512ll
#define Y_GRID_EXTENT 512ll
#define Z_GRID_EXTENT 256ll

#define GRIDIDX(ix,iy,iz) (((iz) * X_GRID_EXTENT * Y_GRID_EXTENT) + ((iy) * X_GRID_EXTENT) + (ix))

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
#include <algorithm>
#include <future>
#include <fmt/format.h>

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
    int64_t userData[2];
} VERTEX2;

typedef struct _VERTEX_BUFFER_GROUP1
{
    GLuint tid;
    GLuint vbo;
    std::vector<VERTEX2> vertices;
    int64_t vsize;
} VERTEX_BUFFER_GROUP1;

// [X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT] ;
typedef struct _BLOCK_ENTITY
{
    glm::uvec3 gridLocation;
    int8_t regType = 0; 
    physx::PxRigidStatic* rigidStatic = NULL;
    uint8_t surroundExistsMask = 0; // block exists on side
    uint8_t surroundAlphaMask = 0; // block is transparenton side (by default, exists will also be true if this is true)
    uint8_t faceMask = 0; // face on this side of this block
    int64_t hashCode = 0;
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
    int64_t regType = 0;
    int64_t textureIndex[6] = { 0,0,0,0,0,0 };
    BOOL isTransparent = FALSE;
} BLOCK_REG, * PBLOCK_REG;

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
    BLOCK_ENTITY* lpBlockEntities = NULL;
    int64_t numEntities = 0;
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
//extern PFNGLGENBUFFERSPROC glGenBuffers;
//extern PFNGLBINDBUFFERPROC glBindBuffer;
//extern PFNGLBUFFERDATAPROC glBufferData;
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
BOOL loadExtensionFunctions();
void CreateVertexBuffer(VOXC_WINDOW_CONTEXT*);
void update_surrounding_blocks(VOXC_WINDOW_CONTEXT* lpctx, int64_t xc, int64_t yc, int64_t zc); 
void initPhysics(VOXC_WINDOW_CONTEXT* lpctx, glm::vec3 startingPosition);
void cleanupPhysics(VOXC_WINDOW_CONTEXT* lpctx);
void getZeroCubeVertices(std::vector<VERTEX2>& zeroCubeVerts);

void block_set_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z, int8_t type);
void block_set_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int8_t type);
int8_t block_get_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z,bool);
int8_t block_get_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t index,bool);
void block_create_new_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int64_t xc, int64_t yc, int64_t zc);
physx::PxRigidStatic* block_get_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index);
void block_release_all_actors(VOXC_WINDOW_CONTEXT* lpctx);

void block_allocate(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z);
void block_cleanup(VOXC_WINDOW_CONTEXT* lpctx);
uint8_t block_get_surround_exists_mask(VOXC_WINDOW_CONTEXT* lpctx, int64_t index);
uint8_t block_get_surround_alpha_mask(VOXC_WINDOW_CONTEXT* lpctx, int64_t index);
void block_set_face_mask(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, uint8_t value);
void block_set_hash_code(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int64_t hashCode);
uint8_t block_get_surround_face_mask(VOXC_WINDOW_CONTEXT* lpctx, int64_t index);
void block_set_surround_exists_mask(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, uint8_t value);
void block_set_surround_alpha_mask(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, uint8_t value);
void block_set_surround_face_mask(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, uint8_t value);
void block_release_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index);

#include "OpenGlProgram.h"
#include "OpenGlVertexBuffer.h"

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
    OpenGlVertexBuffer<VERTEX2>& zeroCubeBuffer;
    OpenGlProgram& fontProg;
    GLuint fontVAO;
    GLuint fontVBO;
    std::map<char, Character>& Characters;
    OpenGlProgram& ddProg;
    OpenGlVertexBuffer<VERTEX2>& quadBuffer;
} RENDER_LOOP_CONTEXT;


//class VertexFindByHash : public std::unary_function<VERTEX2, bool> {
//    int64_t hashcode;
//public:
//    explicit VertexFindByHash(const int64_t value) : hashcode(value) {}
//    bool operator() (const VERTEX2& item) const { return item.userData[0] == hashcode; }
//};
//
//class BlockEntityByHash : public std::unary_function<BLOCK_ENTITY, bool> {
//    int64_t hashcode;
//public:
//    explicit BlockEntityByHash(const int64_t value) : hashcode(value) {}
//    bool operator() (const BLOCK_ENTITY& item) const { return item.hashCode == hashcode; }
//};
//
//class BlockRegFindById : public std::unary_function<BLOCK_REG, bool> {
//    int64_t regtype;
//public:
//    explicit BlockRegFindById(const int64_t type) : regtype(type) {}
//    bool operator() (const BLOCK_REG& item) const { return item.regType == regtype; }
//};
