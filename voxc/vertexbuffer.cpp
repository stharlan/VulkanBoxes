
#include "voxc.h"

std::vector<BLOCK_REG> vBlockRegistry = {
    { REG_DIRT, {TEXTURE_IMG_DIRT,TEXTURE_IMG_DIRT,TEXTURE_IMG_DIRT,TEXTURE_IMG_DIRT,TEXTURE_IMG_DIRT,TEXTURE_IMG_DIRT}},
    { REG_DIRTGRASS, {TEXTURE_IMG_GRASS, TEXTURE_IMG_DIRT, TEXTURE_IMG_DIRTGRASS,TEXTURE_IMG_DIRTGRASS,TEXTURE_IMG_DIRTGRASS,TEXTURE_IMG_DIRTGRASS}},
    { REG_TREETRUNK, {TEXTURE_IMG_WOODRINGS,TEXTURE_IMG_WOODRINGS,TEXTURE_IMG_WOODBARK,TEXTURE_IMG_WOODBARK,TEXTURE_IMG_WOODBARK,TEXTURE_IMG_WOODBARK}},
    { REG_TREELEAVES, {TEXTURE_IMG_LEAVES,TEXTURE_IMG_LEAVES,TEXTURE_IMG_LEAVES,TEXTURE_IMG_LEAVES,TEXTURE_IMG_LEAVES,TEXTURE_IMG_LEAVES} }
};

class BlockRegFindById : public std::unary_function<BLOCK_REG, bool> {
    int64_t regtype;
public:
    explicit BlockRegFindById(const int64_t type) : regtype(type) {}
    bool operator() (const BLOCK_REG &item) const { return item.regType == regtype; }
};

const GLuint topVertexIndices[6] = { 0, 1, 2, 2, 3, 0 };
const GLuint plusxVertexIndices[6] = { 4, 5, 6, 6, 7, 4 };
const GLuint minusxVertexIndices[6] = { 8, 9, 10, 10, 11, 8 };
const GLuint plusyVertexIndices[6] = { 12, 13, 14, 14, 15, 12 };
const GLuint minusyVertexIndices[6] = { 16, 17, 18, 18, 19, 16 };
const GLuint bottomVertexIndices[6] = { 20, 21, 22, 22, 23, 20 };

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
    std::vector<VERTEX1> zeroCubeVerts;
    for (int i = 0; i < 6; i++) 
        zeroCubeVerts.push_back({ sc * (xz * locs[topVertexIndices[i]]), texcrds[topVertexIndices[i]], normals[topVertexIndices[i]], });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[bottomVertexIndices[i]]), texcrds[bottomVertexIndices[i]], normals[bottomVertexIndices[i]], });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[plusxVertexIndices[i]]), texcrds[plusxVertexIndices[i]], normals[plusxVertexIndices[i]], });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[minusxVertexIndices[i]]), texcrds[minusxVertexIndices[i]], normals[minusxVertexIndices[i]], });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[plusyVertexIndices[i]]), texcrds[plusyVertexIndices[i]], normals[plusyVertexIndices[i]], });
    for (int i = 0; i < 6; i++)
        zeroCubeVerts.push_back({ sc * (xz * locs[minusyVertexIndices[i]]), texcrds[minusyVertexIndices[i]], normals[minusyVertexIndices[i]], });
    GLuint zeroCubeId = 0;
    glCreateBuffers(1, &zeroCubeId);
    glNamedBufferStorage(zeroCubeId, sizeof(VERTEX1) * 36, zeroCubeVerts.data(), 0);
    return zeroCubeId;
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
                                block_set_type(lpctx, xc, yc, h + th, REG_TREELEAVES);
                            }
                            else {
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
                int64_t idx = GRIDIDX(xc, yc, zc);
                if (zc < (Z_GRID_EXTENT - 1))
                {
                    if(block_get_type(lpctx, xc, yc, zc + 1)) lpctx->blockEntities[idx].surround |= SURR_ON_TOP;
                }
                if (zc > 0) {
                    if(block_get_type(lpctx, xc, yc, zc - 1)) lpctx->blockEntities[idx].surround |= SURR_ON_BOTTOM;
                }
                if (xc < (X_GRID_EXTENT - 1))
                {
                    if(block_get_type(lpctx, xc + 1, yc, zc))  lpctx->blockEntities[idx].surround |= SURR_PLUS_X;
                }
                if (xc > 0) {
                    if(block_get_type(lpctx, xc - 1, yc, zc)) lpctx->blockEntities[idx].surround |= SURR_MINUS_X;
                }
                if (yc < (Y_GRID_EXTENT - 1))
                {
                    if(block_get_type(lpctx, xc, yc + 1, zc)) lpctx->blockEntities[idx].surround |= SURR_PLUS_Y;
                }
                if (yc > 0) {
                    if(block_get_type(lpctx, xc, yc - 1, zc)) lpctx->blockEntities[idx].surround |= SURR_MINUS_Y;
                }
            }
        }
    }

    printf("creating vertex array and physics actors\n");
    for (int64_t zc = 0; zc < Z_GRID_EXTENT; zc++) {
        for (int64_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
            for (int64_t xc = 0; xc < X_GRID_EXTENT; xc++) {

                glm::mat4 xlate = glm::translate(glm::mat4(1.0f), glm::vec3(xc, yc, zc));

                int64_t idx = GRIDIDX(xc, yc, zc);

                int8_t blockType = block_get_type(lpctx, idx);
                uint8_t mask = block_get_surround_mask(lpctx, idx);
                if(blockType)
                {

                    std::vector<BLOCK_REG>::iterator ifoundBlock = std::find_if(vBlockRegistry.begin(), vBlockRegistry.end(), BlockRegFindById(blockType));
                    if (ifoundBlock == vBlockRegistry.end())
                    {
                        printf("block reg not found %i\n", blockType);
                        continue;
                    }

                    BLOCK_REG& fndBlock = *ifoundBlock;

                    uint64_t facesAdded = 0;

                    // check for block on bottom (-z)
                    if ((mask & SURR_ON_BOTTOM) == 0) {
                        for (int64_t v = 0; v < 6; v++) {
                            lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_BOTTOM]].vertices.push_back({
                                glm::vec3(xlate * locs[bottomVertexIndices[v]]),
                                texcrds[bottomVertexIndices[v]],normals[bottomVertexIndices[v]]
                                });
                            facesAdded++;
                        }
                    }

                    // check for block on top (+z)
                    if ((mask & SURR_ON_TOP) == 0) {
                        for (int64_t v = 0; v < 6; v++) {
                            lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_TOP]].vertices.push_back({
                                glm::vec3(xlate * locs[topVertexIndices[v]]),
                                texcrds[topVertexIndices[v]],normals[topVertexIndices[v]]
                                });
                            facesAdded++;
                        }
                    }

                    // check for block on +x
                    if ((mask & SURR_PLUS_X) == 0) {
                        for (int64_t v = 0; v < 6; v++) {
                            lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_PLUSX]].vertices.push_back({
                                glm::vec3(xlate * locs[plusxVertexIndices[v]]),
                                texcrds[plusxVertexIndices[v]],normals[plusxVertexIndices[v]]
                                });
                            facesAdded++;
                        }
                    }

                    // check for block on -x
                    if ((mask & SURR_MINUS_X) == 0) {
                        for (int64_t v = 0; v < 6; v++) {
                            lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_MINUSX]].vertices.push_back({
                                glm::vec3(xlate * locs[minusxVertexIndices[v]]),
                                texcrds[minusxVertexIndices[v]],normals[minusxVertexIndices[v]]
                                });
                            facesAdded++;
                        }
                    }

                    // check for block on +y
                    if ((mask & SURR_PLUS_Y) == 0) {
                        for (int64_t v = 0; v < 6; v++) {
                            lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_PLUSY]].vertices.push_back({
                                glm::vec3(xlate * locs[plusyVertexIndices[v]]),
                                texcrds[plusyVertexIndices[v]],normals[plusyVertexIndices[v]]
                                });
                            facesAdded++;
                        }
                    }

                    // check for block on -y
                    if ((mask & SURR_MINUS_Y) == 0) {
                        for (int64_t v = 0; v < 6; v++) {
                            lpctx->groups[fndBlock.textureIndex[TEXTURE_INDEX_MINUSY]].vertices.push_back({
                                glm::vec3(xlate * locs[minusyVertexIndices[v]]),
                                texcrds[minusyVertexIndices[v]],normals[minusyVertexIndices[v]]
                                });
                            facesAdded++;
                        }
                    }

                    if(facesAdded > 0)
                    {
                        // create new actors
                        block_create_new_actor(lpctx, idx, xc, yc, zc);
                    }
                }
            }
        }
    }
}
