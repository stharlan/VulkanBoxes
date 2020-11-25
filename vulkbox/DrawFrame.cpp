
#include "Common.h"

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
    createColorResources();
    createDepthResources();
    createFramebuffers();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
}

//const float FLOOR = 1.0f;
const float JUMP_VELOCITY = 5.0f;
const float ACCEL_GRAVITY = -9.8f;
// TODO need to make speed dependent on frame rate
const float SPEED = 20.0f;

bool ActorsAdded = false;
std::vector<physx::PxActor*> blocksAroundMe;

void HelloTriangleApplication::addActorsForCurrentLocation(int64_t xint, int64_t yint, int64_t zint)
{

    // create all the rigid static actors
    // as an array in the crate vertex buffer algo
    // then, just pull them out of the array in this method

    //printf("location has changed; add new actors\n");

    // remove current actors
    if (blocksAroundMe.size() > 0) {
        this->mScene->removeActors(blocksAroundMe.data(), blocksAroundMe.size());
    }

    // release actors
    std::vector<physx::PxActor*>::iterator iter = blocksAroundMe.begin();
    for (; iter != blocksAroundMe.end(); ++iter)
    {
        (*iter)->release();
    }

    // clear array
    blocksAroundMe.clear();

    for (int64_t xbl = xint - 4; xbl < xint + 5; xbl++)
    {
        if (xbl >= 0 && xbl < x_extent)
        {
            for (int64_t ybl = yint - 4; ybl < yint + 5; ybl++)
            {
                if (ybl >= 0 && ybl < y_extent)
                {
                    for (int64_t zbl = zint - 8; zbl < zint + 1; zbl++)
                    {
                        if (zbl >= 0 && zbl < z_extent)
                        {
                            int64_t idx = (zbl * x_extent * y_extent) + (ybl * x_extent) + xbl;
                            if (blockArray[idx] == 1) {
                                // create new actors
                                physx::PxRigidStatic* block =
                                    this->mPhysics->createRigidStatic(
                                        physx::PxTransform(xbl + 0.5f, ybl + 0.5f, zbl + 0.5f));
                                block->attachShape(*this->mBlockShape);
                                blocksAroundMe.push_back(block);
                            }
                        }
                    }
                }
            }
        }
    }

    //printf("adding %lli actors\n", blocksAroundMe.size());

    this->mScene->addActors(blocksAroundMe.data(), blocksAroundMe.size());

    xblock = xint;
    yblock = yint;
    zblock = zint;
}

void HelloTriangleApplication::updateUniformBufferWithPhysics(uint32_t currentImage, float elapsed)
{

    // rebuild
    if (ActorsAdded == false) {
        this->addActorsForCurrentLocation((int)ex, (int)ey, (int)ez);
        ActorsAdded = true;
    }

    float mx = 0.0f;
    float my = 0.0f;

    if (keys[0] == 1) {
        mx += cosf(DEG2RAD(azimuth)) / SPEED;
        my += sinf(DEG2RAD(azimuth)) / SPEED;
    }
    else if (keys[1] == 1)
    {
        mx += cosf(DEG2RAD(azimuth)) / -SPEED;
        my += sinf(DEG2RAD(azimuth)) / -SPEED;
    }
    if (keys[2] == 1)
    {
        mx += sinf(DEG2RAD(azimuth)) / -SPEED;
        my += cosf(DEG2RAD(azimuth)) / SPEED;
    }
    else if (keys[3] == 1)
    {
        mx += sinf(DEG2RAD(azimuth)) / SPEED;
        my += cosf(DEG2RAD(azimuth)) / -SPEED;
    }

    if (keys[4] == 1)
    {
        vz = 10.0f;
        //printf("jump\n");
        keys[4] = 0;
        //this->mPlayerCapsuleActor->addForce(physx::PxVec3(0, 0, 20), physx::PxForceMode::eIMPULSE);
    }
    else {
        vz += -20.0f * elapsed;
    }

    // physics
    physx::PxExtendedVec3 posb = this->mController->getPosition();
    this->mController->move(
        physx::PxVec3(mx, my, vz * elapsed),
        0.00f, // min dist
        elapsed, this->mCCFilters);
    this->mScene->simulate(elapsed);
    this->mScene->fetchResults(true);

    physx::PxExtendedVec3 pos = this->mController->getPosition();

    if (abs(xblock - (int64_t)pos.x) > 4) {
        this->addActorsForCurrentLocation((int64_t)pos.x, (int64_t)pos.y, (int64_t)pos.z);
    }
    else if (abs(yblock - (int64_t)pos.y) > 4) {
        this->addActorsForCurrentLocation((int64_t)pos.x, (int64_t)pos.y, (int64_t)pos.z);
    }
    else if (abs(zblock - (int64_t)pos.z) > 4)
    {
        this->addActorsForCurrentLocation((int64_t)pos.x, (int64_t)pos.y, (int64_t)pos.z);
    }

    //printf("%.4f %.4f %.4f %.4f\n", pos.x, pos.y, pos.z, vz);

    float sinfElevation = sinf(DEG2RAD(elevation));
    sinfElevation = FCLAMP(sinfElevation, -0.99f, 0.99f);
    float invSinfElevation = 1.0f - fabs(sinfElevation);
    glm::vec3 glmpos(pos.x, pos.y, pos.z);
    glm::vec3 glmposlook(
        pos.x + (cosf(DEG2RAD(azimuth)) * invSinfElevation),
        pos.y + (sinf(DEG2RAD(azimuth)) * invSinfElevation),
        pos.z + sinfElevation);
        //pos.x + cosf(DEG2RAD(azimuth)),
        //pos.y + sinf(DEG2RAD(azimuth)),
        //pos.z);
    //,
        //ey + ,
        //ez + 1 + sinfElevation),

    UniformBufferObjectAlt2 ubo{};
    //ubo.model = glm::rotate(
        //glm::mat4(1.0f), 
        //time * glm::radians(90.0f), 
        //glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.model = glm::mat4(1.0f);

    // eye, center, up
    ubo.view = glm::lookAt(
        glmpos,
        glmposlook,
        glm::vec3(0.0f, 0.0f, 1.0f));

    ubo.proj = glm::perspective(
        glm::radians(45.0f),
        swapChainExtent.width / (float)swapChainExtent.height,
        0.01f,
        500.0f);
    ubo.proj[1][1] *= -1;

    ubo.upos = glm::vec4(pos.x, pos.y, pos.z, 1.0);

    void* data;
    vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
}

//void HelloTriangleApplication::updateUniformBufferWithPhysics(uint32_t currentImage, float elapsed)
//{
//    reactphysics3d::Vector3 vel = this->player->getLinearVelocity();
//    //float vx = vel.x;
//    //float vy = vel.y;
//    //float xyvel = sqrtf((vx * vx) + (vy * vy));
//    //if (isnan(xyvel)) xyvel = 0.0f;
//    //printf("%.1f, %.1f, %.1f : lin vel %.2f\n", vel.x, vel.y, vel.z, xyvel);
//    if (keys[0] == 1) {
//        reactphysics3d::Vector3 force(
//            20.0f * cosf(DEG2RAD(azimuth)),
//            20.0f * sinf(DEG2RAD(azimuth)),
//            0);
//        this->player->applyForceToCenterOfMass(force);
//        //reactphysics3d::Vector3 go(
//            //10.0f * cosf(DEG2RAD(azimuth)),
//            //10.0f * sinf(DEG2RAD(azimuth)),
//            //vel.z);
//        //this->player->setLinearVelocity(go);
//    }
//    else {
//        //reactphysics3d::Vector3 stop(0, 0, vel.z);
//        //this->player->setLinearVelocity(stop);
//    }
//
//    // physics
//    this->world->update(elapsed);
//
//    const reactphysics3d::Transform& transform = this->player->getTransform();
//    const reactphysics3d::Vector3& position = transform.getPosition();
//
//    const reactphysics3d::Transform& btransform = this->blocks[1]->getTransform();
//    const reactphysics3d::Vector3& bposition = btransform.getPosition();
//
//    //printf("pos %.2f, %.2f, %.2f; block0 %.2f, %.2f, %.2f\n",
//        //position.x, position.y, position.z,
//        //bposition.x, bposition.y, bposition.z);
//    glm::vec3 glmpos(position.x, position.y, position.z);
//    glm::vec3 glmposlook(
//        position.x + cosf(DEG2RAD(azimuth)), 
//        position.y + sinf(DEG2RAD(azimuth)), 
//        position.z);
//
//    UniformBufferObjectAlt2 ubo{};
//    //ubo.model = glm::rotate(
//        //glm::mat4(1.0f), 
//        //time * glm::radians(90.0f), 
//        //glm::vec3(0.0f, 0.0f, 1.0f));
//    ubo.model = glm::mat4(1.0f);
//
//    // eye, center, up
//    ubo.view = glm::lookAt(
//        glmpos,
//        glmposlook,
//        glm::vec3(0.0f, 0.0f, 1.0f));
//
//    ubo.proj = glm::perspective(
//        glm::radians(45.0f),
//        swapChainExtent.width / (float)swapChainExtent.height,
//        0.01f,
//        100.0f);
//    ubo.proj[1][1] *= -1;
//
//    ubo.upos = glm::vec4(position.x, position.y, position.z, 1.0);
//
//    void* data;
//    vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
//    memcpy(data, &ubo, sizeof(ubo));
//    vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
//}

void HelloTriangleApplication::updateUniformBuffer(uint32_t currentImage, float elapsed)
{
    //static auto startTime = std::chrono::high_resolution_clock::now();
    
    //auto currentTime = std::chrono::high_resolution_clock::now();
    //float time = 
        //std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime)
        //.count();

    // physics
    //this->world->update(elapsed);
    //const reactphysics3d::Transform& transform = this->body->getTransform();
    //const reactphysics3d::Vector3& position = transform.getPosition();
    //printf("pos %.2f, %.2f, %.2f\n", position.x, position.y, position.z);

    float mx = 0.0f;
    float my = 0.0f;

    if (keys[0] == 1) {
        // w
        mx += cosf(DEG2RAD(azimuth)) * WalkingStride;
        my += sinf(DEG2RAD(azimuth)) * WalkingStride;
    }
    else if (keys[1] == 1) {
        // s
        mx += -1.0f * cosf(DEG2RAD(azimuth)) * WalkingStride;
        my += -1.0f * sinf(DEG2RAD(azimuth)) * WalkingStride;
    }
    
    if (keys[2] == 1)
    {
        // a
        mx += -1.0f * sinf(DEG2RAD(azimuth)) * WalkingStride;
        my += cosf(DEG2RAD(azimuth)) * WalkingStride;
    }
    else if (keys[3] == 1)
    {
        // d
        mx += sinf(DEG2RAD(azimuth)) * WalkingStride;
        my += -1.0f * cosf(DEG2RAD(azimuth)) * WalkingStride;
    }

    // floor
    int ix = (int)(ex + mx);
    int iy = (int)(ey + my);
    int iz = (int)ez;
    int64_t idx0 = 0;
    int64_t idx1 = 0;

    if (iz >= 0 && iz < z_extent) {
        for (int bx = ix - 1; bx < ix + 2; bx++) {
            if (bx >= 0 && bx < x_extent) {
                for (int by = iy - 1; by < iy + 2; by++) {
                    if (by >= 0 && by < y_extent) {
                        if (!(bx == ix && by == iy)) {
                            idx0 = (iz * x_extent * y_extent) + (by * x_extent) + bx;
                            if ((iz + 1) < z_extent) {
                                idx1 = ((iz + 1) * x_extent * y_extent) + (by * x_extent) + bx;
                            }
                            else {
                                idx1 = 0;
                            }
                            if (blockArray[idx0] == 1 || blockArray[idx1] == 1)
                            {
                                float deltax = ((float)bx + 0.5f) - (ex + mx);
                                float deltay = ((float)by + 0.5f) - (ey + my);
                                float dist = (deltax * deltax) + (deltay * deltay);
                                if (dist < 1.0f) {
                                    // too close
                                    mx = my = 0.0f;
                                    goto cant_go_there;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

cant_go_there:

    //if (ix >= 0 && ix < x_extent)
    //{
    //    if (iy >= 0 && iy < y_extent)
    //    {
    //        if (iz >= 0 && iz < z_extent)
    //        {
    //            for()


    //            int64_t idx = (iz * x_extent * y_extent) + (iy * x_extent) + ix;
    //            if (blockArray[idx] == 1) {
    //                mx = my = 0.0f;
    //            }
    //            else {
    //                idx = ((iz + 1) * x_extent * y_extent) + (iy * x_extent) + ix;
    //                if (blockArray[idx] == 1) {
    //                    mx = my = 0.0f;
    //                }
    //            }
    //        }
    //    }
    //}

    ex += mx;
    ey += my;
    ix = (int)(ex + mx);
    iy = (int)(ey + my);

    float floor = -100.0f;
    if (ix >= 0 && ix < x_extent)
    {
        if (iy >= 0 && iy < y_extent)
        {
            if (iz >= 0 && iz < z_extent) 
            {
                int64_t idx = ((iz - 1) * x_extent * y_extent) + (iy * x_extent) + ix;
                if (blockArray[idx] == 1) {
                    floor = (float)iz;
                }
            }
        }
    }

    if (keys[5] == 1 && ez == floor)
    {
        keys[5] = 0;
        vz = JUMP_VELOCITY + (-9.8f /* m/s^2 */ * elapsed);
        ez += vz * elapsed;
    }
    else {
        keys[5] = 0;
        if (ez > floor) {
            // accelerate
            vz += ACCEL_GRAVITY * elapsed;
            ez += vz * elapsed;
        }
        if (ez < floor) {
            vz = 0.0f;
            ez = floor;
        }
    }

    float azimuth_in_radians = DEG2RAD(azimuth);

    UniformBufferObjectAlt2 ubo{};
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
        glm::vec3(ex, ey, ez + 1),
        glm::vec3(
            ex + (cosf(azimuth_in_radians) * invSinfElevation), 
            ey + (sinf(azimuth_in_radians) * invSinfElevation),
            ez + 1 + sinfElevation),
        glm::vec3(0.0f, 0.0f, 1.0f));

    ubo.proj = glm::perspective(
        glm::radians(45.0f), 
        swapChainExtent.width / (float)swapChainExtent.height, 
        0.01f, 
        1000.0f);
    ubo.proj[1][1] *= -1;

    ubo.upos = glm::vec4(ex, ey, ez + 1, 1.0);

    void* data;
    vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBuffersMemory[currentImage]);

    //printf("x, y, z : %.1f, %.1f, %.1f : fl %.1f\n", ex, ey, ez, floor);
}

int physicsTripper = 0;
float accumulatedElapsed = 0.0f;

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

    //updateUniformBuffer(imageIndex, elapsed);
    updateUniformBufferWithPhysics(imageIndex, elapsed);
    //switch (physicsTripper)
    //{
    //case 0:
    //    updateUniformBufferWithPhysics(imageIndex, elapsed);
    //    physicsTripper = 1;
    //    break;
    //case 1:
    //    accumulatedElapsed = elapsed;
    //    physicsTripper = 2;
    //    break;
    //case 2:
    //    physicsTripper = 1;
    //    updateUniformBufferWithPhysics(imageIndex, accumulatedElapsed + elapsed);
    //    break;
    //}

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
