
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

const int8_t TreeLeaves[] = {
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

GLuint CreateZeroCube()
{
    glm::mat4 xz = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, -0.5f));
    glm::mat4 sc = glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 1.1f, 1.1f));
    std::vector<VERTEX2> zeroCubeVerts;
    for (int i = 0; i < 6; i++) 
        zeroCubeVerts.push_back({ sc * (xz * locs[topVertexIndices[i]]), texcrds[topVertexIndices[i]], normals[topVertexIndices[i]], {0,0} });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[bottomVertexIndices[i]]), texcrds[bottomVertexIndices[i]], normals[bottomVertexIndices[i]], {0,0} });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[plusxVertexIndices[i]]), texcrds[plusxVertexIndices[i]], normals[plusxVertexIndices[i]], {0,0} });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[minusxVertexIndices[i]]), texcrds[minusxVertexIndices[i]], normals[minusxVertexIndices[i]], {0,0} });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[plusyVertexIndices[i]]), texcrds[plusyVertexIndices[i]], normals[plusyVertexIndices[i]], {0,0} });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[minusyVertexIndices[i]]), texcrds[minusyVertexIndices[i]], normals[minusyVertexIndices[i]], {0,0} });
    GLuint zeroCubeId = 0;
    glCreateBuffers(1, &zeroCubeId);
    glNamedBufferStorage(zeroCubeId, sizeof(VERTEX2) * 36, zeroCubeVerts.data(), 0);
    return zeroCubeId;
}

void update_mask_surround(VOXC_WINDOW_CONTEXT * lpctx, int64_t tx, int64_t ty, int64_t tz, int64_t idx,
    uint8_t bitMask)
{
    int8_t blockType = block_get_type(lpctx, tx, ty, tz);
    if (blockType) {
        SET_BIT(lpctx->blockEntities[idx].surroundExistsMask, bitMask);
        std::vector<BLOCK_REG>::iterator ifb = std::find_if(
            vBlockRegistry.begin(),
            vBlockRegistry.end(),
            [&](const BLOCK_REG& item) { return item.regType == blockType; });
        if (ifb != vBlockRegistry.end()) {
            if (TRUE == ifb->isTransparent) SET_BIT(lpctx->blockEntities[idx].surroundAlphaMask, bitMask);
        }
    }
    else {
        CLEAR_BIT(lpctx->blockEntities[idx].surroundExistsMask, bitMask);
        CLEAR_BIT(lpctx->blockEntities[idx].surroundAlphaMask, bitMask);
    }
}

// this does alpha and exists
void update_masks(VOXC_WINDOW_CONTEXT* lpctx, int64_t xc, int64_t yc, int64_t zc)
{
    int64_t idx = GRIDIDX(xc, yc, zc);
    if (zc < (Z_GRID_EXTENT - 1)) update_mask_surround(lpctx, xc, yc, zc + 1, idx, SURR_ON_TOP);
    if (zc > 0) update_mask_surround(lpctx, xc, yc, zc - 1, idx, SURR_ON_BOTTOM);
    if (xc < (X_GRID_EXTENT - 1)) update_mask_surround(lpctx, xc + 1, yc, zc, idx, SURR_PLUS_X);
    if (xc > 0) update_mask_surround(lpctx, xc - 1, yc, zc, idx, SURR_MINUS_X);
    if (yc < (Y_GRID_EXTENT - 1)) update_mask_surround(lpctx, xc, yc + 1, zc, idx, SURR_PLUS_Y);
    if (yc > 0) update_mask_surround(lpctx, xc, yc - 1, zc, idx, SURR_MINUS_Y);
}

void update_faces(VOXC_WINDOW_CONTEXT* lpctx, int64_t xc, int64_t yc, int64_t zc)
{
    int64_t idx = GRIDIDX(xc, yc, zc);

    int8_t blockType = block_get_type(lpctx, idx);

    if (blockType)
    {

        glm::vec3 translateVector = glm::vec3(xc, yc, zc);
        glm::mat4 xlate = glm::translate(glm::mat4(1.0f), translateVector);

        uint8_t existsMask = block_get_surround_exists_mask(lpctx, idx);
        uint8_t alphaMask = block_get_surround_alpha_mask(lpctx, idx);
        uint8_t faceMask = 0;

        std::vector<BLOCK_REG>::iterator ifoundBlock = std::find_if(
            vBlockRegistry.begin(),
            vBlockRegistry.end(),
            [&](const BLOCK_REG& item) { return item.regType == blockType; });
        if (ifoundBlock == vBlockRegistry.end())
        {
            printf("block reg not found %i\n", blockType);
            return;
        }

        BLOCK_REG& fndBlock = *ifoundBlock;

        uint64_t facesAdded = 0;

        block_set_hash_code(lpctx, idx, idx);

        // check for block on bottom (-z)
        if ((existsMask & SURR_ON_BOTTOM) == 0 || (alphaMask & SURR_ON_BOTTOM)) {
            for (int64_t v = 0; v < 6; v++) {
                lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_BOTTOM]].vertices.push_back({
                    glm::vec3(xlate * locs[bottomVertexIndices[v]]),
                    texcrds[bottomVertexIndices[v]],normals[bottomVertexIndices[v]], {idx,0}
                    });
                facesAdded++;
                faceMask |= SURR_ON_BOTTOM;
            }
        }

        // check for block on top (+z)
        if ((existsMask & SURR_ON_TOP) == 0 || (alphaMask & SURR_ON_TOP)) {
            for (int64_t v = 0; v < 6; v++) {
                lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_TOP]].vertices.push_back({
                    glm::vec3(xlate * locs[topVertexIndices[v]]),
                    texcrds[topVertexIndices[v]],normals[topVertexIndices[v]], {idx,0}
                    });
                facesAdded++;
                faceMask |= SURR_ON_TOP;
            }
        }

        // check for block on +x
        if ((existsMask & SURR_PLUS_X) == 0 || (alphaMask & SURR_PLUS_X)) {
            for (int64_t v = 0; v < 6; v++) {
                lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_PLUSX]].vertices.push_back({
                    glm::vec3(xlate * locs[plusxVertexIndices[v]]),
                    texcrds[plusxVertexIndices[v]],normals[plusxVertexIndices[v]], {idx,0}
                    });
                facesAdded++;
                faceMask |= SURR_PLUS_X;
            }
        }

        // check for block on -x
        if ((existsMask & SURR_MINUS_X) == 0 || (alphaMask & SURR_MINUS_X)) {
            for (int64_t v = 0; v < 6; v++) {
                lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_MINUSX]].vertices.push_back({
                    glm::vec3(xlate * locs[minusxVertexIndices[v]]),
                    texcrds[minusxVertexIndices[v]],normals[minusxVertexIndices[v]], {idx,0}
                    });
                facesAdded++;
                faceMask |= SURR_MINUS_X;
            }
        }

        // check for block on +y
        if ((existsMask & SURR_PLUS_Y) == 0 || (alphaMask & SURR_PLUS_Y)) {
            for (int64_t v = 0; v < 6; v++) {
                lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_PLUSY]].vertices.push_back({
                    glm::vec3(xlate * locs[plusyVertexIndices[v]]),
                    texcrds[plusyVertexIndices[v]],normals[plusyVertexIndices[v]], {idx,0}
                    });
                facesAdded++;
                faceMask |= SURR_PLUS_Y;
            }
        }

        // check for block on -y
        if ((existsMask & SURR_MINUS_Y) == 0 || (alphaMask & SURR_MINUS_Y)) {
            for (int64_t v = 0; v < 6; v++) {
                lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_MINUSY]].vertices.push_back({
                    glm::vec3(xlate * locs[minusyVertexIndices[v]]),
                    texcrds[minusyVertexIndices[v]],normals[minusyVertexIndices[v]], {idx,0}
                    });
                facesAdded++;
                faceMask |= SURR_MINUS_Y;
            }
        }

        if (facesAdded > 0)
        {
            // create new actors
            block_create_new_actor(lpctx, idx, xc, yc, zc);
            block_set_face_mask(lpctx, idx, faceMask);
        }
    }

}

void CreateVertexBuffer(VOXC_WINDOW_CONTEXT* lpctx)
{
    int texWidth = 0;
    int texHeight = 0;
    int texChannels = 0;
    stbi_uc* pixels = stbi_load("c:\\temp\\height8.png", &texWidth, &texHeight,
        &texChannels, 1);

    printf("assigning block types\n");
    // assign blocks to level 0
    for (int64_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
        for (int64_t xc = 0; xc < X_GRID_EXTENT; xc++) {

            int h = pixels[(yc * texWidth) + xc];

            if (h < 1) h = 1;
            for (int64_t zc = 0; zc < h; zc++) {
                if(zc == (h  - 1))
                {
                    block_set_type(lpctx, xc, yc, zc, REG_DIRTGRASS);
                }
                else {
                    block_set_type(lpctx, xc, yc, zc, REG_DIRT);
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
                else if (rand() % 1000 == 1) {
                    // tree trunk
                    for (uint64_t th = 0; th < 6; th++) {
                        if ((h + th) < Z_GRID_EXTENT) {
                            if (th == 5) {
                                // the top of the tree (leaves)
                                //block_set_type(lpctx, xc, yc, h + th, REG_TREELEAVES);
                                for (int64_t tlz = 1; tlz < 6; tlz++) {
                                    if (tlz < Z_GRID_EXTENT) {
                                        for (int64_t ly = yc - 3; ly < yc + 4; ly++) {
                                            if (ly > 0 && ly < Y_GRID_EXTENT) {
                                                int64_t tly = ly - (yc - 3);
                                                for (int64_t lx = xc - 3; lx < xc + 4; lx++) {
                                                    if (lx > 0 && lx < X_GRID_EXTENT)
                                                    {
                                                        int64_t tlx = lx - (xc - 3);
                                                        if (TreeLeaves[(tlz * 7 * 7) + (tly * 7) + tlx] == 1)
                                                        {
                                                            int64_t lidx = GRIDIDX(lx, ly, h + th + (tlz - 1));
                                                            if (block_get_type(lpctx, lidx) == 0) block_set_type(lpctx, lidx, REG_TREELEAVES);
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
                                block_set_type(lpctx, xc, yc, h + th, REG_TREETRUNK);
                                for (int64_t ly = yc - 3; ly < yc + 4; ly++) {
                                    if (ly > 0 && ly < Y_GRID_EXTENT) {
                                        int64_t tly = ly - (yc - 3);
                                        for (int64_t lx = xc - 3; lx < xc + 4; lx++) {
                                            if (lx > 0 && lx < X_GRID_EXTENT)
                                            {
                                                int64_t tlx = lx - (xc - 3);
                                                if (TreeLeaves[(tly * 7) + tlx] == 1)
                                                {
                                                    int64_t lidx = GRIDIDX(lx, ly, h + th);
                                                    if(block_get_type(lpctx, lidx) == 0) block_set_type(lpctx, lidx, REG_TREELEAVES);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else 
                            {
                                // the trunk of the tree
                                block_set_type(lpctx, xc, yc, h + th, REG_TREETRUNK);
                            }
                        }
                    }
                }
            }
        }
    }

    stbi_image_free(pixels);

    printf("assigning surround values\n");
    for (int64_t zc = 0; zc < Z_GRID_EXTENT; zc++) {
        for (int64_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
            for (int64_t xc = 0; xc < X_GRID_EXTENT; xc++) {
                update_masks(lpctx, xc, yc, zc);
            }
        }
    }

    printf("creating vertex array and physics actors\n");
    for (int64_t zc = 0; zc < Z_GRID_EXTENT; zc++) {
        for (int64_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
            for (int64_t xc = 0; xc < X_GRID_EXTENT; xc++) {
                update_faces(lpctx, xc, yc, zc);
            }
        }
    }
}

int64_t blocksToAddress[] = {
    0,0,-1,
    0,0,1,
    1,0,0,
    -1,0,0,
    0,1,0,
    0,-1,0
};

void update_surrounding_blocks(VOXC_WINDOW_CONTEXT* lpctx, int64_t xc, int64_t yc, int64_t zc)
{
    // update masks for this block
    update_masks(lpctx, xc, yc, zc);

    //// update faces and mask for 'some' surrounding blocks
    //for (int i = 0; i < 6; i++)
    //{
    //    int64_t xx = xc + blocksToAddress[i * 3];
    //    int64_t yy = yc + blocksToAddress[(i * 3) + 1];
    //    int64_t zz = zc + blocksToAddress[(i * 3) + 2];
    //    create_block_faces(lpctx, xx, yy, zz);
    //}
}

