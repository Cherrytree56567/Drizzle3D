/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "RenderingLayer.h"

namespace Drizzle3D {
    std::variant<VkDrizzleShader, GLuint> RenderingLayer::Create_Shader(const char* fname, const char* fgname) {
        switch (renderingAPI) {
        case RenderingAPI::OpenGL:
            return Create_OpenGLShader(fname, fgname);
            break;

        case RenderingAPI::Vulkan:
            return Create_VulkanShader(fname, fgname);
            break;
        }
        return (GLuint)NULL;
    }

    std::variant<VkDrizzleShader, GLuint> RenderingLayer::Create_DefaultShader(const char* fname, const char* fgname) {
        switch (renderingAPI) {
        case RenderingAPI::OpenGL:
            return Create_DefaultOpenGLShader(fname, fgname);
            break;

        case RenderingAPI::Vulkan:
            return Create_DefaultVulkanShader(fname, fgname);
            break;
        }

        return (GLuint)NULL;
    }
}
