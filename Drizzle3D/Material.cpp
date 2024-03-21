/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "Material.h"

namespace Drizzle3D {
	Material::Material(std::shared_ptr<ResourceManager> resourcemgr, const char* fname, const char* fgname) {
        Resource frag = resourcemgr->loadFile(fname, "r");

        const char* VSSource = frag.content.c_str();

        Resource vert = resourcemgr->loadFile(fgname, "r");

        const char* FSSource = vert.content.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &VSSource, 0);
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            std::cerr << "[Drizzle3D::Core::RenderingLayer] Error: Error compiling Vertex Shader" << std::endl;

            // Print compilation log
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);
            std::cerr << "Vertex Shader Compilation Log:\n" << &errorLog[0] << std::endl;

            return;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &FSSource, NULL);
        glCompileShader(fragmentShader);

        GLint isCompileda = 0;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompileda);
        if (isCompileda == GL_FALSE) {
            std::cerr << "[Drizzle3D::Core::RenderingLayer] Error: Error compiling Fragment Shader" << std::endl;

            // Print compilation log
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);
            std::cerr << "Fragment Shader Compilation Log:\n" << &errorLog[0] << std::endl;

            return;
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
	}
}