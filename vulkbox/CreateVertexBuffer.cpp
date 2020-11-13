
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
    uint32_t topVertexIndices[6] = { 0, 1, 2, 2, 3, 0 };
    uint32_t bottomVertexIndices[6] = { 4, 6, 5, 6, 4, 7 };
    uint32_t xfrontVertexIndices[6] = { 0, 3, 4, 3, 7, 4 };
    uint32_t yfrontVertexIndices[6] = { 1, 0, 5, 5, 0, 4 };
    uint32_t xbackVertexIndices[6] = { 1, 5, 2, 2, 5, 6 };
    uint32_t ybackVertexIndices[6] = { 6, 7, 2, 2, 7, 3 };
    int64_t x_extent = 64; // 0 to extent
    int64_t y_extent = 64; // 0 to extent
    int64_t z_extent = 1; // 0 to extent
    for (int64_t zc = 0; zc < z_extent; zc++) {
        for (int64_t yc = 0; yc < y_extent; yc++) {
            for (int64_t xc = 0; xc < x_extent; xc++) {
                // is there a block on top? no
                for (int64_t v = 0; v < 6; v++) {
                    vertices4.push_back({ topVertexIndices[v], {xc, yc, zc} });
                }
                // is there a block on bottom? no
                for (int64_t v = 0; v < 6; v++) {
                    vertices4.push_back({ bottomVertexIndices[v], {xc, yc, zc} });
                }
                if (xc == 0) {
                    for (int64_t v = 0; v < 6; v++) {
                        vertices4.push_back({ xfrontVertexIndices[v], {xc, yc, zc} });
                    }
                }
                if (yc == 0) {
                    for (int64_t v = 0; v < 6; v++) {
                        vertices4.push_back({ yfrontVertexIndices[v], {xc, yc, zc} });
                    }
                }
                if (xc == x_extent - 1) {
                    for (int64_t v = 0; v < 6; v++) {
                        vertices4.push_back({ xbackVertexIndices[v], {xc, yc, zc} });
                    }
                }
                if (yc == y_extent - 1) {
                    for (int64_t v = 0; v < 6; v++) {
                        vertices4.push_back({ ybackVertexIndices[v], {xc, yc, zc} });
                    }
                }
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
