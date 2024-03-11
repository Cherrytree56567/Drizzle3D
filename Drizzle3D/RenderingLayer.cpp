/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "RenderingLayer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Drizzle3D {

    Drizzle3D_API std::pair<std::vector<float>, std::vector<unsigned int>> LoadObjFile(const std::string& filePath) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "Error loading OBJ file: " << importer.GetErrorString() << std::endl;
            return std::make_pair(vertices, indices);
        }

        const aiMesh* mesh = scene->mMeshes[0]; // Assuming there is only one mesh in the scene

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            // Vertex position (x, y, z)
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);

            // Texture coordinates (u, v)
            if (mesh->mTextureCoords[0]) {
                vertices.push_back(mesh->mTextureCoords[0][i].x);
                vertices.push_back(mesh->mTextureCoords[0][i].y);
            }
            else {
                // If the model doesn't have texture coordinates, use default values
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            const aiFace& face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }

        return std::make_pair(vertices, indices);
    }

    void RenderingLayer::OnAttach() {
        // Vertex Array
        // Vertex Buffer
        // Index Buffer
        // Shader (not needed)

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

        Light pointLight;

        pointLight.type = Lights::Point;
        pointLight.id = 1;  // Set an appropriate ID

        // Set position for a point light
        pointLight.position = glm::vec3(0.0f, 5.0f, 0.0f);

        // Set color and intensity
        pointLight.color = glm::vec3(1.0f, 1.0f, 1.0f); // white light
        pointLight.strength = 1.0f;
        pointLight.SpecularStrength = 1.0f;

        // Set lighting components
        pointLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
        pointLight.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        pointLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

        // Set attenuation parameters for the point light
        pointLight.constant = 1.0f;
        pointLight.linear = 0.09f;
        pointLight.quadratic = 0.032f;

        AddLight(1, pointLight);

        Camera Def = { glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) };
        AddCamera("Default", Def);
        SwitchCamera("Default");
    }

    Object RenderingLayer::DrawVerts(std::pair<std::vector<float>, std::vector<unsigned int>> vf, glm::mat4 modelMatrix) {
        Object myOBJ;
        myOBJ.vertices = vf.first;
        myOBJ.indices = vf.second;
        myOBJ.modelMatrix = modelMatrix;
        myOBJ.mat = shaderProgram;

        // Create Vertex Array Object (VAO), Vertex Buffer Object (VBO), and Element Buffer Object (EBO)
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

        return myOBJ;
    }

    void RenderingLayer::Render() {
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

        glUseProgram(0);
        glDisable(GL_CULL_FACE);
    }

    Drizzle3D_API GLuint GetTexture(const char* TexturePath) {
        // Load image using stb_image
        int width, height, channels;
        unsigned char* image = stbi_load(TexturePath, &width, &height, &channels, STBI_rgb);

        if (!image) {
            fprintf(stderr, "Failed to load image\n");
            glfwTerminate();
            return -1;
        }

        // Generate texture
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Provide texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Free the image data
        stbi_image_free(image);
        return textureID;
    }
}