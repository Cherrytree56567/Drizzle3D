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

    void RenderingLayer::InitVulkanRendering() {
        log.Warning("Vulkan Initialization Not Implemented.");
        // Switch GLFW_CLIENT_API to GLFW_NO_API
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Initialize Vulkan
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

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &pVulkanPipe.instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        log.Info(std::to_string(extensionCount) + " Extensions Supported.");

        

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
