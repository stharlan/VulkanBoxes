
#define DEG2RAD(d) (d * 3.14159f / 180.0f)

#define FCLAMP(v,i,x) (v < i ? i : (v > x ? x : v))

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

// vertex is going to be two indices
// index 1 is index into vertices 0-7
// index 2 is index into transforms 0-n
struct Vertex4 {
    uint32_t idx;
    glm::vec3 trans;
    float texoffset;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex4);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32_UINT;
        attributeDescriptions[0].offset = offsetof(Vertex4, idx);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex4, trans);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex4, texoffset);

        return attributeDescriptions;
    }
};

//struct Vertex1 {
//    glm::vec3 pos1;
//    glm::vec3 color;
//    glm::vec2 texCoord;
//
//    static VkVertexInputBindingDescription getBindingDescription() {
//        VkVertexInputBindingDescription bindingDescription{};
//        bindingDescription.binding = 0;
//        bindingDescription.stride = sizeof(Vertex1);
//        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
//
//        return bindingDescription;
//    }
//
//    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
//        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
//
//        attributeDescriptions[0].binding = 0;
//        attributeDescriptions[0].location = 0;
//        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
//        attributeDescriptions[0].offset = offsetof(Vertex1, pos1);
//
//        attributeDescriptions[1].binding = 0;
//        attributeDescriptions[1].location = 1;
//        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
//        attributeDescriptions[1].offset = offsetof(Vertex1, color);
//
//        attributeDescriptions[2].binding = 0;
//        attributeDescriptions[2].location = 2;
//        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
//        attributeDescriptions[2].offset = offsetof(Vertex1, texCoord);
//
//        return attributeDescriptions;
//    }
//};

struct UniformBufferObjectAlt2 {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec4 upos;
    glm::vec4 verts[40];
    glm::vec4 texcs[40];
    glm::vec4 norms[40];
};

extern const bool enableValidationLayers;

extern const std::vector<const char*> validationLayers;

extern const std::vector<const char*> deviceExtensions;

//extern const std::vector<Vertex1> vertices;

extern std::vector<Vertex4> vertices4;

//extern const std::vector<uint16_t> indices;

extern const int MAX_FRAMES_IN_FLIGHT;

extern float ex;
extern float ey;
extern float ez;
extern float azimuth;
extern float elevation;
extern float dxpos;
extern float dypos;
extern int keys[];
extern float WalkingStride;
extern float vz;
extern int64_t xblock;
extern int64_t yblock;
extern int64_t zblock;

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initPhysics();
        initVulkan();
        mainLoop();
        cleanupPhysics();
        cleanup();
    }

    HelloTriangleApplication()
    {
        //int8_t* pBlockArray; // [X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT] ;
        //physx::PxRigidStatic** pStaticBlockArray; // [X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT] ;
        this->pBlockArray = (int8_t*)malloc(X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT * sizeof(int8_t));
        this->pStaticBlockArray = (physx::PxRigidStatic**)malloc(X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT * sizeof(physx::PxRigidStatic*));
    }

    ~HelloTriangleApplication()
    {
        free(this->pBlockArray);
        this->pBlockArray = NULL;
        //for (int64_t xc = 0; xc < this->x_extent; xc++) {
        //    for (int64_t yc = 0; yc < this->y_extent; yc++) {
        //        for (int64_t zc = 0; zc < this->z_extent; zc++) {
        //            int64_t idx = GRIDIDX(xc, yc, zc);
        //            if (this->pStaticBlockArray[idx]) this->pStaticBlockArray[idx]->release();
        //        }
        //    }
        //}
        free(this->pStaticBlockArray);
        this->pStaticBlockArray = NULL;
    }

private:
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    void initPhysics();
    void cleanupPhysics();
    void addBlockRigidBody(float bx, float by, float bz);
    void cleanupSwapChain();
    void recreateSwapChain();
    void createInstance();
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    VkFormat findSupportedFormat(const std::vector<VkFormat>&
        candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat();
    bool hasStencilComponent(VkFormat format);
    void createDepthResources();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createTextureImage(VkImage& vkImg, VkDeviceMemory& vkDevMem, const char* filename);
    VkImageView createTextureImageView(VkImage vkImg);
    void createTextureSampler();
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
    void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void createVertexBuffer();
    //void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createCommandBuffers();
    void createSyncObjects();
    //void updateUniformBuffer(uint32_t currentImage, float elapsed);
    void updateUniformBufferWithPhysics(uint32_t currentImage, float elapsed);
    void drawFrame(float elapsed);
    VkShaderModule createShaderModule(const std::vector<char>& code);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    static std::vector<char> readFile(const std::string& filename);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    void addActorsForCurrentLocation(int64_t xint, int64_t yint, int64_t zint);
    void createColorResources();
    void createShadowMappingResources();

private:
    GLFWwindow* window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;

    VkImage textureImage;
    VkImage textureImage2;

    VkDeviceMemory textureImageMemory;
    VkDeviceMemory textureImageMemory2;

    VkImageView textureImageView;
    VkImageView textureImageView2;

    VkSampler textureSampler;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    bool framebufferResized = false;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_8_BIT;
    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    VkImage shadowImage = {};
    VkImageView shadowMapView = {};
    VkDeviceMemory shadowImageMemory = {};

    physx::PxFoundation* mFoundation = NULL;
    physx::PxPvd* mPvd = NULL;
    physx::PxPhysics* mPhysics = NULL;
    physx::PxDefaultCpuDispatcher* mDispatcher = NULL;
    physx::PxScene* mScene = NULL;
    physx::PxRigidDynamic* mPlayerCapsuleActor = NULL;
    //physx::PxShape* mPlayerCapsuleShape = NULL;
    physx::PxShape* mBlockShape = NULL;
    //std::vector<physx::PxRigidStatic*> blocks;
    physx::PxControllerManager* mManager = NULL;
    physx::PxMaterial* pMaterial = NULL;
    physx::PxCapsuleController* mController = NULL;
    physx::PxControllerFilters mCCFilters = {};

    int64_t x_extent = X_GRID_EXTENT; // 0 to extent
    int64_t y_extent = Y_GRID_EXTENT; // 0 to extent
    int64_t z_extent = Z_GRID_EXTENT; // 0 to extent
    int8_t* pBlockArray; // [X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT] ;
    physx::PxRigidStatic** pStaticBlockArray; // [X_GRID_EXTENT * Y_GRID_EXTENT * Z_GRID_EXTENT] ;

};
