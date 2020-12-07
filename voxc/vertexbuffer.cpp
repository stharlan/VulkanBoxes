
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

                    float texOffset = 0.0f;
                    if (lpctx->pBlockArray[idx] == 3) texOffset = 0.125f;
                    if (lpctx->pBlockArray[idx] == 4) texOffset = 0.25f;

                    size_t numVerts = lpctx->vertices4.size();

                    // check for block on bottom (-z)
                    if (zc > 0) {
                        if (lpctx->pBlockArray[GRIDIDX(xc, yc, zc - 1)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                //vertices4.push_back({ bottomVertexIndices[v], {xc, yc, zc}, texOffset });
                                lpctx->vertices4.push_back({ 
                                    xlate * locs[bottomVertexIndices[v]],
                                    //texcrds[bottomVertexIndices[v]] 
                                    {texcrds[bottomVertexIndices[v]].x, texcrds[bottomVertexIndices[v]].y + texOffset}
                                    });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            //vertices4.push_back({ bottomVertexIndices[v], {xc, yc, zc}, texOffset });
                            lpctx->vertices4.push_back({ 
                                xlate * locs[bottomVertexIndices[v]],
                                {texcrds[bottomVertexIndices[v]].x, texcrds[bottomVertexIndices[v]].y + texOffset}
                                });
                        }
                    }

                    // check for block on top (+z)
                    if (zc < Z_GRID_EXTENT - 1)
                    {
                        if (lpctx->pBlockArray[GRIDIDX(xc, yc, zc + 1)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                //vertices4.push_back({ topVertexIndices[v], {xc, yc, zc}, texOffset });
                                lpctx->vertices4.push_back({ 
                                    xlate * locs[topVertexIndices[v]],
                                    //texcrds[topVertexIndices[v]] 
                                    {texcrds[topVertexIndices[v]].x, texcrds[topVertexIndices[v]].y + texOffset}
                                    });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            //vertices4.push_back({ topVertexIndices[v], {xc, yc, zc}, texOffset });
                            lpctx->vertices4.push_back({ 
                                xlate * locs[topVertexIndices[v]],
                                {texcrds[topVertexIndices[v]].x, texcrds[topVertexIndices[v]].y + texOffset}
                                });
                        }
                    }

                    // check for block on +x
                    if (xc < X_GRID_EXTENT - 1)
                    {
                        if (lpctx->pBlockArray[GRIDIDX(xc + 1, yc, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                //vertices4.push_back({ plusxVertexIndices[v], {xc, yc, zc}, texOffset });
                                lpctx->vertices4.push_back({ 
                                    xlate * locs[plusxVertexIndices[v]],
                                    //texcrds[plusxVertexIndices[v]] 
                                    {texcrds[plusxVertexIndices[v]].x, texcrds[plusxVertexIndices[v]].y + texOffset}
                                    });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            //vertices4.push_back({ plusxVertexIndices[v], {xc, yc, zc}, texOffset });
                            lpctx->vertices4.push_back({ 
                                xlate * locs[plusxVertexIndices[v]],
                                {texcrds[plusxVertexIndices[v]].x, texcrds[plusxVertexIndices[v]].y + texOffset}
                                });
                        }
                    }

                    // check for block on -x
                    if (xc > 0)
                    {
                        if (lpctx->pBlockArray[GRIDIDX(xc - 1, yc, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                //vertices4.push_back({ minusxVertexIndices[v], {xc, yc, zc}, texOffset });
                                lpctx->vertices4.push_back({ 
                                    xlate * locs[minusxVertexIndices[v]],
                                    //texcrds[minusxVertexIndices[v]] 
                                    {texcrds[minusxVertexIndices[v]].x, texcrds[minusxVertexIndices[v]].y + texOffset}
                                    });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            //vertices4.push_back({ minusxVertexIndices[v], {xc, yc, zc}, texOffset });
                            lpctx->vertices4.push_back({ 
                                xlate * locs[minusxVertexIndices[v]],
                                //texcrds[minusxVertexIndices[v]] 
                                {texcrds[minusxVertexIndices[v]].x, texcrds[minusxVertexIndices[v]].y + texOffset}
                                });
                        }
                    }

                    // check for block on +y
                    if (yc < Y_GRID_EXTENT - 1)
                    {
                        if (lpctx->pBlockArray[GRIDIDX(xc, yc + 1, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                //vertices4.push_back({ plusyVertexIndices[v], {xc, yc, zc}, texOffset });
                                lpctx->vertices4.push_back({ 
                                    xlate * locs[plusyVertexIndices[v]],
                                    //texcrds[plusyVertexIndices[v]] 
                                    {texcrds[plusyVertexIndices[v]].x, texcrds[plusyVertexIndices[v]].y + texOffset}
                                    });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            //vertices4.push_back({ plusyVertexIndices[v], {xc, yc, zc}, texOffset });
                            lpctx->vertices4.push_back({ 
                                xlate * locs[plusyVertexIndices[v]],
                                {texcrds[plusyVertexIndices[v]].x, texcrds[plusyVertexIndices[v]].y + texOffset}
                                });
                        }
                    }

                    // check for block on -y
                    if (yc > 0)
                    {
                        if (lpctx->pBlockArray[GRIDIDX(xc, yc - 1, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                //vertices4.push_back({ minusyVertexIndices[v], {xc, yc, zc}, texOffset });
                                lpctx->vertices4.push_back({ 
                                    xlate * locs[minusyVertexIndices[v]],
                                    //texcrds[minusyVertexIndices[v]] 
                                    {texcrds[minusyVertexIndices[v]].x, texcrds[minusyVertexIndices[v]].y + texOffset}
                                    });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            //vertices4.push_back({ minusyVertexIndices[v], {xc, yc, zc}, texOffset });
                            lpctx->vertices4.push_back({ 
                                xlate * locs[minusyVertexIndices[v]],
                                {texcrds[minusyVertexIndices[v]].x, texcrds[minusyVertexIndices[v]].y + texOffset}
                                });
                        }
                    }

                    if (lpctx->vertices4.size() > numVerts)
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

    lpctx->vertices4.shrink_to_fit();
}
