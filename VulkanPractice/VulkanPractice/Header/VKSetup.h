//
//  VKSetup.h
//  VulkanPractice
//
//  Created by Anudeep on 26/04/23.
//

/**
 Phase 1: Setup
 1. Check Validation Layer support
 2. Create Vulkan instance  with debug message
 3. Setup the debug messenger
 4. Pick a physical device, check if it has valid family queues
 5. Create logical device to interact with physical device and fetch family queue details
 
 */

/**
 Phase 2: Presentation
 6. Create a window surface to interact with the window and create present queue while creating
 logical device to present to created window surface
 7. Check for swap chain support explicitly, but anyway if present queue is created then swap chain must be supported by the device:
    i. Query for swap chain details i.e., Basic surface capabilities, Surface formats and Available presentation modes
    ii. Choose the right setting for the swap chain
    iii. Finally create a swap chain
    iv. Retrieve swap chain images
 8. Create ImageView for every Image, ImageView describes how to access the image and which part of the image to access it
 
 */

/**
 Phase 3: Graphics Pipeline
 1. Refer https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Introduction for basics
 2. Setup Vertax shader and Fragment shader
 3. Setup all the fixed stages i.e., Input Sampling, Dynamic states of Viewports and Scissors, Rasterizer, Multisampling, Color blending, Pipeline layout
 4. Create a render pass before creating graphics pipeline to tell vulkan how to handle rendering operations
 
 */

/**
 Phase 4: Drawing
 1. Setup Framebuffer
 
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <iostream>
#include <vector>
#include <optional>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    "VK_KHR_portability_subset"     // https://vulkan.lunarg.com/doc/view/1.3.236.0/mac/1.3-extensions/vkspec.html#VUID-VkDeviceCreateInfo-pProperties-04451
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    
    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class HelloTriangleApplication {
public:
    HelloTriangleApplication() = default;
    HelloTriangleApplication(const HelloTriangleApplication& obj) = delete;
    
    HelloTriangleApplication& operator=(const HelloTriangleApplication& obj) = delete;
    
    ~HelloTriangleApplication() = default;
    
    void run();
    
private:
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();
    
    // Vulkan init functions
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffer();
    void createSyncObjects();
    
    // drawing
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void drawFrame();
    
    // Helper functions start
    
    // Vulkan Instance creation
    void checkRequiredExtensionSupport(const std::vector<const char*>& requiredExtensions) const;
    std::vector<const char*> getRequiredExtensions();
    
    // Validation Layer Support
    bool checkValidationLayerSupport();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                                                        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData) {
        
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            // Message is important enough to show
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        }
        
        return VK_FALSE;
    }
    
    // Logical and Physical Device
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    
    // Swap chain
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    
    // Misc
    static std::vector<char> readFile(const std::string& filename);
    
    // Graphics pipeline
    VkShaderModule createShaderModule(const std::vector<char>& code);
    
    // Helper functions end
    
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
    
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    
    std::vector<VkFramebuffer> swapChainFramebuffers;
    
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
    
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
    
    float queuePriority = 1.0f;
};
