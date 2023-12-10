#include "RenderingLayer.h"

namespace Drizzle3D {
	void RenderingLayer::Create_Shader(const char* fname, const char* fgname) {

        std::ifstream file(fname);

        if (!file.is_open()) {
            std::cerr << "[Drizzle3D::Core::RenderingLayer] Error: Error opening file: " << fname << std::endl;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        file.close();

        std::string fileContent = buffer.str();

        const char* VSSource = fileContent.c_str();

        std::ifstream filea(fgname);

        if (!filea.is_open()) {
            std::cerr << "[Drizzle3D::Core::RenderingLayer] Error: Error opening file: " << fgname << std::endl;
        }

        std::stringstream bufferr;
        bufferr << filea.rdbuf();

        filea.close();

        std::string fileCContent = bufferr.str();

        const char* FSSource = fileCContent.c_str();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &VSSource, 0);
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
            std::cerr << "[Drizzle3D::Core::RenderingLayer] Error: Error compiling Vertex Shader" << std::endl;
		}

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &FSSource, NULL);
        glCompileShader(fragmentShader);

        GLint isCompileda = 0;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompileda);
        if (isCompileda == GL_FALSE) {
            std::cerr << "[Drizzle3D::Core::RenderingLayer] Error: Error compiling Fragment Shader" << std::endl;
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glUseProgram(shaderProgram);
	}
}