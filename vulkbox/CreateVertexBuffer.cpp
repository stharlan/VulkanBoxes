
#include "Common.h"



void HelloTriangleApplication::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    // beginSingleTimeCommands from create texture image
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    // endSingleTimeCommands from create texture image
    endSingleTimeCommands(commandBuffer);
}

void HelloTriangleApplication::createVertexBuffer()
{
    srand((unsigned int)time(NULL));

    uint32_t topVertexIndices[6] = { 0, 1, 2, 2, 3, 0 };
    uint32_t plusxVertexIndices[6] = { 4, 5, 6, 6, 7, 4 };
    uint32_t minusxVertexIndices[6] = { 8, 9, 10, 10, 11, 8 };
    uint32_t plusyVertexIndices[6] = { 12, 13, 14, 14, 15, 12 };
    uint32_t minusyVertexIndices[6] = { 16, 17, 18, 18, 19, 16 };
    uint32_t bottomVertexIndices[6] = { 20, 21, 22, 22, 23, 20 };
    uint32_t capVertices1[6] = { 24, 25, 26, 26, 27, 24 };
    uint32_t capVertices2[6] = { 28, 29, 30, 30, 31, 28 };
    uint32_t capVertices3[6] = { 32, 33, 34, 34, 35, 32 };
    uint32_t capVertices4[6] = { 36, 37, 38, 38, 39, 36 };

    int texWidth = 0;
    int texHeight = 0;
    int texChannels = 0;
    stbi_uc* pixels = stbi_load("c:\\temp\\height8.png", &texWidth, &texHeight,
        &texChannels, 1);

    // assign blocks to level 0
    for (int64_t yc = 0; yc < y_extent; yc++) {
        for (int64_t xc = 0; xc < x_extent; xc++) {

            //float height = ((cos((float)xc * 3.14159f / 64.0f) + 1.0f) / 2.0f) * z_extent;
            int h = pixels[(yc * texWidth) + xc];

            //if (xc < 10) printf("%i\n", h);

            if (h < 1) h = 1;
            //int height = ((float)h / 255.0f) * z_extent;
            for (int64_t zc = 0; zc < h; zc++) {
                this->pBlockArray[GRIDIDX(xc,yc,zc)] = 1;
            }
             
            // if the block is less than the z extent
            // put veg at the top
            if (h < z_extent) {
                if (rand() % 100 == 1)
                {
                    // flower
                    this->pBlockArray[GRIDIDX(xc, yc, h)] = 2;
                }
                else if (rand() % 1000 == 1) {
                    // tree trunk
                    for (uint64_t th = 0; th < 6; th++) {
                        if ((h + th) < z_extent) {
                            if (th == 5) {
                                this->pBlockArray[GRIDIDX(xc, yc, h + th)] = 4;
                            }
                            else {
                                this->pBlockArray[GRIDIDX(xc, yc, h + th)] = 3;
                            }
                        }
                    }
                }
            }
        }
    }

    stbi_image_free(pixels);

    for (int64_t zc = 0; zc < z_extent; zc++) {
        for (int64_t yc = 0; yc < y_extent; yc++) {
            for (int64_t xc = 0; xc < x_extent; xc++) {

                int64_t idx = GRIDIDX(xc, yc, zc);

                if (this->pBlockArray[idx] == 1 
                    || this->pBlockArray[idx] == 3
                    || this->pBlockArray[idx] == 4) {

                    float texOffset = 0.0f;
                    if (this->pBlockArray[idx] == 3) texOffset = 0.125f;
                    if (this->pBlockArray[idx] == 4) texOffset = 0.25f;

                    size_t numVerts = vertices4.size();

                    // check for block on bottom (-z)
                    if (zc > 0) {
                        if (this->pBlockArray[GRIDIDX(xc, yc, zc - 1)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                vertices4.push_back({ bottomVertexIndices[v], {xc, yc, zc}, texOffset });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            vertices4.push_back({ bottomVertexIndices[v], {xc, yc, zc}, texOffset });
                        }
                    }

                    // check for block on top (+z)
                    if (zc < z_extent - 1)
                    {
                        if (this->pBlockArray[GRIDIDX(xc, yc, zc + 1)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                vertices4.push_back({ topVertexIndices[v], {xc, yc, zc}, texOffset });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            vertices4.push_back({ topVertexIndices[v], {xc, yc, zc}, texOffset });
                        }
                    }

                    // check for block on +x
                    if (xc < x_extent - 1)
                    {
                        if (this->pBlockArray[GRIDIDX(xc + 1, yc, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                vertices4.push_back({ plusxVertexIndices[v], {xc, yc, zc}, texOffset });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            vertices4.push_back({ plusxVertexIndices[v], {xc, yc, zc}, texOffset });
                        }
                    }

                    // check for block on -x
                    if (xc > 0)
                    {
                        if (this->pBlockArray[GRIDIDX(xc - 1, yc, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                vertices4.push_back({ minusxVertexIndices[v], {xc, yc, zc}, texOffset });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            vertices4.push_back({ minusxVertexIndices[v], {xc, yc, zc}, texOffset });
                        }
                    }

                    // check for block on +y
                    if (yc < y_extent - 1)
                    {
                        if (this->pBlockArray[GRIDIDX(xc, yc + 1, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                vertices4.push_back({ plusyVertexIndices[v], {xc, yc, zc}, texOffset });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            vertices4.push_back({ plusyVertexIndices[v], {xc, yc, zc}, texOffset });
                        }
                    }

                    // check for block on -y
                    if (yc > 0)
                    {
                        if (this->pBlockArray[GRIDIDX(xc, yc - 1, zc)] != 1) {
                            for (int64_t v = 0; v < 6; v++) {
                                vertices4.push_back({ minusyVertexIndices[v], {xc, yc, zc}, texOffset });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            vertices4.push_back({ minusyVertexIndices[v], {xc, yc, zc}, texOffset });
                        }
                    }

                    if (vertices4.size() > numVerts)
                    {
                        // create new actors
                        this->pStaticBlockArray[idx] = this->mPhysics->createRigidStatic(
                            physx::PxTransform(xc + 0.5f, yc + 0.5f, zc + 0.5f));
                        this->pStaticBlockArray[idx]->attachShape(*this->mBlockShape);
                    }
                    else {
                        this->pStaticBlockArray[idx] = NULL;

                    }
                }
                else {
                    this->pStaticBlockArray[idx] = NULL;
                }
            }
        }
    }

    for (int64_t zc = 0; zc < z_extent; zc++) {
        for (int64_t yc = 0; yc < y_extent; yc++) {
            for (int64_t xc = 0; xc < x_extent; xc++) {
                if (this->pBlockArray[GRIDIDX(xc, yc, zc)] == 2) {
                    for (int64_t v = 0; v < 6; v++) {
                        vertices4.push_back({ capVertices1[v], {xc, yc, zc}, 0.0f });
                    }
                    for (int64_t v = 0; v < 6; v++) {
                        vertices4.push_back({ capVertices2[v], {xc, yc, zc}, 0.0f });
                    }
                    for (int64_t v = 0; v < 6; v++) {
                        vertices4.push_back({ capVertices3[v], {xc, yc, zc}, 0.0f });
                    }
                    for (int64_t v = 0; v < 6; v++) {
                        vertices4.push_back({ capVertices4[v], {xc, yc, zc}, 0.0f });
                    }
                }
            }
        }
    }

    vertices4.shrink_to_fit();

    VkDeviceSize bufferSize = sizeof(vertices4[0]) * vertices4.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    // createBuffer from create texture image
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices4.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    createBuffer(bufferSize, 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        vertexBuffer, vertexBufferMemory);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}
