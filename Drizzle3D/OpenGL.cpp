#include "RenderingLayer.h"

namespace Drizzle3D {
	void RenderingLayer::InitGlRendering() {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glEnable(GL_DEPTH_TEST);
        glGenBuffers(1, &lightsBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, lightsBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * 500, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightsBuffer);  // Binding to the binding point 0
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        GLuint projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");
        Create_Shader("VertexShader.glsl", "FragmentShader.glsl");
        OldshaderProgram = shaderProgram;
        glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUseProgram(0);
	}
    
    void RenderingLayer::DrawVertGLRendering(Object &myOBJ) {

        glGenVertexArrays(1, &myOBJ.VertexArray);
        glGenBuffers(1, &myOBJ.VertexBuffer);
        glGenBuffers(1, &myOBJ.IndexBuffer);

        // Bind the VAO
        glBindVertexArray(myOBJ.VertexArray);

        // Bind the VBO and set vertex data
        glBindBuffer(GL_ARRAY_BUFFER, myOBJ.VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, myOBJ.vertices.size() * sizeof(float), myOBJ.vertices.data(), GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Set vertex attribute pointers for Tex Coords
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Bind the EBO and set index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myOBJ.IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, myOBJ.indices.size() * sizeof(unsigned int), myOBJ.indices.data(), GL_STATIC_DRAW);

        // Unbind VAO to prevent accidentally modifying it elsewhere
        glBindVertexArray(0);
    }

    void RenderingLayer::RenderInitGlRendering() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 150.0f);
        glm::mat4 viewMatrix = glm::lookAt(returnCamera(current_camera)->position, returnCamera(current_camera)->look_at_position, returnCamera(current_camera)->up);
        //glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 cameraPosition = glm::inverse(viewMatrix)[3];

        GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
        GLuint projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");
        GLuint viewPos = glGetUniformLocation(shaderProgram, "viewPos");

        glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(viewPos, 1, GL_FALSE, glm::value_ptr(cameraPosition));

        GLuint numLightsLoc = glGetUniformLocation(shaderProgram, "numLights");
        GLuint lightsArrayLoc = glGetUniformLocation(shaderProgram, "lights");

        // Set the uniform values for the number of lights
        glUniform1i(numLightsLoc, static_cast<int>(Lights.size()));

        // Loop through each light in the array and set its values
        for (size_t i = 0; i < Lights.size(); ++i) {
            std::string lightPrefix = "lights[" + std::to_string(i) + "]";

            glUniform3fv(glGetUniformLocation(shaderProgram, (lightPrefix + ".direction").c_str()), 1, glm::value_ptr(Lights[i].direction));
            glUniform3fv(glGetUniformLocation(shaderProgram, (lightPrefix + ".position").c_str()), 1, glm::value_ptr(Lights[i].position));
            glUniform3fv(glGetUniformLocation(shaderProgram, (lightPrefix + ".color").c_str()), 1, glm::value_ptr(Lights[i].color));
            glUniform1f(glGetUniformLocation(shaderProgram, (lightPrefix + ".strength").c_str()), Lights[i].strength);
            glUniform1f(glGetUniformLocation(shaderProgram, (lightPrefix + ".specularStrength").c_str()), Lights[i].SpecularStrength);
            glUniform3fv(glGetUniformLocation(shaderProgram, (lightPrefix + ".ambient").c_str()), 1, glm::value_ptr(Lights[i].ambient));
            glUniform3fv(glGetUniformLocation(shaderProgram, (lightPrefix + ".diffuse").c_str()), 1, glm::value_ptr(Lights[i].diffuse));
            glUniform3fv(glGetUniformLocation(shaderProgram, (lightPrefix + ".specular").c_str()), 1, glm::value_ptr(Lights[i].specular));
            glUniform1i(glGetUniformLocation(shaderProgram, (lightPrefix + ".type").c_str()), Lights[i].type);
            glUniform1i(glGetUniformLocation(shaderProgram, (lightPrefix + ".id").c_str()), Lights[i].id);
            glUniform1f(glGetUniformLocation(shaderProgram, (lightPrefix + ".constant").c_str()), Lights[i].constant);
            glUniform1f(glGetUniformLocation(shaderProgram, (lightPrefix + ".linear").c_str()), Lights[i].linear);
            glUniform1f(glGetUniformLocation(shaderProgram, (lightPrefix + ".quadratic").c_str()), Lights[i].quadratic);
        }
        for (const auto& obje : Objects) {
        if (obje.hide)
            continue;
        } else {
            if (obje.mat != shaderProgram) {
                glUseProgram(obje.mat);
                GLuint viewMatrixLoc = glGetUniformLocation(obje.mat, "viewMatrix");
                GLuint projectionMatrixLoc = glGetUniformLocation(obje.mat, "projectionMatrix");
                GLuint viewPos = glGetUniformLocation(obje.mat, "viewPos");

                glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
                glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
                glUniformMatrix4fv(viewPos, 1, GL_FALSE, glm::value_ptr(cameraPosition));

                GLuint numLightsLoc = glGetUniformLocation(obje.mat, "numLights");
                GLuint lightsArrayLoc = glGetUniformLocation(obje.mat, "lights");

                // Set the uniform values for the number of lights
                glUniform1i(numLightsLoc, static_cast<int>(Lights.size()));

                // Loop through each light in the array and set its values
                for (size_t i = 0; i < Lights.size(); ++i) {
                    std::string lightPrefix = "lights[" + std::to_string(i) + "]";

                    glUniform3fv(glGetUniformLocation(obje.mat, (lightPrefix + ".direction").c_str()), 1, glm::value_ptr(Lights[i].direction));
                    glUniform3fv(glGetUniformLocation(obje.mat, (lightPrefix + ".position").c_str()), 1, glm::value_ptr(Lights[i].position));
                    glUniform3fv(glGetUniformLocation(obje.mat, (lightPrefix + ".color").c_str()), 1, glm::value_ptr(Lights[i].color));
                    glUniform1f(glGetUniformLocation(obje.mat, (lightPrefix + ".strength").c_str()), Lights[i].strength);
                    glUniform1f(glGetUniformLocation(obje.mat, (lightPrefix + ".specularStrength").c_str()), Lights[i].SpecularStrength);
                    glUniform3fv(glGetUniformLocation(obje.mat, (lightPrefix + ".ambient").c_str()), 1, glm::value_ptr(Lights[i].ambient));
                    glUniform3fv(glGetUniformLocation(obje.mat, (lightPrefix + ".diffuse").c_str()), 1, glm::value_ptr(Lights[i].diffuse));
                    glUniform3fv(glGetUniformLocation(obje.mat, (lightPrefix + ".specular").c_str()), 1, glm::value_ptr(Lights[i].specular));
                    glUniform1i(glGetUniformLocation(obje.mat, (lightPrefix + ".type").c_str()), Lights[i].type);
                    glUniform1i(glGetUniformLocation(obje.mat, (lightPrefix + ".id").c_str()), Lights[i].id);
                    glUniform1f(glGetUniformLocation(obje.mat, (lightPrefix + ".constant").c_str()), Lights[i].constant);
                    glUniform1f(glGetUniformLocation(obje.mat, (lightPrefix + ".linear").c_str()), Lights[i].linear);
                    glUniform1f(glGetUniformLocation(obje.mat, (lightPrefix + ".quadratic").c_str()), Lights[i].quadratic);
                }
                if (obje.name == "Skybox") {
                    GLuint IsSkyBox = glGetUniformLocation(obje.mat, "IsSkyBox");

                    glUniform1i(IsSkyBox, static_cast<bool>(true));
                }
                else {
                    GLuint IsSkyBox = glGetUniformLocation(obje.mat, "IsSkyBox");

                    glUniform1i(IsSkyBox, static_cast<bool>(false));
                }
                if (Lighting == false) {
                    GLuint IsSkyBox = glGetUniformLocation(obje.mat, "IsSkyBox");

                    glUniform1i(IsSkyBox, static_cast<bool>(true));
                }
                glBindTexture(GL_TEXTURE_2D, obje.textureID);
                glUniform1i(glGetUniformLocation(obje.mat, "textureSampler"), 0);

                GLuint modelMatrixLoc = glGetUniformLocation(obje.mat, "modelMatrix");
                glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(obje.modelMatrix));
            }
            else {
                glUseProgram(shaderProgram);
                if (obje.name == "Skybox") {
                    GLuint IsSkyBox = glGetUniformLocation(shaderProgram, "IsSkyBox");

                    glUniform1i(IsSkyBox, static_cast<bool>(true));
                }
                else {
                    GLuint IsSkyBox = glGetUniformLocation(shaderProgram, "IsSkyBox");

                    glUniform1i(IsSkyBox, static_cast<bool>(false));
                }

                if (Lighting == false) {
                    GLuint IsSkyBox = glGetUniformLocation(shaderProgram, "IsSkyBox");

                    glUniform1i(IsSkyBox, static_cast<bool>(true));
                }
                glBindTexture(GL_TEXTURE_2D, obje.textureID);
                glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);

                GLuint modelMatrixLoc = glGetUniformLocation(shaderProgram, "modelMatrix");
                glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(obje.modelMatrix));
            }

            glBindVertexArray(obje.VertexArray);
            glDrawElements(GL_TRIANGLES, obje.indices.size(), GL_UNSIGNED_INT, 0);
        }
        }

        glUseProgram(0);
        glDisable(GL_CULL_FACE);
    }

}
