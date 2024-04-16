/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "Material.h"

namespace Drizzle3D {
	Material::Material(std::shared_ptr<ResourceManager> resourcemgr, const char* fname, const char* fgname, RenderingAPI rAPI) : renderingAPI(rAPI) {
        Resource frag = resourcemgr->loadFile(fname, "r");

        const char* VSSource = frag.content.c_str();

        Resource vert = resourcemgr->loadFile(fgname, "r");

        const char* FSSource = vert.content.c_str();

        switch (renderingAPI) {
        case (RenderingAPI::OpenGL):
            if (true) {
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
            break;

        case (RenderingAPI::Vulkan):
            std::cout << "[Drizzle3D::Core] Warning: Vulkan Materials is not supported.";
        }
	}

    void Material::AddVariableToShader(const char* variableName, GLenum type, std::any value) {
        switch (renderingAPI) {
        case (RenderingAPI::OpenGL):
            if (true) {
                GLint location = glGetUniformLocation(shaderProgram, variableName);
                if (location != -1) {
                    switch (type) {
                    case GL_FLOAT:
                        glUniform1fv(location, 1, std::any_cast<GLfloat*>(value));
                        break;
                    case GL_FLOAT_VEC2:
                        glUniform2fv(location, 1, std::any_cast<GLfloat*>(value));
                        break;
                    case GL_FLOAT_VEC3:
                        glUniform3fv(location, 1, std::any_cast<GLfloat*>(value));
                        break;
                    case GL_FLOAT_VEC4:
                        glUniform4fv(location, 1, std::any_cast<GLfloat*>(value));
                        break;
                    case GL_INT:
                        glUniform1iv(location, 1, std::any_cast<GLint*>(value));
                        break;
                    case GL_INT_VEC2:
                        glUniform2iv(location, 1, std::any_cast<GLint*>(value));
                        break;
                    case GL_INT_VEC3:
                        glUniform3iv(location, 1, std::any_cast<GLint*>(value));
                        break;
                    case GL_INT_VEC4:
                        glUniform4iv(location, 1, std::any_cast<GLint*>(value));
                        break;
                    case GL_BOOL:
                        glUniform1iv(location, 1, std::any_cast<GLint*>(value));
                        break;
                    case GL_BOOL_VEC2:
                        glUniform2iv(location, 1, std::any_cast<GLint*>(value));
                        break;
                    case GL_BOOL_VEC3:
                        glUniform3iv(location, 1, std::any_cast<GLint*>(value));
                        break;
                    case GL_BOOL_VEC4:
                        glUniform4iv(location, 1, std::any_cast<GLint*>(value));
                        break;
                    case GL_FLOAT_MAT2:
                        glUniformMatrix2fv(location, 1, GL_FALSE, std::any_cast<GLfloat*>(value));
                        break;
                    case GL_FLOAT_MAT3:
                        glUniformMatrix3fv(location, 1, GL_FALSE, std::any_cast<GLfloat*>(value));
                        break;
                    case GL_FLOAT_MAT4:
                        glUniformMatrix4fv(location, 1, GL_FALSE, std::any_cast<GLfloat*>(value));
                        break;
                    default:
                        log.Error("Unsupported variable type");
                    }
                }
                else {
                    log.Error("Variable " + (std::string)variableName + " not found in shader");
                }
            }
        case (RenderingAPI::Vulkan):
            std::cout << "[Drizzle3D::Core] Warning: Vulkan Materials is not supported.";
        }
    }
}