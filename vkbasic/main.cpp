
#pragma comment(lib, "C:\\VulkanSDK\\1.2.154.1\\Lib\\vulkan-1.lib")

#define VK_USE_PLATFORM_WIN32_KHR

#include <Windows.h>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <vector>

typedef struct _VBASIC_CONTEXT
{
	VkInstance vkInstance = NULL;
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
		printf("ERROR: Failed to create vulkan instance\n");
	}
	else {
		printf("Vulkan instance created\n");
	}
}

void vbasic_cleanup(VBASIC_CONTEXT* lpctx)
{
	if (lpctx->vkInstance) vkDestroyInstance(lpctx->vkInstance, nullptr);
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
	vbasic_create_vulkan_instance(&ctx);

	vbasic_begin_message_loop(hwnd);

	vbasic_cleanup(&ctx);

	FreeConsole();

	return 0;
}