#include "RenderingLayer.h"

namespace Drizzle3D {
    bool RenderingLayer::checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    std::vector<const char*> RenderingLayer::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

        Logging log;

        log.Warning(pCallbackData->pMessage, "[Drizzle3D::Core::Vulkan] ");

        return VK_FALSE;
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    void RenderingLayer::VulkanDestroy() {
        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(pVulkanPipe.instance, pVulkanPipe.debugMessenger, nullptr);
        }

        vkDestroyInstance(pVulkanPipe.instance, nullptr);
    }

    int RenderingLayer::rateDeviceSuitability(VkPhysicalDevice device) {

        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        int score = 0;

        
        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        // Maximum possible size of textures affects graphics quality
        score += deviceProperties.limits.maxImageDimension2D;

        // Application can't function without geometry shaders
        if (!deviceFeatures.geometryShader) {
            return 0;
        }

        log.Info((std::string)deviceProperties.deviceName + (std::string)": " + std::to_string(score) + (std::string)" Points.", "[Drizzle3D::Core::Vulkan] ");

        return score;
    }

    void RenderingLayer::InitVulkanRendering() {
        log.Warning("Vulkan Initialization Not Implemented.");
        // Switch GLFW_CLIENT_API to GLFW_NO_API
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Basic Info
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        log.Info(std::to_string(extensionCount) + " Extensions Supported.", "[Drizzle3D::Core::Vulkan] ");

        // Initialize Vulkan
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "New Drizzle3D Game";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Drizzle3D";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &pVulkanPipe.instance) != VK_SUCCESS) {
            throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to create instance!");
        }

        // Setup Debug Messenger
        if (!enableValidationLayers) {

            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            populateDebugMessengerCreateInfo(createInfo);

            if (CreateDebugUtilsMessengerEXT(pVulkanPipe.instance, &createInfo, nullptr, &pVulkanPipe.debugMessenger) != VK_SUCCESS) {
                throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to set up debug messenger!");
            }
        }

        // Pick Physical Device
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(pVulkanPipe.instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(pVulkanPipe.instance, &deviceCount, devices.data());

        std::multimap<int, VkPhysicalDevice> candidates;

        for (const auto& device : devices) {
            int score = rateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }

        // Check if the best candidate is suitable at all
        if (candidates.rbegin()->first > 0) {
            physicalDevice = candidates.rbegin()->second;
        }
        else {
            throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to find a suitable GPU!");
        }

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        // Init Vulkan Shaders
        // Create shader modules for vertex and fragment shaders
        // Create pipeline layout
        // Create render pass
        // Create graphics pipeline
        // Create command buffers

        exit(0);
    }

	void RenderingLayer::RenderInitVulkanRendering() {
		log.Warning("Vulkan Rendering Not Implemented.");
        // Clear Color and Depth Buffer

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 150.0f);
        glm::mat4 viewMatrix = glm::lookAt(returnCamera(current_camera)->position, returnCamera(current_camera)->look_at_position, returnCamera(current_camera)->up);
        //glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 cameraPosition = glm::inverse(viewMatrix)[3];

        // Pass ProjectionMatrix, ViewMatrix and CameraPosition to Vulkan Shader

        // Pass Lights and Number of Lights to Vulkan Shader

        // Set the uniform values for the number of lights

        // Loop through each light in the array
        for (size_t i = 0; i < Lights.size(); ++i) {
            std::string lightPrefix = "lights[" + std::to_string(i) + "]";
            // Set its values
        }
        for (const auto& obje : Objects) {
            if (obje.hide) {
                continue;
            } else {
            // Look at Object struct in RenderingLayer.h
            if (false) { // If Default Vulkan shader is not equal to Vulkan Shader Inside "obje"

                // Set Vulkan Shader inside of "obje" as current usable one
                
                // Pass ProjectionMatrix, ViewMatrix and CameraPosition to Vulkan Shader

                // Pass Lights and Number of Lights to Vulkan Shader inside of "obje"

                // Set the uniform values for the number of lights to Vulkan Shader inside of "obje"

                // Loop through each light in the array
                for (size_t i = 0; i < Lights.size(); ++i) {
                    std::string lightPrefix = "lights[" + std::to_string(i) + "]";
                    // Set its values to Vulkan Shader inside of "obje"
                }

                // If Skybox is True
                if (obje.name == "Skybox") {
                    // Pass true to Skybox Value in Vulkan Shader inside of "obje"
                }
                else {
                    // Pass false to Skybox Value in Vulkan Shader inside of "obje"
                }
                if (Lighting == false) {
                    // Pass true to Skybox Value in Vulkan Shader inside of "obje"
                }
                // Convert OpenGL Texture to Vulkan Texture

                // Pass Vulkan Texture to Vulkan Shader inside of "obje"

                // Pass Vulkan Shader the ModelMatrix inside "obje"
            }
            else {
                // If Skybox is True
                if (obje.name == "Skybox") {
                    // Pass true to Skybox Value in Vulkan Shader
                }
                else {
                    // Pass false to Skybox Value in Vulkan Shader
                }
                if (Lighting == false) {
                    // Pass true to Skybox Value in Vulkan Shader
                }
                // Convert OpenGL Texture to Vulkan Texture

                // Pass Vulkan Texture to Vulkan Shader

                // Pass Vulkan Shader the ModelMatrix
            }
            }
            // Bind Vertex Array inside of "obje"
            // Draw Vertexs
        }

        // Disable Face Culling
	}

	void RenderingLayer::DrawVertVulkanRendering(Object& myOBJ) {
		log.Warning("Vulkan Vertex Drawing Not Implemented.");
		// Create vertex buffer
		// Allocate memory for vertex buffer
		// Bind memory to vertex buffer
		// Copy vertex data to vertex buffer
		// Create index buffer (similar to vertex buffer)
	}

    void RenderingLayer::Create_VulkanShader(const char* fname, const char* fgname) {

    }

    void RenderingLayer::Create_DefaultVulkanShader(const char* fname, const char* fgname) {

    }
}
