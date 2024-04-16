/*
***********************************************************************
*                                                                     *
* Drizzle3D � 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <variant>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Layer.h"
#include "Material.h"
#include "ResourceManager.h"
#include "base.h"
#include "Flags.h"
#include "Vulkan.h"

namespace Drizzle3D {

    Drizzle3D_API std::pair<std::vector<float>, std::vector<unsigned int>> LoadObjFile(const std::string& filePath);
    Drizzle3D_API GLuint GetTexture(const char* texturePath);

    enum Lights {
        Directional,
        Point
    };

    typedef std::pair<VkShaderModule, VkShaderModule> VkDrizzleShader;

    struct Light {
        glm::vec3 direction;
        glm::vec3 position;
        glm::vec3 color;
        float strength;
        float SpecularStrength;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        Lights type;
        int id;

        float constant;
        float linear;
        float quadratic;
    };

    struct Object {
        GLuint VertexArray, VertexBuffer, IndexBuffer = 0;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        glm::mat4 modelMatrix;
        GLuint textureID = NULL;
        GLuint mat = 0;
        char* name = (char*)"PLZ_SPECIFY_A_NAME";
        bool hide = false;
        VkPipeline Pipeline;
    };

    struct Camera {
        glm::vec3 position;
        glm::vec3 look_at_position;
        glm::vec3 up;
        char* ID;
    };

    class RenderingLayer : public Layer {
    public:
        Drizzle3D_API RenderingLayer(RenderingAPI rAPI, std::shared_ptr<Window> window, std::shared_ptr<ResourceManager> resmgr);

        Drizzle3D_API void OnAttach() override;
        Drizzle3D_API void OnDetach() override {}
        Drizzle3D_API void Render() override;

        Drizzle3D_API bool IsShown() const override { return show; }
        Drizzle3D_API const std::string& GetName() const override { return name; }
        Drizzle3D_API void SetShow(bool value) override { show = value; }

        Drizzle3D_API std::variant<VkDrizzleShader, GLuint> Create_Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
        Drizzle3D_API std::variant<VkDrizzleShader, GLuint> Create_DefaultShader(const char* vertexShaderSource, const char* fragmentShaderSource);
        Drizzle3D_API Object DrawVerts(std::pair<std::vector<float>, std::vector<unsigned int>> vf, glm::mat4 modelMatrix = glm::mat4(1.0f));
        Drizzle3D_API void AddObject(const char* name, Object theObject);
        Drizzle3D_API Object* returnObject(const char* name);
        Drizzle3D_API void RemoveObject(const char* name);
        Drizzle3D_API void AddLight(float id, Light theLight);
        Drizzle3D_API Light* returnLight(float id);
        Drizzle3D_API void RemoveLight(float id);
        Drizzle3D_API void SwitchCamera(const char* name);
        Drizzle3D_API void AddCamera(const char* id, Camera theCamera);
        Drizzle3D_API Camera* returnCamera(const char* id);
        Drizzle3D_API void RemoveCamera(const char* id);
        Drizzle3D_API char* GetActiveCamera() { return current_camera; }
        Drizzle3D_API Camera ReturnActiveCamera();
        Drizzle3D_API Camera GetCameraFromID(char* cam);
        Drizzle3D_API Flags* GetFlags() { return &flags; }
    private:
        void InitGlRendering();
        void RenderInitGlRendering();
        void DrawVertGLRendering(Object& myOBJ);
        void InitVulkanRendering();
        void RenderInitVulkanRendering();
        void DrawVertVulkanRendering(Object& myOBJ);
        GLuint Create_DefaultOpenGLShader(const char* vertexShaderSource, const char* fragmentShaderSource);
        VkDrizzleShader Create_DefaultVulkanShader(const char* vertexShaderSource, const char* fragmentShaderSource);
        GLuint Create_OpenGLShader(const char* vertexShaderSource, const char* fragmentShaderSource);
        VkDrizzleShader Create_VulkanShader(const char* vertexShaderSource, const char* fragmentShaderSource);
        void VulkanDestroy();

        bool checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();
        int rateDeviceSuitability(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSwapChain();
        void createImageViews();
        VkPipeline createGraphicsPipeline(const char* fname, const char* fgname, VkViewport viewport);
        void createRenderPass();
        void createFramebuffers();

        bool Lighting = true;
        bool fullscreen = false;
        RenderingAPI renderingAPI;
        bool show;
        GLuint shaderProgram = 0;
        GLuint OldshaderProgram = 0;
        std::string name;
        std::shared_ptr<Window> pWindow;
        VulkanPipeline pVulkanPipe;
        std::vector<Object> Objects;
        std::vector<Light> Lights;
        std::vector<Camera> Cameras;
        Flags flags;
        GLuint lightsBuffer = 0;
        char* current_camera = (char*)"Default";
        std::shared_ptr<ResourceManager> resourcemgr;
        Logging log;
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
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
	};
}
