
// TODO per fragment lighting
// https://paroj.github.io/gltut/Illumination/Tut10%20Fragment%20Lighting.html
// TODO shadow mapping
// https://blogs.igalia.com/itoral/2017/07/30/working-with-lights-and-shadows-part-ii-the-shadow-map/
// TODO fox addActorsForCurrentLocation to store all blocks in mem

// build

#define STB_IMAGE_IMPLEMENTATION
#include "Common.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#define X_GRID_EXTENT 512
#define Y_GRID_EXTENT 512
#define Z_GRID_EXTENT 256

int64_t x_extent = X_GRID_EXTENT; // 0 to extent
int64_t y_extent = Y_GRID_EXTENT; // 0 to extent
int64_t z_extent = Z_GRID_EXTENT; // 0 to extent
int8_t blockArray[X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT] = { };

// this is going to have to be a uniform
// but, for now, it can be hard coded into shader
//const std::vector<glm::vec3> transforms = {
//    {0.0f, 0.0f, 0.0f}
//};

// for now, it can be hard coded into shader
//const std::vector<Vertex1> vertices = {
//    {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
//    {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
//    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
//    {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
//    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
//    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
//    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
//};

// this will be the vertex list
// vertex is going to be two indices
// index 1 is index into vertices 0-7
// index 2 is index into transforms 0-n
std::vector<Vertex3> vertices4;
//const std::vector<Vertex3> vertices4 = {
//    {0, {0.0f, 0.0f, 0.0f} }, 
//    {1, {0.0f, 0.0f, 0.0f} },
//    {2, {0.0f, 0.0f, 0.0f} },
//    {2, {0.0f, 0.0f, 0.0f} },
//    {3, {0.0f, 0.0f, 0.0f} },
//    {0, {0.0f, 0.0f, 0.0f} },
//    {0, {1.0f, 0.0f, 0.0f} },
//    {1, {1.0f, 0.0f, 0.0f} },
//    {2, {1.0f, 0.0f, 0.0f} },
//    {2, {1.0f, 0.0f, 0.0f} },
//    {3, {1.0f, 0.0f, 0.0f} },
//    {0, {1.0f, 0.0f, 0.0f} }
//};

//const std::vector<uint16_t> indices = {
//    0, 1, 2, 2, 3, 0,
//    4, 6, 5, 6, 4, 7,
//    0, 3, 4, 3, 7, 4,
//    1, 5, 2, 2, 5, 6,
//    1, 0, 5, 5, 0, 4,
//    6, 7, 2, 2, 7, 3 
//};

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

// begin class members

void HelloTriangleApplication::initWindow() 
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwMaximizeWindow(window);
}

void HelloTriangleApplication::framebufferResizeCallback(GLFWwindow* window, int width, int height) 
{
    auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

// this is never called
bool HelloTriangleApplication::hasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format ==
        VK_FORMAT_D24_UNORM_S8_UINT;
}

void HelloTriangleApplication::initVulkan() {
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandPool();
    createColorResources();
    createDepthResources();
    createFramebuffers();
    createTextureImage(textureImage, textureImageMemory, "c:\\temp\\grassbox512tg1.png");
    createTextureImage(textureImage2, textureImageMemory2, "c:\\temp\\dirt.png");
    textureImageView = createTextureImageView(textureImage);
    textureImageView2 = createTextureImageView(textureImage2);
    createTextureSampler();
    createVertexBuffer();
    //createIndexBuffer();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
    createSyncObjects();
}

float ex = 250.0f;
float ey = 250.0f;
float ez = Z_GRID_EXTENT + 10.0f;
float vz = 0.0f;
float WalkingStride = 0.0f;
int keys[] = {0,0,0,0,0};
int64_t xblock = (int)ex;
int64_t yblock = (int)ey;
int64_t zblock = (int)ez;

void HelloTriangleApplication::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //if (key == GLFW_KEY_E && action == GLFW_PRESS) activate_airship();
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            keys[0] = 1;
            break;
        case GLFW_KEY_S:
            keys[1] = 1;
            break;
        case GLFW_KEY_A:
            keys[2] = 1;
            break;
        case GLFW_KEY_D:
            keys[3] = 1;
            break;
        case GLFW_KEY_SPACE:
            keys[4] = 1;
            printf("space\n");
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, 1);
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key) {
        case GLFW_KEY_W:
            keys[0] = 0;
            break;
        case GLFW_KEY_S:
            keys[1] = 0;
            break;
        case GLFW_KEY_A:
            keys[2] = 0;
            break;
        case GLFW_KEY_D:
            keys[3] = 0;
            break;
        }
    }
}

float azimuth = 0.0f;
float elevation = 0.0f;
double dxpos = 0;
double dypos = 0;
int initMouse = 1;
double lx = 0;
double ly = 0;

// higher is slower
float turnSpeed = 8.0f;
// higher is faster
float walkSpeed = 10.0f;

void HelloTriangleApplication::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (initMouse == 1) {
        initMouse = 0;
        lx = xpos;
        ly = ypos;
    }
    dxpos = xpos - lx;
    dypos = ypos - ly;
    lx = xpos;
    ly = ypos;
    float tempAz = azimuth - (dxpos / turnSpeed);
    if (tempAz < 0.0f) tempAz = 360.0f + tempAz;
    if (tempAz > 360.0f) tempAz = tempAz - 360.0f;
    azimuth = tempAz;
    float tempEl = elevation - (dypos / turnSpeed);
    if (tempEl > 90.0) tempEl = 90.0f;
    if (tempEl < -90.0f) tempEl = -90.0f;
    elevation = tempEl;
}

void HelloTriangleApplication::mainLoop()
{
    static LARGE_INTEGER perfCount;
    static LARGE_INTEGER perfFreq;
    LONGLONG lastCount = 0;

    QueryPerformanceCounter(&perfCount);
    lastCount = perfCount.QuadPart;
    QueryPerformanceFrequency(&perfFreq); // counts per second

    int64_t ctr = 0;
    int64_t elapsedTicks = 0;

    while (!glfwWindowShouldClose(window)) {

        QueryPerformanceCounter(&perfCount);
        elapsedTicks = perfCount.QuadPart - lastCount;
        float FramesPerSecond = (float)perfFreq.QuadPart / (float)elapsedTicks;
        if (FramesPerSecond <= 0.0f)
        {
            FramesPerSecond = 30.0f;
        }
        if (ctr++ == 100) {
            printf("FPS %.1f\n", FramesPerSecond);
            ctr = 0;
        }
        WalkingStride = walkSpeed / FramesPerSecond; 
        lastCount = perfCount.QuadPart;

        glfwPollEvents();
        drawFrame(1.0f / FramesPerSecond);
    }
    vkDeviceWaitIdle(device);
}

// end class members

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
