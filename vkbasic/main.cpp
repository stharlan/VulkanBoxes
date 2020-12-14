
#pragma comment(lib, "C:\\VulkanSDK\\1.2.154.1\\Lib\\vulkan-1.lib")

#define VK_USE_PLATFORM_WIN32_KHR

#include <Windows.h>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <vector>
#include <set>
#include <stdexcept>

typedef struct _VBASIC_CONTEXT
{
	VkInstance vkInstance = NULL;
	VkSurfaceKHR vkSurface = NULL;
	VkPhysicalDevice vkPhysicalDev = NULL;
	VkDevice vkDevice = NULL;
	VkQueue vkGraphicsQueue = NULL;
	VkQueue vkPresentQueue = NULL;
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
	else {
		printf("Vulkan instance created\n");
	}

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

	uint32_t graphicsFamilyIndex = -1;
	uint32_t presentFamilyIndex = -1;

	uint32_t ii = 0;
	VkBool32 supported = VK_FALSE;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphicsFamilyIndex = ii;
		}
		vkGetPhysicalDeviceSurfaceSupportKHR(lpctx->vkPhysicalDev, ii, lpctx->vkSurface, &supported);
		if (VK_TRUE == supported) {
			presentFamilyIndex = ii;
		}
		ii++;
	}

	if (graphicsFamilyIndex == -1 || presentFamilyIndex == -1) {
		throw std::runtime_error("Failed to get family indices");
	}

	printf("found queues %i %i\n", graphicsFamilyIndex, presentFamilyIndex);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { graphicsFamilyIndex, presentFamilyIndex };

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

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	createInfo.enabledLayerCount = 0;

	if (vkCreateDevice(lpctx->vkPhysicalDev, &createInfo, nullptr, &lpctx->vkDevice) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}
	vkGetDeviceQueue(lpctx->vkDevice, graphicsFamilyIndex, 0, &lpctx->vkGraphicsQueue);
	vkGetDeviceQueue(lpctx->vkDevice, presentFamilyIndex, 0, &lpctx->vkPresentQueue);

	printf("logical device created; queues %lli %lli\n", 
		(long long)lpctx->vkGraphicsQueue, 
		(long long)lpctx->vkPresentQueue);

}

void vbasic_create_swapchain(VBASIC_CONTEXT* lpctx)
{
	VkSurfaceCapabilitiesKHR caps;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(lpctx->vkPhysicalDev, lpctx->vkSurface, &caps);
	
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;

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