
#pragma comment(lib, "C:\\VulkanSDK\\1.2.154.1\\Lib\\vulkan-1.lib")
#pragma comment(lib, "C:\\Library\\glfw-3.3.2.bin.WIN64\\lib-vc2019\\glfw3.lib")

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>
#include <time.h>

#include "HelloTriangleApplication.h"

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
    srand(time(NULL));

    uint32_t topVertexIndices[6] = { 0, 1, 2, 2, 3, 0 };
    uint32_t plusxVertexIndices[6] = { 4, 5, 6, 6, 7, 4 };
    uint32_t minusxVertexIndices[6] = { 8, 9, 10, 10, 11, 8 };
    uint32_t plusyVertexIndices[6] = { 12, 13, 14, 14, 15, 12 };
    uint32_t minusyVertexIndices[6] = { 16, 17, 18, 18, 19, 16 };


    // assign blocks to level 0
    for (int64_t zc = 0; zc < z_extent; zc++) {
        for (int64_t yc = 0; yc < y_extent; yc++) {
            for (int64_t xc = 0; xc < x_extent; xc++) {
                int64_t idx = (zc * x_extent * y_extent) + (yc * x_extent) + xc;
                if (zc == 0) blockArray[idx] = 1;
                else if (rand() % 100 == 5) blockArray[idx] = 1;
                else blockArray[idx] = 0;
            }
        }
    }

    for (int64_t zc = 0; zc < z_extent; zc++) {
        for (int64_t yc = 0; yc < y_extent; yc++) {
            for (int64_t xc = 0; xc < x_extent; xc++) {

                int64_t idx = (zc * x_extent * y_extent) + (yc * x_extent) + xc;

                // check for block on top (+z)
                if (blockArray[idx] == 1)
                {
                    if (zc < z_extent - 1)
                    {
                        int64_t idx_zplus = ((zc + 1) * x_extent * y_extent) + (yc * x_extent) + xc;
                        if (blockArray[idx_zplus] == 0) {
                            for (int64_t v = 0; v < 6; v++) {
                                vertices4.push_back({ topVertexIndices[v], {xc, yc, zc} });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            vertices4.push_back({ topVertexIndices[v], {xc, yc, zc} });
                        }
                    }

                    // check for block on +x
                    if (xc < x_extent - 1)
                    {
                        int64_t idx_xplus = (zc * x_extent * y_extent) + (yc * x_extent) + xc + 1;
                        if (blockArray[idx_xplus] == 0) {
                            for (int64_t v = 0; v < 6; v++) {
                                vertices4.push_back({ plusxVertexIndices[v], {xc, yc, zc} });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            vertices4.push_back({ plusxVertexIndices[v], {xc, yc, zc} });
                        }
                    }

                    // check for block on -x
                    if (xc > 0)
                    {
                        int64_t idx_xminus = (zc * x_extent * y_extent) + (yc * x_extent) + xc - 1;
                        if (blockArray[idx_xminus] == 0) {
                            for (int64_t v = 0; v < 6; v++) {
                                vertices4.push_back({ minusxVertexIndices[v], {xc, yc, zc} });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            vertices4.push_back({ minusxVertexIndices[v], {xc, yc, zc} });
                        }
                    }

                    // check for block on +y
                    if (yc < y_extent - 1)
                    {
                        int64_t idx_yplus = (zc * x_extent * y_extent) + ((yc + 1) * x_extent) + xc;
                        if (blockArray[idx_yplus] == 0) {
                            for (int64_t v = 0; v < 6; v++) {
                                vertices4.push_back({ plusyVertexIndices[v], {xc, yc, zc} });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            vertices4.push_back({ plusyVertexIndices[v], {xc, yc, zc} });
                        }
                    }

                    // check for block on -y
                    if (yc > 0)
                    {
                        int64_t idx_yminus = (zc * x_extent * y_extent) + ((yc - 1) * x_extent) + xc;
                        if (blockArray[idx_yminus] == 0) {
                            for (int64_t v = 0; v < 6; v++) {
                                vertices4.push_back({ minusyVertexIndices[v], {xc, yc, zc} });
                            }
                        }
                    }
                    else {
                        for (int64_t v = 0; v < 6; v++) {
                            vertices4.push_back({ minusyVertexIndices[v], {xc, yc, zc} });
                        }
                    }
                }

                // is there a block on top? no
                //for (int64_t v = 0; v < 6; v++) {
                    //vertices4.push_back({ topVertexIndices[v], {xc, yc, zc} });
                //}

                // is there a block on bottom? no
                //for (int64_t v = 0; v < 6; v++) {
                //    vertices4.push_back({ bottomVertexIndices[v], {xc, yc, zc} });
                //}
                //if (xc == 0) {
                //    for (int64_t v = 0; v < 6; v++) {
                //        vertices4.push_back({ xfrontVertexIndices[v], {xc, yc, zc} });
                //    }
                //}
                //if (yc == 0) {
                //    for (int64_t v = 0; v < 6; v++) {
                //        vertices4.push_back({ yfrontVertexIndices[v], {xc, yc, zc} });
                //    }
                //}
                //if (xc == x_extent - 1) {
                //    for (int64_t v = 0; v < 6; v++) {
                //        vertices4.push_back({ xbackVertexIndices[v], {xc, yc, zc} });
                //    }
                //}
                //if (yc == y_extent - 1) {
                //    for (int64_t v = 0; v < 6; v++) {
                //        vertices4.push_back({ ybackVertexIndices[v], {xc, yc, zc} });
                //    }
                //}
            }
        }
    }
    vertices4.shrink_to_fit();

    //int64_t gridSize = 50;
    //std::vector<glm::vec3> transArray;
    //transArray.resize(((2 * gridSize) + 1) * ((2 * gridSize) + 1));
    //for (int ix = -gridSize; ix < gridSize + 1; ix++) {
    //    for (int iy = -gridSize; iy < gridSize + 1; iy++) {
    //        int idx = ((ix + gridSize) * ((2 * gridSize) + 1)) + (iy + gridSize);
    //        transArray[idx] = glm::vec3((float)ix, (float)iy, 0.0f);
    //    }
    //}

    //vertices4.resize(transArray.size() * 6);
    //for (uint64_t f = 0; f < transArray.size(); f++) {
    //    for (uint64_t v = 0; v < 6; v++) {
    //        uint64_t i = (f * 6) + v;
    //        vertices4[i].idx = vertexIndices[v];
    //        vertices4[i].trans = transArray[f];
    //    }
    //}

    //VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
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
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}
