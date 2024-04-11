/*
***********************************************************************
*                                                                     *
* Drizzle3D � 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include "ResourceManager.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <tuple>
#include <any>
#include "Logging.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Drizzle3D {
	class Material {
	public:
		Drizzle3D_API Material(std::shared_ptr<ResourceManager> resourcemgr, const char* fname, const char* fgname);
		Drizzle3D_API GLuint GetShaderProgram() { return shaderProgram; }
		Drizzle3D_API void AddVariableToShader(const char* variableName, GLenum type, std::any value);
	private:
		GLuint shaderProgram;
		Logging log;
	};
}
