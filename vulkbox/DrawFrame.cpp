
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

void HelloTriangleApplication::recreateSwapChain()
{
    int width = 0, height = 0;
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createDepthResources();
    createFramebuffers();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
}

const float FLOOR = 2.0f;
const float JUMP_VELOCITY = 5.0f;
const float ACCEL_GRAVITY = -9.8f;

void HelloTriangleApplication::updateUniformBuffer(uint32_t currentImage, float elapsed)
{
    //static auto startTime = std::chrono::high_resolution_clock::now();
    
    //auto currentTime = std::chrono::high_resolution_clock::now();
    //float time = 
        //std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime)
        //.count();

    if (keys[0] == 1) {
        // w
        ex += cosf(DEG2RAD(azimuth)) * WalkingStride;
        ey += sinf(DEG2RAD(azimuth)) * WalkingStride;
    }
    else if (keys[1] == 1) {
        // s
        ex -= cosf(DEG2RAD(azimuth)) * WalkingStride;
        ey -= sinf(DEG2RAD(azimuth)) * WalkingStride;
    }
    
    if (keys[2] == 1)
    {
        // a
        ex -= sinf(DEG2RAD(azimuth)) * WalkingStride;
        ey += cosf(DEG2RAD(azimuth)) * WalkingStride;
    }
    else if (keys[3] == 1)
    {
        // d
        ex += sinf(DEG2RAD(azimuth)) * WalkingStride;
        ey -= cosf(DEG2RAD(azimuth)) * WalkingStride;
    }
    if (keys[5] == 1 && ez == FLOOR)
    {
        keys[5] = 0;
        vz = JUMP_VELOCITY + (-9.8f /* m/s^2 */ * elapsed);
        ez += vz * elapsed;
    }
    else {
        keys[5] = 0;
        if (ez > FLOOR) {
            // accelerate
            vz += ACCEL_GRAVITY * elapsed;
            ez += vz * elapsed;
        }
        if (ez < FLOOR) {
            vz = 0.0f;
            ez = FLOOR;
        }
    }

    float azimuth_in_radians = DEG2RAD(azimuth);

    UniformBufferObjectAlt1 ubo{};
    //ubo.model = glm::rotate(
        //glm::mat4(1.0f), 
        //time * glm::radians(90.0f), 
        //glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.model = glm::mat4(1.0f);
    
    // eye, center, up
    float sinfElevation = sinf(DEG2RAD(elevation));
    sinfElevation = FCLAMP(sinfElevation, -0.99f, 0.99f);
    float invSinfElevation = 1.0f - fabs(sinfElevation);
    ubo.view = glm::lookAt(
        glm::vec3(ex, ey, ez),
        glm::vec3(
            ex + (cosf(azimuth_in_radians) * invSinfElevation), 
            ey + (sinf(azimuth_in_radians) * invSinfElevation),
            ez + sinfElevation),
        glm::vec3(0.0f, 0.0f, 1.0f));

    ubo.proj = glm::perspective(
        glm::radians(45.0f), 
        swapChainExtent.width / (float)swapChainExtent.height, 
        0.01f, 
        100.0f);
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
}

void HelloTriangleApplication::drawFrame(float elapsed)
{

    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, 
        imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    updateUniformBuffer(imageIndex, elapsed);

    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
