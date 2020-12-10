
#include "voxc.h"

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

void CreateVertexBuffer(VOXC_WINDOW_CONTEXT* lpctx)
{
    int texWidth = 0;
    int texHeight = 0;
    int texChannels = 0;
    stbi_uc* pixels = stbi_load("c:\\temp\\height8.png", &texWidth, &texHeight,
        &texChannels, 1);

    lpctx->pBlockArray = (int8_t*)malloc(X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT * sizeof(int8_t));
    memset(lpctx->pBlockArray, 0, X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT * sizeof(int8_t));

    // assign blocks to level 0
    for (int64_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
        for (int64_t xc = 0; xc < X_GRID_EXTENT; xc++) {

            int h = pixels[(yc * texWidth) + xc];

            if (h < 1) h = 1;
            for (int64_t zc = 0; zc < h; zc++) {
                lpctx->pBlockArray[GRIDIDX(xc, yc, zc)] = 1;
            }

            // if the block is less than the z extent
            // put veg at the top
            if (h < Z_GRID_EXTENT) {
                if (rand() % 100 == 1)
                {
                    // flower
                    lpctx->pBlockArray[GRIDIDX(xc, yc, h)] = 2;
                }
                else if (rand() % 1000 == 1) {
                    // tree trunk
                    for (uint64_t th = 0; th < 6; th++) {
                        if ((h + th) < Z_GRID_EXTENT) {
                            if (th == 5) {
                                lpctx->pBlockArray[GRIDIDX(xc, yc, h + th)] = 4;
                            }
                            else {
                                lpctx->pBlockArray[GRIDIDX(xc, yc, h + th)] = 3;
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

                if (lpctx->pBlockArray[idx] == 1
                    || lpctx->pBlockArray[idx] == 3
                    || lpctx->pBlockArray[idx] == 4) {

                    //size_t numVerts = lpctx->vertices4.size();

                    // check for block on bottom (-z)
                    if (zc > 0) {
                        if (lpctx->pBlockArray[GRIDIDX(xc, yc, zc - 1)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                lpctx->groups[TEX_DIRT].vertices.push_back({
                                    glm::vec3(xlate * locs[bottomVertexIndices[v]]),
                                    texcrds[bottomVertexIndices[v]],normals[bottomVertexIndices[v]]
                                    });
                                
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            lpctx->groups[TEX_DIRT].vertices.push_back({
                                glm::vec3(xlate * locs[bottomVertexIndices[v]]),
                                texcrds[bottomVertexIndices[v]],normals[bottomVertexIndices[v]]
                                });
                        }
                    }

                    // check for block on top (+z)
                    if (zc < Z_GRID_EXTENT - 1)
                    {
                        if (lpctx->pBlockArray[GRIDIDX(xc, yc, zc + 1)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                lpctx->groups[TEX_GRASS].vertices.push_back({
                                    glm::vec3(xlate * locs[topVertexIndices[v]]),
                                    texcrds[topVertexIndices[v]],normals[topVertexIndices[v]]
                                    });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            lpctx->groups[TEX_GRASS].vertices.push_back({
                                glm::vec3(xlate * locs[topVertexIndices[v]]),
                                texcrds[topVertexIndices[v]],normals[topVertexIndices[v]]
                                });
                        }
                    }

                    // check for block on +x
                    if (xc < X_GRID_EXTENT - 1)
                    {
                        if (lpctx->pBlockArray[GRIDIDX(xc + 1, yc, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                lpctx->groups[TEX_DIRTGRASS].vertices.push_back({
                                    glm::vec3(xlate * locs[plusxVertexIndices[v]]),
                                    texcrds[plusxVertexIndices[v]],normals[plusxVertexIndices[v]]
                                    });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            lpctx->groups[TEX_DIRTGRASS].vertices.push_back({
                                glm::vec3(xlate * locs[plusxVertexIndices[v]]),
                                texcrds[plusxVertexIndices[v]],normals[plusxVertexIndices[v]]
                                });
                        }
                    }

                    // check for block on -x
                    if (xc > 0)
                    {
                        if (lpctx->pBlockArray[GRIDIDX(xc - 1, yc, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                lpctx->groups[TEX_DIRTGRASS].vertices.push_back({
                                    glm::vec3(xlate * locs[minusxVertexIndices[v]]),
                                    texcrds[minusxVertexIndices[v]],normals[minusxVertexIndices[v]]
                                    });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            lpctx->groups[TEX_DIRTGRASS].vertices.push_back({
                                glm::vec3(xlate * locs[minusxVertexIndices[v]]),
                                texcrds[minusxVertexIndices[v]],normals[minusxVertexIndices[v]]
                                });
                        }
                    }

                    // check for block on +y
                    if (yc < Y_GRID_EXTENT - 1)
                    {
                        if (lpctx->pBlockArray[GRIDIDX(xc, yc + 1, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                lpctx->groups[TEX_DIRTGRASS].vertices.push_back({
                                    glm::vec3(xlate * locs[plusyVertexIndices[v]]),
                                    texcrds[plusyVertexIndices[v]],normals[plusyVertexIndices[v]]
                                    });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            lpctx->groups[TEX_DIRTGRASS].vertices.push_back({
                                glm::vec3(xlate * locs[plusyVertexIndices[v]]),
                                texcrds[plusyVertexIndices[v]],normals[plusyVertexIndices[v]]
                                });
                        }
                    }

                    // check for block on -y
                    if (yc > 0)
                    {
                        if (lpctx->pBlockArray[GRIDIDX(xc, yc - 1, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                lpctx->groups[TEX_DIRTGRASS].vertices.push_back({
                                    glm::vec3(xlate * locs[minusyVertexIndices[v]]),
                                    texcrds[minusyVertexIndices[v]],normals[minusyVertexIndices[v]]
                                    });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            lpctx->groups[TEX_DIRTGRASS].vertices.push_back({
                                glm::vec3(xlate * locs[minusyVertexIndices[v]]),
                                texcrds[minusyVertexIndices[v]],normals[minusyVertexIndices[v]]
                                });
                        }
                    }

                    //if (lpctx->vertices4.size() > numVerts)
                    //{
                        // create new actors
                        //this->pStaticBlockArray[idx] = this->mPhysics->createRigidStatic(
                            //physx::PxTransform(xc + 0.5f, yc + 0.5f, zc + 0.5f));
                        //this->pStaticBlockArray[idx]->attachShape(*this->mBlockShape);
                    //}
                    //else {
                        //this->pStaticBlockArray[idx] = NULL;

                    //}
                }
                else {
                    //this->pStaticBlockArray[idx] = NULL;
                }
            }
        }
    }

    std::vector<VERTEX_BUFFER_GROUP1>::iterator i = lpctx->groups.begin();
    for (; i != lpctx->groups.end(); ++i)
    {
        i->vertices.shrink_to_fit();
    }
}
