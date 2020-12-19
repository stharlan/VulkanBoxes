
#include "voxc.h"

std::vector<BLOCK_REG> vBlockRegistry = {
    { REG_DIRT, {TEXTURE_IMG_DIRT,TEXTURE_IMG_DIRT,TEXTURE_IMG_DIRT,TEXTURE_IMG_DIRT,TEXTURE_IMG_DIRT,TEXTURE_IMG_DIRT}, FALSE},
    { REG_DIRTGRASS, {TEXTURE_IMG_GRASS, TEXTURE_IMG_DIRT, TEXTURE_IMG_DIRTGRASS,TEXTURE_IMG_DIRTGRASS,TEXTURE_IMG_DIRTGRASS,TEXTURE_IMG_DIRTGRASS}, FALSE},
    { REG_TREETRUNK, {TEXTURE_IMG_WOODRINGS,TEXTURE_IMG_WOODRINGS,TEXTURE_IMG_WOODBARK,TEXTURE_IMG_WOODBARK,TEXTURE_IMG_WOODBARK,TEXTURE_IMG_WOODBARK}, FALSE},
    { REG_TREELEAVES, {TEXTURE_IMG_LEAVES,TEXTURE_IMG_LEAVES,TEXTURE_IMG_LEAVES,TEXTURE_IMG_LEAVES,TEXTURE_IMG_LEAVES,TEXTURE_IMG_LEAVES}, TRUE}
};

const GLuint topVertexIndices[6] = { 0, 1, 2, 2, 3, 0 };
const GLuint plusxVertexIndices[6] = { 4, 5, 6, 6, 7, 4 };
const GLuint minusxVertexIndices[6] = { 8, 9, 10, 10, 11, 8 };
const GLuint plusyVertexIndices[6] = { 12, 13, 14, 14, 15, 12 };
const GLuint minusyVertexIndices[6] = { 16, 17, 18, 18, 19, 16 };
const GLuint bottomVertexIndices[6] = { 20, 21, 22, 22, 23, 20 };

const uint8_t TreeLeaves[] = {
    0,0,0,0,0,0,0,
    0,0,1,1,1,0,0,
    0,1,0,0,0,1,0,
    0,1,0,0,0,1,0,
    0,1,0,0,0,1,0,
    0,0,1,1,1,0,0,
    0,0,0,0,0,0,0,

    0,1,1,1,1,1,0,
    1,0,0,0,0,0,1,
    1,0,1,1,1,0,1,
    1,0,1,1,1,0,1,
    1,0,1,1,1,0,1,
    1,0,0,0,0,0,1,
    0,1,1,1,1,1,0,

    0,1,1,1,1,1,0,
    1,0,0,0,0,0,1,
    1,0,0,0,0,0,1,
    1,0,0,0,0,0,1,
    1,0,0,0,0,0,1,
    1,0,0,0,0,0,1,
    0,1,1,1,1,1,0,

    0,1,1,1,1,1,0,
    1,0,0,0,0,0,1,
    1,0,0,0,0,0,1,
    1,0,0,0,0,0,1,
    1,0,0,0,0,0,1,
    1,0,0,0,0,0,1,
    0,1,1,1,1,1,0,

    0,0,0,0,0,0,0,
    0,0,1,1,1,0,0,
    0,1,0,0,0,1,0,
    0,1,0,0,0,1,0,
    0,1,0,0,0,1,0,
    0,0,1,1,1,0,0,
    0,0,0,0,0,0,0,

    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,
    0,0,1,1,1,0,0,
    0,0,1,1,1,0,0,
    0,0,1,1,1,0,0,
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0
};

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

//const glm::vec4 texcrds[40] = {
//
//    {0.125f, 0.0f,0,0},
//    {0.249f, 0.0f,0,0},
//    {0.249f, 0.124f,0,0},
//    {0.125f, 0.124f,0,0},
//
//    {0.25f, 0.0f,0,0},
//    {0.374f, 0.0f,0,0},
//    {0.374f, 0.124f,0,0},
//    {0.25f, 0.124f,0,0},
//
//    {0.0f, 0.0f,0,0},
//    {0.124f, 0.0f,0,0},
//    {0.124f, 0.124f,0,0},
//    {0.0f, 0.124f,0,0},
//
//    {0.625f, 0.0f,0,0},
//    {0.749f, 0.0f,0,0},
//    {0.749f, 0.124f,0,0},
//    {0.625f, 0.124f,0,0},
//
//    {0.5f, 0.0f,0,0},
//    {0.624f, 0.0f,0,0},
//    {0.624f, 0.124f,0,0},
//    {0.5f, 0.124f,0,0},
//
//    {0.375f, 0.0f,0,0},
//    {0.499f, 0.0f,0,0},
//    {0.499f, 0.124f,0,0},
//    {0.375f, 0.124f,0,0},
//
//    {0.75f, 0.124f,0,0},
//    {0.874f, 0.124f,0,0},
//    {0.874f, 0.0f,0,0},
//    {0.75f, 0.0f,0,0},
//
//    {0.75f, 0.124f,0,0},
//    {0.75f, 0.0f,0,0},
//    {0.874f, 0.0f,0,0},
//    {0.874f, 0.124f,0,0},
//
//    {0.75f, 0.124f,0,0},
//    {0.874f, 0.124f,0,0},
//    {0.874f, 0.0f,0,0},
//    {0.75f, 0.0f,0,0},
//
//    {0.75f, 0.124f,0,0},
//    {0.75f, 0.0f,0,0},
//    {0.874f, 0.0f,0,0},
//    {0.874f, 0.124f,0,0}
//};

const glm::vec2 texcrds[40] = {

    {0,0},{1,0},{1,1},{0,1},
    {0,1},{1,1},{1,0},{0,0},
    {0,1},{0,0},{1,0},{1,1},
    {0,1},{0,0},{1,0},{1,1},
    {0,1},{1,1},{1,0},{0,0},
    {0,0},{1,0},{1,1},{0,1},
    {0,0},{1,0},{1,1},{0,1},
    {0,0},{1,0},{1,1},{0,1},
    {0,0},{1,0},{1,1},{0,1},
    {0,0},{1,0},{1,1},{0,1}

    //{0.125f, 0.0f,0,0},
    //{0.249f, 0.0f,0,0},
    //{0.249f, 0.124f,0,0},
    //{0.125f, 0.124f,0,0},

    //{0.25f, 0.0f,0,0},
    //{0.374f, 0.0f,0,0},
    //{0.374f, 0.124f,0,0},
    //{0.25f, 0.124f,0,0},

    //{0.0f, 0.0f,0,0},
    //{0.124f, 0.0f,0,0},
    //{0.124f, 0.124f,0,0},
    //{0.0f, 0.124f,0,0},

    //{0.625f, 0.0f,0,0},
    //{0.749f, 0.0f,0,0},
    //{0.749f, 0.124f,0,0},
    //{0.625f, 0.124f,0,0},

    //{0.5f, 0.0f,0,0},
    //{0.624f, 0.0f,0,0},
    //{0.624f, 0.124f,0,0},
    //{0.5f, 0.124f,0,0},

    //{0.375f, 0.0f,0,0},
    //{0.499f, 0.0f,0,0},
    //{0.499f, 0.124f,0,0},
    //{0.375f, 0.124f,0,0},

    //{0.75f, 0.124f,0,0},
    //{0.874f, 0.124f,0,0},
    //{0.874f, 0.0f,0,0},
    //{0.75f, 0.0f,0,0},

    //{0.75f, 0.124f,0,0},
    //{0.75f, 0.0f,0,0},
    //{0.874f, 0.0f,0,0},
    //{0.874f, 0.124f,0,0},

    //{0.75f, 0.124f,0,0},
    //{0.874f, 0.124f,0,0},
    //{0.874f, 0.0f,0,0},
    //{0.75f, 0.0f,0,0},

    //{0.75f, 0.124f,0,0},
    //{0.75f, 0.0f,0,0},
    //{0.874f, 0.0f,0,0},
    //{0.874f, 0.124f,0,0}
};

// normals for cube sides
const glm::vec3 normals[40] = {
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
    {0.0f, 0.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
    {1.0f, -1.0f, 0.0f},
    {1.0f, -1.0f, 0.0f},
    {1.0f, -1.0f, 0.0f},
    {1.0f, -1.0f, 0.0f},
    {-1.0f, 1.0f, 0.0f},
    {-1.0f, 1.0f, 0.0f},
    {-1.0f, 1.0f, 0.0f},
    {-1.0f, 1.0f, 0.0f},

    {1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},

    {-1.0f, -1.0f, 0.0f},
    {-1.0f, -1.0f, 0.0f},
    {-1.0f, -1.0f, 0.0f},
    {-1.0f, -1.0f, 0.0f}
};

//GLuint CreateZeroCube()
//{
//    glm::mat4 xz = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, -0.5f));
//    glm::mat4 sc = glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 1.1f, 1.1f));
//    std::vector<VERTEX2> zeroCubeVerts;
//    for (int i = 0; i < 6; i++) 
//        zeroCubeVerts.push_back({ sc * (xz * locs[topVertexIndices[i]]), texcrds[topVertexIndices[i]], normals[topVertexIndices[i]], {0,0} });
//    for (int i = 0; i < 6; i++)
//        zeroCubeVerts.push_back({ sc * (xz * locs[bottomVertexIndices[i]]), texcrds[bottomVertexIndices[i]], normals[bottomVertexIndices[i]], {0,0} });
//    for (int i = 0; i < 6; i++)
//        zeroCubeVerts.push_back({ sc * (xz * locs[plusxVertexIndices[i]]), texcrds[plusxVertexIndices[i]], normals[plusxVertexIndices[i]], {0,0} });
//    for (int i = 0; i < 6; i++)
//        zeroCubeVerts.push_back({ sc * (xz * locs[minusxVertexIndices[i]]), texcrds[minusxVertexIndices[i]], normals[minusxVertexIndices[i]], {0,0} });
//    for (int i = 0; i < 6; i++)
//        zeroCubeVerts.push_back({ sc * (xz * locs[plusyVertexIndices[i]]), texcrds[plusyVertexIndices[i]], normals[plusyVertexIndices[i]], {0,0} });
//    for (int i = 0; i < 6; i++)
//        zeroCubeVerts.push_back({ sc * (xz * locs[minusyVertexIndices[i]]), texcrds[minusyVertexIndices[i]], normals[minusyVertexIndices[i]], {0,0} });
//    GLuint zeroCubeId = 0;
//    glCreateBuffers(1, &zeroCubeId);
//    glNamedBufferStorage(zeroCubeId, sizeof(VERTEX2) * 36, zeroCubeVerts.data(), 0);
//    return zeroCubeId;
//}

void getZeroCubeVertices(std::vector<VERTEX4>& zeroCubeVerts)
{
    glm::mat4 xz = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, -0.5f));
    glm::mat4 sc = glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 1.1f, 1.1f));
    //std::vector<VERTEX2> zeroCubeVerts;
    for (int i = 0; i < 6; i++) 
        zeroCubeVerts.push_back({ sc * (xz * locs[topVertexIndices[i]]), texcrds[topVertexIndices[i]], normals[topVertexIndices[i]] });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[bottomVertexIndices[i]]), texcrds[bottomVertexIndices[i]], normals[bottomVertexIndices[i]] });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[plusxVertexIndices[i]]), texcrds[plusxVertexIndices[i]], normals[plusxVertexIndices[i]] });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[minusxVertexIndices[i]]), texcrds[minusxVertexIndices[i]], normals[minusxVertexIndices[i]] });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[plusyVertexIndices[i]]), texcrds[plusyVertexIndices[i]], normals[plusyVertexIndices[i]] });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[minusyVertexIndices[i]]), texcrds[minusyVertexIndices[i]], normals[minusyVertexIndices[i]] });
    //GLuint zeroCubeId = 0;
    //glCreateBuffers(1, &zeroCubeId);
    //glNamedBufferStorage(zeroCubeId, sizeof(VERTEX2) * 36, zeroCubeVerts.data(), 0);
    //return zeroCubeId;
}

// update the exists and alpha mask for a block adjacent to another block
// target side is an exists flag
void update_mask_surround(VOXC_WINDOW_CONTEXT * lpctx, uint32_t srcx, uint32_t srcy, uint32_t srcz, 
    uint32_t ix, uint32_t iy, uint32_t iz,
    uint16_t existsFlag)
{
    uint16_t alphaFlag = existsFlag << 6ll;
    uint8_t srcBlockType = block_get_regtype(lpctx, srcx, srcy, srcz);
    //int8_t existsMaskToUpdate = block_get_surround_exists_mask(lpctx, indexToUpdate);
    //int8_t alphaMaskToUpdate = block_get_surround_alpha_mask(lpctx, indexToUpdate);
    uint16_t flags = block_get_flags(lpctx, ix, iy, iz);
    if (srcBlockType) {
        SET_BIT(flags, existsFlag);
        std::vector<BLOCK_REG>::iterator ifb = std::find_if(
            vBlockRegistry.begin(),
            vBlockRegistry.end(),
            [srcBlockType](const BLOCK_REG& item) { return item.regType == srcBlockType; });
        if (ifb != vBlockRegistry.end()) {
            if (TRUE == ifb->isTransparent) {
                SET_BIT(flags, alphaFlag);
            }
            else {
                CLEAR_BIT(flags, alphaFlag);
            }
        }
    }
    else {
        CLEAR_BIT(flags, existsFlag);
        CLEAR_BIT(flags, alphaFlag);
    }
    //block_set_surround_exists_mask(lpctx, indexToUpdate, existsMaskToUpdate);
    //block_set_surround_alpha_mask(lpctx, indexToUpdate, alphaMaskToUpdate);
    block_set_flags(lpctx, ix, iy, iz, flags);
}

// update exists and alpha mask for all blocks surrounding a block
// TODO update this to allow specifying only one side to update (optimization)
// THE SIDE TO UPDATE FLAG IS AN EXISTS FLAG
void update_masks(VOXC_WINDOW_CONTEXT* lpctx, uint32_t xc, uint32_t yc, uint32_t zc,
    uint16_t sideToUpdate)
{

    //if (sideToUpdate > 0) printf("masks: updating face %i only\n", sideToUpdate);

    //if (sideToUpdate > 0) {
        //uint8_t blockType = block_get_regtype(lpctx, xc, yc, zc);
        //printf("masks: %i %i %i block type is %i\n", xc, yc, zc, blockType);
    //}

    //int64_t idx = GRIDIDX(xc, yc, zc);
    if(sideToUpdate == 0 || sideToUpdate == EXISTS_ON_TOP) 
        if (zc < (Z_GRID_EXTENT - 1)) update_mask_surround(lpctx, xc, yc, zc + 1, xc, yc, zc, EXISTS_ON_TOP);
    if (sideToUpdate == 0 || sideToUpdate == EXISTS_ON_BOTTOM)
        if (zc > 0) update_mask_surround(lpctx, xc, yc, zc - 1, xc, yc, zc, EXISTS_ON_BOTTOM);
    if (sideToUpdate == 0 || sideToUpdate == EXISTS_PLUS_X)
        if (xc < (X_GRID_EXTENT - 1)) update_mask_surround(lpctx, xc + 1, yc, zc, xc, yc, zc, EXISTS_PLUS_X);
    if (sideToUpdate == 0 || sideToUpdate == EXISTS_MINUS_X)
        if (xc > 0) update_mask_surround(lpctx, xc - 1, yc, zc, xc, yc, zc, EXISTS_MINUS_X);
    if (sideToUpdate == 0 || sideToUpdate == EXISTS_PLUS_Y)
        if (yc < (Y_GRID_EXTENT - 1)) update_mask_surround(lpctx, xc, yc + 1, zc, xc, yc, zc, EXISTS_PLUS_Y);
    if (sideToUpdate == 0 || sideToUpdate == EXISTS_MINUS_Y)
        if (yc > 0) update_mask_surround(lpctx, xc, yc - 1, zc, xc, yc, zc, EXISTS_MINUS_Y);
}

void add_vertex(VOXC_WINDOW_CONTEXT* lpctx, BLOCK_REG& fndBlock, uint32_t textureIndexConst, 
    glm::mat4& xlate, const GLuint vertexIndices[],
    std::map <GLuint, std::vector<VERTEX4>>& vertices)
    //(VOXC_WINDOW_CONTEXT* lpctx, BLOCK_REG& fndBlock,
    //glm::mat4& xlate, int64_t idx, uint32_t textureIndexConst, const GLuint vertexIndices[],
    //std::vector<VERTEX4>& vertices)
{
    for (int64_t v = 0; v < 6; v++) {
        // TODO find out what vertex block this belongs to
        // vertex blocks will be 8x8x8 with the origin at zero
        // so block position will be x / 8, y / 8, z / 8
        //int64_t textureConst = fndBlock.textureIndex[textureIndexConst];

        VERTEX4 vtx;
        //vtx.blockId = blockId;
        vtx.vertex = glm::vec3(xlate * locs[vertexIndices[v]]);
        vtx.norm = normals[vertexIndices[v]];
        vtx.texc = texcrds[vertexIndices[v]];

        GLuint texId = lpctx->tex_const_id_map.at(fndBlock.textureIndex[textureIndexConst]);

        std::map<GLuint, std::vector<VERTEX4>>::iterator viter = vertices.find(texId);
        if (viter != vertices.end())
        {
            // found it
            viter->second.push_back(vtx);
        }
        else {
            // insert it
            std::vector<VERTEX4> v = {vtx};
            vertices.insert(std::pair < GLuint, std::vector<VERTEX4>>(texId, v));
        }

        //vertices.push_back(vtx);

        //int result = vmm_add_vertex(lpctx, textureConst, &vtx);

        //if (result == 1)
        //{
        //    // need new allocation
        //    uint32_t numBuffers = lpctx->vertex_buffers.size();
        //    for (int i = 0; i < numBuffers; i++)
        //    {
        //        std::vector<VERTEX_BUFFER_CHUNK>::iterator vbchnk = std::find_if(
        //            lpctx->vertex_buffers[i].chunks.begin(),
        //            lpctx->vertex_buffers[i].chunks.end(),
        //            [textureConst](const VERTEX_BUFFER_CHUNK& vbufch) {
        //                return (vbufch.texture_const == textureConst);
        //            }
        //        );
        //        if (vbchnk != lpctx->vertex_buffers[i].chunks.end())
        //        {
        //            vmm_allocate_single_buffer(lpctx, vbchnk->texture_id, vbchnk->texture_const);
        //            result = vmm_add_vertex(lpctx, textureConst, &vtx);
        //            if (result != 0) throw std::runtime_error("failed to aded vertex after adding new chunk");
        //            i = numBuffers; // quit for
        //        }
        //    }
        //    if (result == 1)
        //    {
        //        // can't find chunk with texture id
        //        // shouldn't happen
        //        throw new std::runtime_error("failed to find chunk with tex id");
        //    }
        //}

        //VERTEX_BUFFER_GROUP1& group = lpctx->groups[textureIndex];
        //std::map<uint16_t, VERTEX_BLOCK>::iterator vBlock = group.vertexBlocks.find(gridLocationId);
        //if (vBlock == group.vertexBlocks.end())
        //{
        //    std::vector<VERTEX2> vertices;
        //    VERTEX_BLOCK newvBlock;
        //    newvBlock.gridLocationId = gridLocationId;
        //    newvBlock.vbo = 0;
        //    newvBlock.vsize = 0;
        //    newvBlock.vertices.push_back({
        //        glm::vec3(xlate * locs[vertexIndices[v]]),
        //        texcrds[vertexIndices[v]],normals[vertexIndices[v]], {idx,0}
        //        });
        //    group.vertexBlocks.insert(std::pair<int16_t, VERTEX_BLOCK>(gridLocationId, newvBlock));
        //}
        //else {
        //    vBlock->second.vertices.push_back({
        //        glm::vec3(xlate * locs[vertexIndices[v]]),
        //        texcrds[vertexIndices[v]],normals[vertexIndices[v]], {idx,0}
        //        });
        //}

        //group.vertices.push_back({
        //    glm::vec3(xlate * locs[bottomVertexIndices[v]]),
        //    texcrds[bottomVertexIndices[v]],normals[bottomVertexIndices[v]], {idx,0}
        //    });
    }

}

// THE SIDE TO UPDATE FLAG IS AN EXISTS FLAG
int update_faces(VOXC_WINDOW_CONTEXT* lpctx,
    uint32_t xc, uint32_t yc, uint32_t zc,
    uint8_t sideToUpdate,
    std::map <GLuint, std::vector<VERTEX4>>& vertices)
{

    //int64_t idx = GRIDIDX(xc, yc, zc);
    //int16_t gridLocationId = VERTEX_BLOCK_ID(xc, yc, zc);
    //glm::u8vec4 blockId = glm::u8vec4(0, xc, yc, zc);

    uint8_t blockType = block_get_regtype(lpctx, xc, yc, zc);

    //if (sideToUpdate > 0) {
    //    printf("faces: %i %i %i %i block type is %i\n", idx, xc, yc, zc, blockType);
    //}

    int facesAdded = 0;

    if (blockType)
    {

        //if (sideToUpdate > 0) printf("faces: updating face %i only\n", sideToUpdate);

        // create a transform
        glm::vec3 translateVector = glm::vec3(xc, yc, zc);
        glm::mat4 xlate = glm::translate(glm::mat4(1.0f), translateVector);

        // get the existing masks
        uint16_t flags = block_get_flags(lpctx, xc, yc, zc);

        // find the registry information for block type
        std::vector<BLOCK_REG>::iterator ifoundBlock = std::find_if(
            vBlockRegistry.begin(),
            vBlockRegistry.end(),
            [blockType](const BLOCK_REG& item) { return item.regType == blockType; });
        if (ifoundBlock == vBlockRegistry.end())
        {
            printf("block reg not found %i\n", blockType);
            return facesAdded;
        }

        BLOCK_REG& fndBlock = *ifoundBlock;

        //block_set_hash_code(lpctx, idx, idx);

        // logic:
        //   if no block on bottom, add a face
        //   if alpha on bottom, add a face

        // check for block on bottom (-z)
        if (sideToUpdate == 0 || sideToUpdate == EXISTS_ON_BOTTOM)
        {
            if (!IS_BITSET(flags, EXISTS_ON_BOTTOM) || IS_BITSET(flags, ALPHA_ON_BOTTOM)) {
                if (!IS_BITSET(flags, FACE_ON_BOTTOM)) {
                    SET_BIT(flags, FACE_ON_BOTTOM);
                    // texture index bottom
                    //add_vertex(lpctx, fndBlock, xlate, idx, TEXTURE_INDEX_BOTTOM, bottomVertexIndices, vertices);
                    //add_vertex(lpctx, fndBlock, xlate, idx, TEXTURE_INDEX_BOTTOM, bottomVertexIndices, vertices);
                    add_vertex(lpctx, fndBlock, TEXTURE_INDEX_BOTTOM, xlate, bottomVertexIndices, vertices);
                    facesAdded++;
                }
            }
        }

        // check for block on top (+z)
        if (sideToUpdate == 0 || sideToUpdate == EXISTS_ON_TOP)
        {
            if (!IS_BITSET(flags, EXISTS_ON_TOP) || IS_BITSET(flags, ALPHA_ON_TOP)) {
                if (!IS_BITSET(flags, FACE_ON_TOP)) {
                    for (int64_t v = 0; v < 6; v++) {
                        //lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_TOP]].vertices.push_back({
                        //    glm::vec3(xlate * locs[topVertexIndices[v]]),
                        //    texcrds[topVertexIndices[v]],normals[topVertexIndices[v]], {idx,0}
                        //    });
                        //add_vertex(lpctx, fndBlock, xlate, idx, TEXTURE_INDEX_TOP, topVertexIndices, vertices);
                        add_vertex(lpctx, fndBlock, TEXTURE_INDEX_TOP, xlate, topVertexIndices, vertices);
                    }
                    SET_BIT(flags, FACE_ON_TOP);
                    facesAdded++;
                }
            }
        }

        // check for block on +x
        if (sideToUpdate == 0 || sideToUpdate == EXISTS_PLUS_X)
        {
            if (!IS_BITSET(flags, EXISTS_PLUS_X) || IS_BITSET(flags, ALPHA_PLUS_X)) {
                if (!IS_BITSET(flags, FACE_PLUS_X)) {
                    for (int64_t v = 0; v < 6; v++) {
                        //lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_PLUSX]].vertices.push_back({
                            //glm::vec3(xlate * locs[plusxVertexIndices[v]]),
                            //texcrds[plusxVertexIndices[v]],normals[plusxVertexIndices[v]], {idx,0}
                            //});
                        //add_vertex(lpctx, fndBlock, xlate, idx, TEXTURE_INDEX_PLUSX, plusxVertexIndices, vertices);
                        add_vertex(lpctx, fndBlock, TEXTURE_INDEX_PLUSX, xlate, plusxVertexIndices, vertices);
                    }
                    SET_BIT(flags, FACE_PLUS_X);
                    facesAdded++;
                }
            }
        }

        // check for block on -x
        if (sideToUpdate == 0 || sideToUpdate == EXISTS_MINUS_X)
        {
            if (!IS_BITSET(flags, EXISTS_MINUS_X) || IS_BITSET(flags, ALPHA_MINUS_X)) {
                if (!IS_BITSET(flags, FACE_MINUS_X)) {
                    for (int64_t v = 0; v < 6; v++) {
                        //lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_MINUSX]].vertices.push_back({
                        //    glm::vec3(xlate * locs[minusxVertexIndices[v]]),
                        //    texcrds[minusxVertexIndices[v]],normals[minusxVertexIndices[v]], {idx,0}
                        //    });
                        //add_vertex(lpctx, fndBlock, xlate, idx, TEXTURE_INDEX_MINUSX, minusxVertexIndices, vertices);
                        add_vertex(lpctx, fndBlock, TEXTURE_INDEX_MINUSX, xlate, minusxVertexIndices, vertices);
                    }
                    SET_BIT(flags, FACE_MINUS_X);
                    facesAdded++;
                }
            }
        }

        // check for block on +y
        if (sideToUpdate == 0 || sideToUpdate == EXISTS_PLUS_Y)
        {
            if (!IS_BITSET(flags, EXISTS_PLUS_Y) || IS_BITSET(flags, ALPHA_PLUS_Y)) {
                if (!IS_BITSET(flags, FACE_PLUS_Y)) {
                    for (int64_t v = 0; v < 6; v++) {
                        //lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_PLUSY]].vertices.push_back({
                        //    glm::vec3(xlate * locs[plusyVertexIndices[v]]),
                        //    texcrds[plusyVertexIndices[v]],normals[plusyVertexIndices[v]], {idx,0}
                        //    });
                        //add_vertex(lpctx, fndBlock, xlate, idx, TEXTURE_INDEX_PLUSY, plusyVertexIndices, vertices);
                        add_vertex(lpctx, fndBlock, TEXTURE_INDEX_PLUSY, xlate, plusyVertexIndices, vertices);
                    }
                    SET_BIT(flags, FACE_PLUS_Y);
                    facesAdded++;
                }
            }
        }

        // check for block on -y
        if (sideToUpdate == 0 || sideToUpdate == EXISTS_MINUS_Y)
        {
            if (!IS_BITSET(flags, EXISTS_MINUS_Y) || IS_BITSET(flags, ALPHA_MINUS_Y)) {
                if (!IS_BITSET(flags, FACE_MINUS_Y)) {
                    for (int64_t v = 0; v < 6; v++) {
                        //lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_MINUSY]].vertices.push_back({
                        //    glm::vec3(xlate * locs[minusyVertexIndices[v]]),
                        //    texcrds[minusyVertexIndices[v]],normals[minusyVertexIndices[v]], {idx,0}
                        //    });
                        //add_vertex(lpctx, fndBlock, xlate, idx, TEXTURE_INDEX_MINUSY, minusyVertexIndices, vertices);
                        add_vertex(lpctx, fndBlock, TEXTURE_INDEX_MINUSY, xlate, minusyVertexIndices, vertices);
                    }
                    SET_BIT(flags, FACE_MINUS_Y);
                    facesAdded++;
                }
            }
        }

        // if any faces are set
        //if ((flags & FACES_ALL) && block_get_actor(lpctx, idx) == NULL)
        if ((flags & FACES_ALL) && block_get_actor(lpctx, xc, yc, zc) == NULL)
        {
            //block_create_new_actor(lpctx, idx, xc, yc, zc);
            block_create_new_actor(lpctx, xc, yc, zc, (float)xc, (float)yc, (float)zc);
        }

        //block_set_face_mask(lpctx, idx, faceMask);
        //block_set_flags(lpctx, idx, flags);
        block_set_flags(lpctx, xc, yc, zc, flags);
    }

    return facesAdded;
}

// the hight map is a buffer w pixels wide and h pixels high
// the values are 0 to 255
void create_vertex_buffer(VOXC_WINDOW_CONTEXT* lpctx, RENDER_MSG_ON_SCREEN_FNPTR msgfn, MESSAGE_CONTEXT* mctx)
{
    int texWidth = 512;
    int texHeight = 512;
    int texChannels = 1;
    //stbi_uc* pixels = stbi_load("c:\\temp\\height8.png", &texWidth, &texHeight, &texChannels, 1);
    //if (!pixels)
        //throw new std::runtime_error("failed to load height map");
    BYTE* pixels = (BYTE*)malloc(512 * 512);
    msgfn(mctx, "Generating terrain...", 1.0f);
    generate_terrain(pixels);

    printf("assigning block types\n");
    // assign blocks to level 0
    for (uint32_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
        msgfn(mctx, "Assigning block types...", yc / (float)Y_GRID_EXTENT);
        for (uint32_t xc = 0; xc < X_GRID_EXTENT; xc++) {

            int h = pixels[(yc * texWidth) + xc];

            if (h < 1) h = 1;
            for (uint32_t zc = 0; zc < h; zc++) {
                if(zc == (h - 1))
                {
                    block_set_regtype(lpctx, xc, yc, zc, REG_DIRTGRASS);
                }
                else {
                    block_set_regtype(lpctx, xc, yc, zc, REG_DIRT);
                }
            }

            // if the block is less than the z extent
            // put veg at the top
            if (h < Z_GRID_EXTENT) {
                if (rand() % 100 == 1)
                {
            //        // flower
            //        block_set_type(lpctx, xc, yc, h, 2);
                }
                else if (rand() % 200 == 1) {
                    // tree trunk
                    for (uint32_t th = 0; th < 6; th++) {
                        if ((h + th) < Z_GRID_EXTENT) {
                            if (th == 5) {
                                // the top of the tree (leaves)
                                //block_set_type(lpctx, xc, yc, h + th, REG_TREELEAVES);
                                for (uint32_t tlz = 1; tlz < 6; tlz++) {
                                    if (tlz < Z_GRID_EXTENT) {
                                        for (uint32_t ly = yc - 3; ly < yc + 4; ly++) {
                                            if (ly > 0 && ly < Y_GRID_EXTENT) {
                                                uint32_t tly = ly - (yc - 3);
                                                for (uint32_t lx = xc - 3; lx < xc + 4; lx++) {
                                                    if (lx > 0 && lx < X_GRID_EXTENT)
                                                    {
                                                        uint32_t tlx = lx - (xc - 3);
                                                        if (TreeLeaves[(tlz * 7 * 7) + (tly * 7) + tlx] == 1)
                                                        {
                                                            //int64_t lidx = GRIDIDX(lx, ly, h + th + (tlz - 1));
                                                            //if (block_get_regtype(lpctx, lidx, false) == 0) block_set_regtype(lpctx, lidx, REG_TREELEAVES);
                                                            if (block_get_regtype(lpctx, lx, ly, h + th + (tlz - 1)) == REG_AIR)
                                                            {
                                                                block_set_regtype(lpctx, lx, ly, h + th + (tlz - 1), REG_TREELEAVES);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else if (th == 4)
                            {
                                block_set_regtype(lpctx, xc, yc, h + th, REG_TREETRUNK);
                                for (uint32_t ly = yc - 3; ly < yc + 4; ly++) {
                                    if (ly > 0 && ly < Y_GRID_EXTENT) {
                                        uint32_t tly = ly - (yc - 3);
                                        for (uint32_t lx = xc - 3; lx < xc + 4; lx++) {
                                            if (lx > 0 && lx < X_GRID_EXTENT)
                                            {
                                                uint32_t tlx = lx - (xc - 3);
                                                if (TreeLeaves[(tly * 7) + tlx] == 1)
                                                {
                                                    //int64_t lidx = GRIDIDX(lx, ly, h + th);
                                                    //if(block_get_regtype(lpctx, lidx, false) == 0) block_set_regtype(lpctx, lidx, REG_TREELEAVES);
                                                    if (block_get_regtype(lpctx, lx, ly, h + th) == REG_AIR)
                                                    {
                                                        block_set_regtype(lpctx, lx, ly, h + th, REG_TREELEAVES);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else 
                            {
                                // the trunk of the tree
                                block_set_regtype(lpctx, xc, yc, h + th, REG_TREETRUNK);
                            }
                        }
                    }
                }
            }
        }
    }

    stbi_image_free(pixels);

    //printf("assigning surround values\n");
    //for (uint32_t zc = 0; zc < Z_GRID_EXTENT; zc++) {
    //    for (uint32_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
    //        for (uint32_t xc = 0; xc < X_GRID_EXTENT; xc++) {
    //            // update all aides
    //            update_masks(lpctx, xc, yc, zc, 0);
    //        }
    //    }
    //}

    //uint32_t block_compute_block_id(uint32_t x, uint32_t y, uint32_t z)
    //{
    //    uint32_t g = ((y >> 8) << 4) + (x >> 8);
    //    uint32_t blockId = COMPUTE_BLOCK_ID(g, x % 256, y % 256, z % 256);
    //}

    //glm::u32vec3 block_compute_position(uint32_t blockId)

    uint32_t bvctr = 0;
    uint32_t bvmax = lpctx->blockVector.size();
    uint32_t bvstep = bvmax / 100;
    std::unordered_map<uint32_t, BLOCK_ENTITY>::iterator ii = lpctx->blockVector.begin();
    for (; ii != lpctx->blockVector.end(); ++ii)
    {
        if(bvctr % bvstep == 0) 
            msgfn(mctx, "Updating bit masks...", bvctr / (float)bvmax);
        glm::u32vec3 upos = block_compute_position(ii->first);
        update_masks(lpctx, upos.x, upos.y, upos.z, 0);
        bvctr++;
    }

    bvctr = 0;
    bvmax = lpctx->blockVector.size();
    bvstep = bvmax / 100;
    for (auto bvi = lpctx->blockVector.begin(), last = lpctx->blockVector.end(); bvi != last; ) {
        if ((bvi->second.flags & EXISTS_ALL) == EXISTS_ALL)
        {
            bvi = lpctx->blockVector.erase(bvi);
            if (bvctr % bvstep == 0)
                msgfn(mctx, "Culling blocks...", bvctr / (float)bvmax);
            bvctr++;
        }
        else {
            ++bvi;
        }
    }

    //lpctx->blockVector.erase(
    //    std::remove_if(
    //        lpctx->blockVector.begin(),
    //        lpctx->blockVector.end(),
    //        [](std::pair<const uint32_t,BLOCK_ENTITY>& item) {
    //            return item.second.flags & EXISTS_ALL;
    //        }
    //    ),
    //    lpctx->blockVector.end());

                    //lpctx->groups[gi].vertices.erase(
                    //    std::remove_if(lpctx->groups[gi].vertices.begin(), lpctx->groups[gi].vertices.end(),
                    //        [hashCode](const VERTEX2& item) { return item.userData[0] == hashCode;  }),
                    //    lpctx->groups[gi].vertices.end());


    //printf("creating vertex array and physics actors\n");
    //for (int64_t zc = 0; zc < Z_GRID_EXTENT; zc++) {
    //    for (int64_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
    //        for (int64_t xc = 0; xc < X_GRID_EXTENT; xc++) {
    //            update_faces(lpctx, xc, yc, zc, 0);
    //        }
    //    }
    //}

    for (uint32_t zz = 0; zz < Z_GRID_EXTENT; zz += 16)
    {
        msgfn(mctx, "Generating faces...", zz / (float)Z_GRID_EXTENT);

        for (uint32_t yy = 0; yy < Y_GRID_EXTENT; yy += 16)
        {
            for (uint32_t xx = 0; xx < X_GRID_EXTENT; xx += 16)
            {

                VBLOCK_16 v16;
                v16.block16id = block_compute_block16_id(xx, yy, zz);
                v16.centroid = glm::vec2(xx + 8.0f, yy + 8.0f);

                std::map <GLuint, std::vector<VERTEX4>> vertices;
                for (uint32_t zc = zz; zc < zz + 16; zc++) {
                    for (uint32_t yc = yy; yc < yy + 16; yc++) {
                        for (uint32_t xc = xx; xc < xx + 16; xc++) {
                            // get vertices of faces
                            update_faces(lpctx, xc, yc, zc, 0, vertices);
                        }
                    }
                }

                uint32_t nverts = 0;
                for (const auto& vItem : vertices) nverts += vItem.second.size();
                if (nverts > 0) {

                    BYTE* mem = (BYTE*)malloc(nverts * sizeof(VERTEX4));
                    memset(mem, 0, nverts * sizeof(VERTEX4));

                    BYTE* mptr = mem;
                    for (const auto& vItem : vertices)
                    {
                        memcpy(mptr, vItem.second.data(), vItem.second.size() * sizeof(VERTEX4));
                        mptr += vItem.second.size() * sizeof(VERTEX4);
                        VBLOCK_SUBVERT sv;
                        sv.tex_id = vItem.first;
                        sv.num_vertices = vItem.second.size();
                      
                        //lpctx->draw.subverts.push_back(sv);
                        v16.subverts.push_back(sv);
                    }

                    // turn vertices into vbo
                    // now I have a map by texture id with vertices
                    // need to get some memory to put the vertices in
                    // sort them by texture id and record
                    // the texture id and offset in a list of structures
                    // and store them in the vblock
                    v16.vbo = 0;
                    //glCreateBuffers(1, &lpctx->draw.vbo);
                    //glNamedBufferStorage(lpctx->draw.vbo, nverts * sizeof(VERTEX4), mem, 0);
                    glCreateBuffers(1, &v16.vbo);
                    glNamedBufferStorage(v16.vbo, nverts * sizeof(VERTEX4), mem, 0);


                    free(mem);

                    lpctx->drawVector.push_back(v16);
                }

            }
        }
    }

}

// side to update is opposide the offset
int64_t blocksToAddress[] = {
    0,0,-1,EXISTS_ON_TOP,
    0,0,1,EXISTS_ON_BOTTOM,
    1,0,0,EXISTS_MINUS_X,
    -1,0,0,EXISTS_PLUS_X,
    0,1,0,EXISTS_MINUS_Y,
    0,-1,0,EXISTS_PLUS_Y
};

void update_surrounding_blocks(VOXC_WINDOW_CONTEXT* lpctx, int64_t xc, int64_t yc, int64_t zc)
{
    //printf("--update_surrounding_blocks--\n");

    //// update masks for this block
    //// surround mask needs to be updated
    //// alpha mask needs to be updated
    //update_masks(lpctx, xc, yc, zc, 0);
    //int64_t grididx = GRIDIDX(xc, yc, zc);
    //printf("%i - %i\n", grididx, block_get_regtype(lpctx, grididx, true));

    //// update faces and mask for 'some' surrounding blocks
    //for (int i = 0; i < 6; i++)
    //{
    //    int64_t xx = xc + blocksToAddress[i * 4];
    //    if (xx < 0 || xx > (X_GRID_EXTENT - 1)) continue;
    //    int64_t yy = yc + blocksToAddress[(i * 4) + 1];
    //    if (yy < 0 || yy > (Y_GRID_EXTENT - 1)) continue;
    //    int64_t zz = zc + blocksToAddress[(i * 4) + 2];
    //    if (zz < 0 || zz > (Z_GRID_EXTENT - 1)) continue;

    //    uint8_t sideToUpdate = (uint8_t)blocksToAddress[(i * 4) + 3];
    //    grididx = GRIDIDX(xx, yy, zz);
    //    printf("%i - %i\n", grididx, block_get_regtype(lpctx, grididx, false));
    //    update_masks(lpctx, xx, yy, zz, sideToUpdate);
    //    int fa = update_faces(lpctx, xx, yy, zz, sideToUpdate);
    //    printf("faces added = %i\n", fa);
    //}
}

