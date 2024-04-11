/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "RenderingLayer.h"

namespace Drizzle3D {
    void RenderingLayer::Create_Shader(const char* fname, const char* fgname) {
        switch (renderingAPI) {
        case RenderingAPI::OpenGL:
            Create_OpenGLShader(fname, fgname);
            break;

        case RenderingAPI::Vulkan:
            Create_VulkanShader(fname, fgname);
            break;
        }
    }

    void RenderingLayer::Create_DefaultShader(const char* fname, const char* fgname) {
        switch (renderingAPI) {
        case RenderingAPI::OpenGL:
            Create_DefaultOpenGLShader(fname, fgname);
            break;

        case RenderingAPI::Vulkan:
            Create_DefaultVulkanShader(fname, fgname);
            break;
        }
    }
}
