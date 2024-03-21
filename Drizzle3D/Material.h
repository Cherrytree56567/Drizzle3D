/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include "ResourceManager.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Drizzle3D {
	class Drizzle3D_API Material {
	public:
		Material(std::shared_ptr<ResourceManager> resourcemgr, const char* fname, const char* fgname);
		GLuint GetShaderProgram() { return shaderProgram; }
	private:
		GLuint shaderProgram;
	};
}
