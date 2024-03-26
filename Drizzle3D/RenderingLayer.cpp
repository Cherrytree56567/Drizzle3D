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

    RenderingLayer::RenderingLayer(Window* window, std::shared_ptr<ResourceManager> resmgr) : name("3DLayer"), show(true), pWindow(window), resourcemgr(resmgr) {
        flags.AddFlag("Lighting", Lighting);
        flags.AddFlag("Fullscreen", fullscreen);
        flags.AddFlag("Show", show);
        flags.AddFlag("UseOpenGL", UseOpenGL);
    }

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
        
        if (UseOpenGL) {
            InitGlRendering();
        }

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
        if (UseOpenGL) {
            DrawVertGLRendering(myOBJ);
        }

        return myOBJ;
    }

    void RenderingLayer::Render() {
        if (fullscreen) {
            glfwMaximizeWindow(pWindow->returnwindow());
        } else {
            glfwRestoreWindow(pWindow->returnwindow());
        }

        if (UseOpenGL) {
            RenderInitGlRendering();
        }
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