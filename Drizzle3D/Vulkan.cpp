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
        vkDestroyPipeline(pVulkanPipe.device, pVulkanPipe.DefaultgraphicsPipeline, nullptr);
        vkDestroyPipelineLayout(pVulkanPipe.device, pVulkanPipe.pipelineLayout, nullptr);
        vkDestroyRenderPass(pVulkanPipe.device, pVulkanPipe.renderPass, nullptr);

        for (auto imageView : pVulkanPipe.swapChainImageViews) {
            vkDestroyImageView(pVulkanPipe.device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(pVulkanPipe.device, pVulkanPipe.swapChain, nullptr);
        vkDestroyDevice(pVulkanPipe.device, nullptr);

        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(pVulkanPipe.instance, pVulkanPipe.debugMessenger, nullptr);
        }

        vkDestroySurfaceKHR(pVulkanPipe.instance, pVulkanPipe.surface, nullptr);
        vkDestroyInstance(pVulkanPipe.instance, nullptr);
    }

    QueueFamilyIndices RenderingLayer::findQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, pVulkanPipe.surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

    void switchError(VkResult s) {
        switch (s) {
        case VK_NOT_READY:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_NOT_READY: A fence or query has not yet completed." << std::endl;
            break;
        case VK_TIMEOUT:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_TIMEOUT: A wait operation has not completed in the specified time." << std::endl;
            break;
        case VK_EVENT_SET:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_EVENT_SET: An event is signaled." << std::endl;
            break;
        case VK_EVENT_RESET:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_EVENT_RESET: An event is unsignaled." << std::endl;
            break;
        case VK_INCOMPLETE:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_INCOMPLETE: A return array was too small for the result." << std::endl;
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_OUT_OF_HOST_MEMORY: A host memory allocation has failed." << std::endl;
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_OUT_OF_DEVICE_MEMORY: A device memory allocation has failed." << std::endl;
            break;
        case VK_ERROR_INITIALIZATION_FAILED:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_INITIALIZATION_FAILED: Initialization of an object could not be completed for implementation-specific reasons." << std::endl;
            break;
        case VK_ERROR_DEVICE_LOST:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_DEVICE_LOST: The logical or physical device has been lost." << std::endl;
            break;
        case VK_ERROR_MEMORY_MAP_FAILED:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_MEMORY_MAP_FAILED: Mapping of a memory object has failed." << std::endl;
            break;
        case VK_ERROR_LAYER_NOT_PRESENT:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_LAYER_NOT_PRESENT: A requested layer is not present or could not be loaded." << std::endl;
            break;
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_EXTENSION_NOT_PRESENT: A requested extension is not supported." << std::endl;
            break;
        case VK_ERROR_FEATURE_NOT_PRESENT:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_FEATURE_NOT_PRESENT: A requested feature is not supported." << std::endl;
            break;
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_INCOMPATIBLE_DRIVER: The requested version of Vulkan is not supported by the driver or is otherwise incompatible for implementation-specific reasons." << std::endl;
            break;
        case VK_ERROR_TOO_MANY_OBJECTS:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_TOO_MANY_OBJECTS: Too many objects of the type have already been created." << std::endl;
            break;
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_FORMAT_NOT_SUPPORTED: A requested format is not supported on this device." << std::endl;
            break;
        case VK_ERROR_FRAGMENTED_POOL:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_FRAGMENTED_POOL: A pool allocation has failed due to fragmentation of the pool's memory." << std::endl;
            break;
        case VK_ERROR_UNKNOWN:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_UNKNOWN: An unknown error occurred." << std::endl;
            break;
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_OUT_OF_POOL_MEMORY: A pool memory allocation has failed." << std::endl;
            break;
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_INVALID_EXTERNAL_HANDLE: An external handle is not a valid handle." << std::endl;
            break;
        case VK_ERROR_FRAGMENTATION_EXT:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_FRAGMENTATION_EXT: A descriptor pool creation has failed due to fragmentation." << std::endl;
            break;
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: An opaque capture address is not valid." << std::endl;
            break;
        case VK_ERROR_SURFACE_LOST_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_SURFACE_LOST_KHR: The surface is no longer available." << std::endl;
            break;
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: The requested window is already in use by Vulkan or another API in a manner which prevents it from being used again." << std::endl;
            break;
        case VK_SUBOPTIMAL_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_SUBOPTIMAL_KHR: A swapchain no longer matches the surface properties exactly, but can still be used to present to the surface successfully." << std::endl;
            break;
        case VK_ERROR_OUT_OF_DATE_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_OUT_OF_DATE_KHR: A surface has changed in such a way that it is no longer compatible with the swapchain, and further presentation requests using the swapchain will fail." << std::endl;
            break;
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: The display used by a swapchain does not use the same presentable image layout, or is incompatible in a way that prevents sharing an image." << std::endl;
            break;
        case VK_ERROR_VALIDATION_FAILED_EXT:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_VALIDATION_FAILED_EXT: A validation layer found an error." << std::endl;
            break;
        case VK_ERROR_INVALID_SHADER_NV:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_INVALID_SHADER_NV: One or more shaders failed to compile or link." << std::endl;
            break;
        case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: One or more parameters provided to CreateVideoSessionKHR are not within valid ranges." << std::endl;
            break;
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: Layout in DRM Format Modifier is not supported." << std::endl;
            break;
        case VK_ERROR_NOT_PERMITTED_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_NOT_PERMITTED_KHR: Requested operation is not permitted." << std::endl;
            break;
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: An operation on a swapchain created with a full-screen exclusive mode presentation type failed because the mode is no longer supported." << std::endl;
            break;
        case VK_THREAD_IDLE_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_THREAD_IDLE_KHR: A deferred operation is waiting on a thread." << std::endl;
            break;
        case VK_THREAD_DONE_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_THREAD_DONE_KHR: A deferred operation is blocked on a thread." << std::endl;
            break;
        case VK_OPERATION_DEFERRED_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_OPERATION_DEFERRED_KHR: A deferred operation was requested and no deferred operation is pending." << std::endl;
            break;
        case VK_OPERATION_NOT_DEFERRED_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_OPERATION_NOT_DEFERRED_KHR: A deferred operation is pending and a deferred operation was requested but not performed." << std::endl;
            break;
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_COMPRESSION_EXHAUSTED_EXT: A block of compressed data was too large to be processed or contained an invalid block." << std::endl;
            break;
        case VK_INCOMPATIBLE_SHADER_BINARY_EXT:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_INCOMPATIBLE_SHADER_BINARY_EXT: The shader binary is not compatible with one or more of the rest of the pipeline state." << std::endl;
            break;
        case VK_PIPELINE_COMPILE_REQUIRED:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_PIPELINE_COMPILE_REQUIRED: A requested pipeline creation requires compilation but the application has not made use of the pipeline compilation feedback information." << std::endl;
            break;
        case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR: The image usage is not supported by the current queue family." << std::endl;
            break;
        case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR: The video decoder is not capable of changing the video picture layout." << std::endl;
            break;
        case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR: The video decoder cannot create the requested video picture layout." << std::endl;
            break;
        case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR: The video decoder cannot create the requested video picture layout because it doesn't support it in the requested format." << std::endl;
            break;
        case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR: The video decoder cannot create the requested video picture layout because it doesn't support the requested codec." << std::endl;
            break;
        case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR: The video decoder cannot create the requested video picture layout because it doesn't support the requested standard version." << std::endl;
            break;
        case VK_RESULT_MAX_ENUM:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: VK_RESULT_MAX_ENUM: Maximum value of an enumeration." << std::endl;
            break;
        default:
            std::cout << "[Drizzle3D::Core::Vulkan] Error: Unknown Vulkan error code: " << s << std::endl;
            break;
        }
    }

    bool RenderingLayer::isDeviceSuitable(VkPhysicalDevice device) {
        QueueFamilyIndices indices = findQueueFamilies(device);

        bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }

    bool RenderingLayer::checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    int RenderingLayer::rateDeviceSuitability(VkPhysicalDevice device) {

        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
        QueueFamilyIndices indices = findQueueFamilies(device);

        int score = 0;
        
        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        // Maximum possible size of textures affects graphics quality
        score += deviceProperties.limits.maxImageDimension2D;

        // Application can't function without geometry shaders
        if (!deviceFeatures.geometryShader) {
            score = 0;
            return 0;
        }

        if (!indices.isComplete()) {
            score = 0;
            return 0;
        }

        if (!isDeviceSuitable(device)) {
            score = 0;
            return 0;
        }

        log.Info((std::string)deviceProperties.deviceName + (std::string)": " + std::to_string(score) + (std::string)" Points.", "[Drizzle3D::Core::Vulkan] ");

        return score;
    }

    SwapChainSupportDetails RenderingLayer::querySwapChainSupport(VkPhysicalDevice device) {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, pVulkanPipe.surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, pVulkanPipe.surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, pVulkanPipe.surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, pVulkanPipe.surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, pVulkanPipe.surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR RenderingLayer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR RenderingLayer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D RenderingLayer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }
        else {
            int width, height;
            glfwGetFramebufferSize(pWindow->returnwindow(), &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void RenderingLayer::createInstance() {
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
    }

    void RenderingLayer::setupDebugMessenger() {
        if (!enableValidationLayers) {

            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            populateDebugMessengerCreateInfo(createInfo);

            if (CreateDebugUtilsMessengerEXT(pVulkanPipe.instance, &createInfo, nullptr, &pVulkanPipe.debugMessenger) != VK_SUCCESS) {
                throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to set up debug messenger!");
            }
        }
    }

    void RenderingLayer::createSurface() {
        uint32_t count = 0;
        glfwGetRequiredInstanceExtensions(&count);

        std::cout << "Required Instance Extensions: " << count << "\n";

        VkResult s = glfwCreateWindowSurface(pVulkanPipe.instance, pWindow->returnwindow(), nullptr, &pVulkanPipe.surface);
        if (s != VK_SUCCESS) {
            switchError(s);
            throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to create window surface!");
        }
    }

    void RenderingLayer::pickPhysicalDevice() {
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
            pVulkanPipe.physicalDevice = candidates.rbegin()->second;
        }
        else {
            throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to find a suitable GPU!");
        }
    }

    void RenderingLayer::createLogicalDevice() {
        QueueFamilyIndices indices = findQueueFamilies(pVulkanPipe.physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

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

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(pVulkanPipe.physicalDevice, &createInfo, nullptr, &pVulkanPipe.device) != VK_SUCCESS) {
            throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to create logical device!");
        }

        vkGetDeviceQueue(pVulkanPipe.device, indices.graphicsFamily.value(), 0, &pVulkanPipe.graphicsQueue);
        vkGetDeviceQueue(pVulkanPipe.device, indices.presentFamily.value(), 0, &pVulkanPipe.presentQueue);
    }

    void RenderingLayer::createSwapChain() {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(pVulkanPipe.physicalDevice);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = pVulkanPipe.surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(pVulkanPipe.physicalDevice);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(pVulkanPipe.device, &createInfo, nullptr, &pVulkanPipe.swapChain) != VK_SUCCESS) {
            throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(pVulkanPipe.device, pVulkanPipe.swapChain, &imageCount, nullptr);
        pVulkanPipe.swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(pVulkanPipe.device, pVulkanPipe.swapChain, &imageCount, pVulkanPipe.swapChainImages.data());

        pVulkanPipe.swapChainImageFormat = surfaceFormat.format;
        pVulkanPipe.swapChainExtent = extent;
    }

    void RenderingLayer::createImageViews() {
        pVulkanPipe.swapChainImageViews.resize(pVulkanPipe.swapChainImages.size());

        for (size_t i = 0; i < pVulkanPipe.swapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = pVulkanPipe.swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = pVulkanPipe.swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(pVulkanPipe.device, &createInfo, nullptr, &pVulkanPipe.swapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views!");
            }
        }
    }

    VkPipeline RenderingLayer::createGraphicsPipeline(const char* fname, const char* fgname, VkViewport viewport) {
        VkDrizzleShader ShaderModule = Create_VulkanShader(fname, fgname);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = ShaderModule.first;
        vertShaderStageInfo.pName = "main";
        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = ShaderModule.second;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(pVulkanPipe.device, &pipelineLayoutInfo, nullptr, &pVulkanPipe.pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to create pipeline layout!");
        }

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
        pipelineInfo.pDynamicState = &dynamicState;

        pipelineInfo.layout = pVulkanPipe.pipelineLayout;

        pipelineInfo.renderPass = pVulkanPipe.renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional

        VkPipeline graphicsPipelinaae;
        VkResult a = vkCreateGraphicsPipelines(pVulkanPipe.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipelinaae);
        if (a != VK_SUCCESS) {
            switchError(a);
            throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(pVulkanPipe.device, ShaderModule.first, nullptr);
        vkDestroyShaderModule(pVulkanPipe.device, ShaderModule.second, nullptr);

        return graphicsPipelinaae;
    }

    void RenderingLayer::createRenderPass() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = pVulkanPipe.swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

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

        if (vkCreateRenderPass(pVulkanPipe.device, &renderPassInfo, nullptr, &pVulkanPipe.renderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
    }

    void RenderingLayer::InitVulkanRendering() {

        log.Warning("Vulkan Initialization Not Implemented.");

        // Basic Info
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        log.Info(std::to_string(extensionCount) + " Extensions Supported.", "[Drizzle3D::Core::Vulkan] ");

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)pVulkanPipe.swapChainExtent.width;
        viewport.height = (float)pVulkanPipe.swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createSwapChain();
        createImageViews();
        createRenderPass();
        pVulkanPipe.DefaultgraphicsPipeline = createGraphicsPipeline("vert.spv", "frag.spv", viewport);

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

    VkDrizzleShader RenderingLayer::Create_VulkanShader(const char* fname, const char* fgname) {

        std::ifstream file(fname, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = buffer.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

        VkShaderModule vertShader;
        if (vkCreateShaderModule(pVulkanPipe.device, &createInfo, nullptr, &vertShader) != VK_SUCCESS) {
            throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to create Vertex shader module!");
        }

        std::ifstream filefrag(fgname, std::ios::ate | std::ios::binary);

        if (!filefrag.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSizefrag = (size_t)filefrag.tellg();
        std::vector<char> bufferfrag(fileSizefrag);

        filefrag.seekg(0);
        filefrag.read(bufferfrag.data(), fileSizefrag);

        filefrag.close();

        VkShaderModuleCreateInfo createInfoFrag{};
        createInfoFrag.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfoFrag.codeSize = bufferfrag.size();
        createInfoFrag.pCode = reinterpret_cast<const uint32_t*>(bufferfrag.data());

        VkShaderModule fragShader;
        if (vkCreateShaderModule(pVulkanPipe.device, &createInfoFrag, nullptr, &fragShader) != VK_SUCCESS) {
            throw std::runtime_error("[Drizzle3D::Core::Vulkan] Error: Failed to create Fragment shader module!");
        }

        return std::make_pair(vertShader, fragShader);
    }

    VkDrizzleShader RenderingLayer::Create_DefaultVulkanShader(const char* fname, const char* fgname) {
        return std::make_pair((VkShaderModule)NULL, (VkShaderModule)NULL);
    }
}
