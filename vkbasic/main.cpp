
#pragma comment(lib, "C:\\VulkanSDK\\1.2.154.1\\Lib\\vulkan-1.lib")

#define VK_USE_PLATFORM_WIN32_KHR

#include <Windows.h>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <vector>
#include <stdexcept>

typedef struct _VBASIC_CONTEXT
{
	VkInstance vkInstance = NULL;
	VkPhysicalDevice vkPhysicalDev = NULL;
	VkDevice vkDevice = NULL;
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
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
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

void vbasic_create_vulkan_instance(VBASIC_CONTEXT* lpctx)
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
}

void vbasic_cleanup(VBASIC_CONTEXT* lpctx)
{
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

	int graphicsFamilyIndex = -1;

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphicsFamilyIndex = i;
		}
		i++;
	}

	if (graphicsFamilyIndex == -1) {
		throw std::runtime_error("Failed to get graphics family index");
	}

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = graphicsFamilyIndex;
	queueCreateInfo.queueCount = 1;
	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = 0;
	createInfo.enabledLayerCount = 0;

	if (vkCreateDevice(lpctx->vkPhysicalDev, &createInfo, nullptr, &lpctx->vkDevice) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	printf("logical device created\n");
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
		vbasic_create_vulkan_instance(&ctx);
		vbasic_select_physical_device(&ctx);
		vbasic_create_logical_device(&ctx);
	}
	catch (const std::exception& e) {
		printf("%s\n", e.what());
		vbasic_cleanup(&ctx);
		return EXIT_FAILURE;
	}

	vbasic_begin_message_loop(hwnd);

	vbasic_cleanup(&ctx);

	FreeConsole();

	return EXIT_SUCCESS;
}