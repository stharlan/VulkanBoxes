
#pragma comment(lib, "C:\\VulkanSDK\\1.2.154.1\\Lib\\vulkan-1.lib")

#define VK_USE_PLATFORM_WIN32_KHR

#include <Windows.h>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <vector>
#include <set>
#include <stdexcept>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct _VERTEX
{
	glm::vec3 position;
	glm::vec2 texc;
	glm::vec3 norm;
} VERTEX;

typedef struct _VBASIC_CONTEXT
{
	VkInstance vkInstance = NULL;
	VkSurfaceKHR vkSurface = NULL;
	VkPhysicalDevice vkPhysicalDev = NULL;
	VkDevice vkDevice = NULL;
	VkQueue vkGraphicsQueue = NULL;
	VkQueue vkPresentQueue = NULL;
	int32_t graphicsFamilyIndex = -1;
	int32_t presentFamilyIndex = -1;
	VkSwapchainKHR vkSwapChain = NULL;
	std::vector<VkImage> vkSwapChainImages;
	VkFormat vkSwapChainImageFormat;
	VkExtent2D vkSwapChainExtent;
	std::vector<VkImageView> vkSwapChainImageViews;
	VkShaderModule vkVshader = nullptr;
	VkShaderModule vkFshader = nullptr;
	VkBuffer vkBuffer = nullptr;
	VkDeviceMemory vkBufferMemory = nullptr;
	VkPipelineLayout vkPipelineLayout = nullptr;
	VkRenderPass vkRenderPass = nullptr;
	VkPipeline vkGraphicsPipeline;
} VBASIC_CONTEXT;

LRESULT CALLBACK WindowProc(
	HWND   hwnd,
	UINT   uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

HWND vbasic_create_window(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = L"VBASIC";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	RegisterClass(&wc);

	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		L"VBASIC",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, hInstance, 0);
	return hwnd;
}

void vbasic_begin_message_loop(HWND hwnd)
{
	BOOL bRet;
	MSG msg;

	while ((bRet = GetMessage(&msg, hwnd, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void vbasic_create_vulkan_instance(VBASIC_CONTEXT* lpctx, HWND hwnd, HINSTANCE hInst)
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "VBASIC";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "VBASIC";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	std::vector<const char*> enabledExtensions = { 
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME 
	};

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
	createInfo.ppEnabledExtensionNames = enabledExtensions.data();
	createInfo.enabledLayerCount = 0;
	if (vkCreateInstance(&createInfo, nullptr, &lpctx->vkInstance) != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Failed to create vulkan instance");
	}
	
	printf("Vulkan instance created\n");

	VkWin32SurfaceCreateInfoKHR surfCreateInfo{};
	surfCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfCreateInfo.hwnd = hwnd;
	surfCreateInfo.hinstance = hInst;

	if (vkCreateWin32SurfaceKHR(lpctx->vkInstance, &surfCreateInfo, nullptr, &lpctx->vkSurface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

void vbasic_cleanup(VBASIC_CONTEXT* lpctx)
{
	if (lpctx->vkBuffer) vkDestroyBuffer(lpctx->vkDevice, lpctx->vkBuffer, nullptr);
	if (lpctx->vkBufferMemory) vkFreeMemory(lpctx->vkDevice, lpctx->vkBufferMemory, nullptr);
	if (lpctx->vkGraphicsPipeline) vkDestroyPipeline(lpctx->vkDevice, lpctx->vkGraphicsPipeline, nullptr);
	if (lpctx->vkPipelineLayout) vkDestroyPipelineLayout(lpctx->vkDevice, lpctx->vkPipelineLayout, nullptr);
	if (lpctx->vkRenderPass) vkDestroyRenderPass(lpctx->vkDevice, lpctx->vkRenderPass, nullptr);
	if (lpctx->vkVshader) vkDestroyShaderModule(lpctx->vkDevice, lpctx->vkVshader, nullptr);
	if (lpctx->vkFshader) vkDestroyShaderModule(lpctx->vkDevice, lpctx->vkFshader, nullptr);
	for (auto imageView : lpctx->vkSwapChainImageViews) {
		vkDestroyImageView(lpctx->vkDevice, imageView, nullptr);
	}
	if (lpctx->vkSwapChain) vkDestroySwapchainKHR(lpctx->vkDevice, lpctx->vkSwapChain, nullptr);
	if (lpctx->vkSurface) vkDestroySurfaceKHR(lpctx->vkInstance, lpctx->vkSurface, nullptr);
	if (lpctx->vkDevice) vkDestroyDevice(lpctx->vkDevice, nullptr);
	if (lpctx->vkInstance) vkDestroyInstance(lpctx->vkInstance, nullptr);
}

void vbasic_select_physical_device(VBASIC_CONTEXT* lpctx)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(lpctx->vkInstance, &deviceCount, nullptr);
	printf("%i devices\n", deviceCount);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(lpctx->vkInstance, &deviceCount, devices.data());
	for (const auto& device : devices) {
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		printf("%s\n", deviceProperties.deviceName);
		if (strcmp("GeForce GTX 1660 Ti", deviceProperties.deviceName) == 0) {
			printf("found a device\n");
			lpctx->vkPhysicalDev = device;
			return;
		}
	}
}

void vbasic_create_logical_device(VBASIC_CONTEXT* lpctx)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(lpctx->vkPhysicalDev, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(lpctx->vkPhysicalDev, &queueFamilyCount, queueFamilies.data());

	uint32_t ii = 0;
	VkBool32 supported = VK_FALSE;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			lpctx->graphicsFamilyIndex = ii;
		}
		vkGetPhysicalDeviceSurfaceSupportKHR(lpctx->vkPhysicalDev, ii, lpctx->vkSurface, &supported);
		if (VK_TRUE == supported) {
			lpctx->presentFamilyIndex = ii;
		}
		ii++;
	}

	if (lpctx->graphicsFamilyIndex == -1 || lpctx->presentFamilyIndex == -1) {
		throw std::runtime_error("Failed to get family indices");
	}

	printf("found queues %i %i\n", lpctx->graphicsFamilyIndex, lpctx->presentFamilyIndex);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { (uint32_t)lpctx->graphicsFamilyIndex, (uint32_t)lpctx->presentFamilyIndex };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	createInfo.enabledLayerCount = 0;

	if (vkCreateDevice(lpctx->vkPhysicalDev, &createInfo, nullptr, &lpctx->vkDevice) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}
	vkGetDeviceQueue(lpctx->vkDevice, lpctx->graphicsFamilyIndex, 0, &lpctx->vkGraphicsQueue);
	vkGetDeviceQueue(lpctx->vkDevice, lpctx->presentFamilyIndex, 0, &lpctx->vkPresentQueue);

	printf("logical device created; queues %lli %lli\n", 
		(long long)lpctx->vkGraphicsQueue, 
		(long long)lpctx->vkPresentQueue);

}

void vbasic_create_swapchain(VBASIC_CONTEXT* lpctx)
{
	VkSurfaceCapabilitiesKHR caps;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(lpctx->vkPhysicalDev, lpctx->vkSurface, &caps);
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(lpctx->vkPhysicalDev, lpctx->vkSurface, &formatCount, nullptr);
	if (formatCount != 0) {
		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(lpctx->vkPhysicalDev, lpctx->vkSurface, &formatCount, formats.data());
	}
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(lpctx->vkPhysicalDev, lpctx->vkSurface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(lpctx->vkPhysicalDev, lpctx->vkSurface, &presentModeCount, presentModes.data());
	}

	// color space VK_COLOR_SPACE_SRGB_NONLINEAR_KHR is 0
	// VK_FORMAT_B8G8R8A8_UNORM
	// VK_FORMAT_B8G8R8A8_SRGB
	// VK_FORMAT_A2B10G10R10_UNORM_PACK32
	for (const auto& format : formats)
	{
		printf("%i / %i\n", format.format, format.colorSpace);
	}

	// VK_PRESENT_MODE_IMMEDIATE_KHR = 0,
	// VK_PRESENT_MODE_MAILBOX_KHR = 1, triple buffering
	// VK_PRESENT_MODE_FIFO_KHR = 2,
	// VK_PRESENT_MODE_FIFO_RELAXED_KHR = 3,
	for (const auto& mode : presentModes)
	{
		printf("mode %i\n", mode);
	}

	printf("min w/h: %i %i  max w/h: %i %i\n",
		caps.minImageExtent.width, caps.minImageExtent.height,
		caps.maxImageExtent.width, caps.maxImageExtent.height);
	// 2 to 8: want 3
	printf("min img ct %i to %i\n", caps.minImageCount, caps.maxImageCount);

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = lpctx->vkSurface;
	createInfo.minImageCount = 3;
	createInfo.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
	createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	createInfo.imageExtent = caps.maxImageExtent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	if (lpctx->graphicsFamilyIndex != lpctx->presentFamilyIndex) {
		printf("queues are different\n");
		uint32_t indices[] = { (uint32_t)lpctx->graphicsFamilyIndex,(uint32_t)lpctx->presentFamilyIndex};
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = indices;
	}
	else {
		printf("queues are same\n");
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}
	createInfo.preTransform = caps.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(lpctx->vkDevice, &createInfo, nullptr, &lpctx->vkSwapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	printf("swap chain is ready\n");

	uint32_t imageCount = 0;
	vkGetSwapchainImagesKHR(lpctx->vkDevice, lpctx->vkSwapChain, &imageCount, nullptr);
	lpctx->vkSwapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(lpctx->vkDevice, lpctx->vkSwapChain, &imageCount, lpctx->vkSwapChainImages.data());

	lpctx->vkSwapChainImageFormat = VK_FORMAT_B8G8R8A8_SRGB;
	lpctx->vkSwapChainExtent = caps.maxImageExtent;

	printf("got %i swap chain images\n", imageCount);
}

void vbasic_create_swapchain_image_views(VBASIC_CONTEXT* lpctx)
{
	lpctx->vkSwapChainImageViews.resize(lpctx->vkSwapChainImages.size());
	for (size_t i = 0; i < lpctx->vkSwapChainImages.size(); i++) 
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = lpctx->vkSwapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = lpctx->vkSwapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		if (vkCreateImageView(lpctx->vkDevice, &createInfo, nullptr, &lpctx->vkSwapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
	}

	printf("created all swapchain image views\n");
}

VkShaderModule getShaderFromFile(VBASIC_CONTEXT* lpctx, const char* fn)
{
	std::vector<BYTE> shaderCode;
	FILE* f = nullptr;
	fopen_s(&f, fn, "rb");
	if (f) {
		fseek(f, 0, SEEK_END);
		long len = ftell(f);
		shaderCode.resize(len);
		fseek(f, 0, SEEK_SET);
		fread(shaderCode.data(), len, 1, f);
		fclose(f);
		printf("read %lli bytes from %s\n", shaderCode.size(), fn);
	}
	else {
		throw std::runtime_error("failed to open shader file!");
	}

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderCode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
	VkShaderModule shaderModule;
	if (vkCreateShaderModule(lpctx->vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
	return shaderModule;
}

void vbasic_create_graphics_pipeline(VBASIC_CONTEXT* lpctx)
{
	// *******
	// shaders
	// *******
	std::vector<BYTE> vertexShader;
	std::vector<BYTE> fragmentShader;
	lpctx->vkVshader = getShaderFromFile(lpctx, "vbasic.spv");
	lpctx->vkFshader = getShaderFromFile(lpctx, "fbasic.spv");
	printf("shaders are loaded\n");

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = lpctx->vkVshader;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = lpctx->vkFshader;
	fragShaderStageInfo.pName = "main";

	// programmable pipeline stages
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	printf("shaders are loaded\n");

	// *****************
	// vertex descriptor
	// *****************
	// specifically for VERTEX structure
	// glm::vec3 pos
	// glm::vec2 tex
	// glm::vec3 norm
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(VERTEX);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	VkVertexInputAttributeDescription attrs[3];
	// position
	attrs[0].binding = 0;
	attrs[0].location = 0;
	attrs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attrs[0].offset = offsetof(VERTEX, position);
	// tex coords
	attrs[1].binding = 0;
	attrs[1].location = 1;
	attrs[1].format = VK_FORMAT_R32G32_SFLOAT;
	attrs[1].offset = offsetof(VERTEX, position);
	// normal
	attrs[2].binding = 0;
	attrs[2].location = 2;
	attrs[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attrs[2].offset = offsetof(VERTEX, position);

	printf("vertices are described\n");

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = 3;
	vertexInputInfo.pVertexAttributeDescriptions = &attrs[0];

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)lpctx->vkSwapChainExtent.width;
	viewport.height = (float)lpctx->vkSwapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = lpctx->vkSwapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL; // line mode draw lines
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(lpctx->vkDevice, &pipelineLayoutInfo, nullptr, &lpctx->vkPipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	printf("pipeline layout is created\n");


	// ********
	// pipeline
	// ********
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	pipelineInfo.layout = lpctx->vkPipelineLayout;
	pipelineInfo.renderPass = lpctx->vkRenderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional
	if (vkCreateGraphicsPipelines(lpctx->vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &lpctx->vkGraphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	printf("graphics pipeline is created\n");

	// !! start here with a new method

	// ******
	// buffer
	// ******
	VERTEX triangle[] = {
		{{-0.5f,0.0f,-0.5f},{0.0f,0.0f},{0.0f,1.0f,0.0f}},
		{{0.5f,0.0f,-0.5f},{1.0f,0.0f},{0.0f,1.0f,0.0f}},
		{{0.0f,0.0f,0.5f},{0.5f,1.0f},{0.0f,1.0f,0.0f}}
	};

	// create buffer
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(VERTEX) * 3;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	if (vkCreateBuffer(lpctx->vkDevice, &bufferInfo, nullptr, &lpctx->vkBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer!");
	}

	// get memory requirements
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(lpctx->vkDevice, lpctx->vkBuffer, &memRequirements);
	printf("s: %lli, a: %lli, b: %i\n",
		memRequirements.size,
		memRequirements.alignment,
		memRequirements.memoryTypeBits);

	// find memory type
	uint32_t typeFilter = memRequirements.memoryTypeBits;
	VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	VkPhysicalDeviceMemoryProperties memProperties;
	uint32_t memoryTypeIndex = 0;
	bool isFound = false;
	vkGetPhysicalDeviceMemoryProperties(lpctx->vkPhysicalDev, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			memoryTypeIndex = i;
			isFound = true;
		}
	}

	if(false == isFound) 
		throw std::runtime_error("failed to find suitable memory type!");

	// allocate memory
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = memoryTypeIndex;

	if (vkAllocateMemory(lpctx->vkDevice, &allocInfo, nullptr, &lpctx->vkBufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	// associate buffer with memory
	vkBindBufferMemory(lpctx->vkDevice, lpctx->vkBuffer, lpctx->vkBufferMemory, 0);

	printf("buffer is created and associated with memory\n");

	// copy data into buffer
	void* data;
	vkMapMemory(lpctx->vkDevice, lpctx->vkBufferMemory, 0, bufferInfo.size, 0, &data);
	memcpy(data, &triangle[0], (size_t)bufferInfo.size);
	vkUnmapMemory(lpctx->vkDevice, lpctx->vkBufferMemory); 

	printf("buffer is loaded\n");
}

void vbasic_create_render_pass(VBASIC_CONTEXT* lpctx)
{
	// ***********
	// render pass
	// ***********
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = lpctx->vkSwapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	if (vkCreateRenderPass(lpctx->vkDevice, &renderPassInfo, nullptr, &lpctx->vkRenderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}

	printf("render pass is created\n");

}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PSTR szCmdLine, 
	_In_ int iCmdShow)
{

	AllocConsole();
	FILE* f = nullptr;
	freopen_s(&f, "CONIN$", "r", stdin);
	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONOUT$", "w", stderr);

	HWND hwnd = vbasic_create_window(hInstance);
	ShowWindow(hwnd, SW_SHOW);

	VBASIC_CONTEXT ctx{};
	try {
		vbasic_create_vulkan_instance(&ctx, hwnd, hInstance);
		vbasic_select_physical_device(&ctx);
		vbasic_create_logical_device(&ctx);
		vbasic_create_swapchain(&ctx);
		vbasic_create_swapchain_image_views(&ctx);
		vbasic_create_render_pass(&ctx);

		vbasic_create_graphics_pipeline(&ctx);
	}
	catch (const std::exception& e) {
		printf("%s\n", e.what());
		vbasic_cleanup(&ctx);
		getchar();
		return EXIT_FAILURE;
	}

	vbasic_begin_message_loop(hwnd);

	vbasic_cleanup(&ctx);

	FreeConsole();

	return EXIT_SUCCESS;
}