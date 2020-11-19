#pragma once

#pragma comment(lib, "C:\\VulkanSDK\\1.2.154.1\\Lib\\vulkan-1.lib")
#pragma comment(lib, "C:\\Library\\glfw-3.3.2.bin.WIN64\\lib-vc2019\\glfw3dll.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysX_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXCommon_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXFoundation_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXExtensions_static_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "C:\\Users\\stuar\\source\\repos\\PhysX\\physx\\bin\\win.x86_64.vc142.mt\\profile\\PhysXCharacterKinematic_static_64.lib")

#include <Windows.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PVD_HOST "127.0.0.1"
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include <PxFoundation.h>
#include <PxPhysicsVersion.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <extensions/PxDefaultAllocator.h>
#include <pvd/PxPvd.h>
#include <pvd/PxPvdTransport.h>
#include <characterkinematic/PxControllerManager.h>
#include <characterkinematic/PxCapsuleController.h>


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
