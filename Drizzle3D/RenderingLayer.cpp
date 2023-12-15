#include "RenderingLayer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Drizzle3D {
    Texture LoadTexture(const std::string& textureFilePath) {
        Texture texture;
        int channels; // Number of color channels in the image (RGB, RGBA, etc.)
        stbi_uc* data = stbi_load(textureFilePath.c_str(), &texture.width, &texture.height, &channels, STBI_rgb);

        if (data) {
            // Convert image data to float and store in texture.colors
            for (int i = 0; i < texture.width * texture.height * 3; ++i) {
                texture.colors.push_back(static_cast<float>(data[i]) / 255.0f);
            }

            stbi_image_free(data);
        }
        else {
            std::cerr << "Error: Unable to load texture file: " << textureFilePath << std::endl;
        }

        return texture;
    }

    std::pair<std::vector<float>, std::vector<unsigned int>> LoadObjFile(const std::string& objFilePath, const std::string& textureFilePath) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Default color if material file is NULL or empty
        float defaultColor[3] = { 1.0f, 1.0f, 1.0f };

        Texture texture = LoadTexture(textureFilePath);

        std::ifstream objFile(objFilePath);
        if (objFile.is_open()) {
            std::vector<glm::vec3> tempVertices;
            std::vector<glm::vec3> tempNormals;
            std::vector<glm::vec2> tempTexCoords;

            std::string line;
            while (std::getline(objFile, line)) {
                std::istringstream iss(line);
                std::string token;

                if (iss >> token) {
                    if (token == "vn") {
                        // Vertex normal information
                        glm::vec3 normal;
                        if (iss >> normal.x >> normal.y >> normal.z) {
                            tempNormals.push_back(normal);
                        }
                    }
                    else if (token == "vt") {
                        // Texture coordinate information
                        glm::vec2 texCoord;
                        if (iss >> texCoord.s >> texCoord.t) {
                            tempTexCoords.push_back(texCoord);
                        }
                    }
                    else if (token == "f") {
                        // Face information
                        unsigned int v1, vn1, v2, vn2, v3, vn3;
                        char slash;

                        if (iss >> v1 >> slash >> slash >> vn1 >> v2 >> slash >> slash >> vn2 >> v3 >> slash >> slash >> vn3) {
                            // Adjust indices to be 0-based
                            indices.push_back(v1 - 1);
                            indices.push_back(v2 - 1);
                            indices.push_back(v3 - 1);
                        }
                    }
                    else if (token == "o") {
                        // Object name (you can handle it as needed)
                        std::string objectName;
                        iss >> objectName;
                        std::cout << "Object Name: " << objectName << std::endl;
                    }
                    else if (token == "v") {
                        // Vertex information
                        glm::vec3 vertex;
                        if (iss >> vertex.x >> vertex.y >> vertex.z) {
                            tempVertices.push_back(vertex);
                        }
                    }
                }
            }
            objFile.close();

            // Combine vertices, normals, and texture coordinates into the final vector
            // Combine vertices, normals, and texture coordinates into the final vector
            for (size_t i = 0; i < tempVertices.size(); ++i) {
                // Append vertex coordinates
                vertices.push_back(tempVertices[i].x);
                vertices.push_back(tempVertices[i].y);
                vertices.push_back(tempVertices[i].z);

                // Append texture coordinates if available
                if (!tempTexCoords.empty()) {
                    vertices.push_back(tempTexCoords[i].s);
                    vertices.push_back(tempTexCoords[i].t);
                }

                // Append texture colors if available
                if (!texture.colors.empty()) {
                    vertices.push_back(texture.colors[i * 4]);     // R
                    vertices.push_back(texture.colors[i * 4 + 1]); // G
                    vertices.push_back(texture.colors[i * 4 + 2]); // B
                }
                else {
                    // Append default color
                    vertices.push_back(defaultColor[0]);
                    vertices.push_back(defaultColor[1]);
                    vertices.push_back(defaultColor[2]);
                }

                // Append normal coordinates
                vertices.push_back(tempNormals[i].x);
                vertices.push_back(tempNormals[i].y);
                vertices.push_back(tempNormals[i].z);
            }

        }
        else {
            std::cerr << "Error: Unable to open OBJ file: " << objFilePath << std::endl;
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
        glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUseProgram(0);

        Light myLight;
        myLight.pos_or_dir = glm::vec3(-1.0f, -1.0f, -1.0f);
        myLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
        myLight.intensity = 1.0f;
        myLight.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
        myLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        myLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        myLight.isDirectional = true;
        myLight.isPoint = false;
        myLight.constantPoint = 0.0f;
        myLight.linearPoint = 0.0f;
        myLight.quadraticPoint = 0.0f;
        myLight.ID = 1.0f;
        AddLight(1.0f, myLight);
    }

    Object RenderingLayer::DrawVerts(std::pair<std::vector<float>, std::vector<unsigned int>> vf, glm::mat4 modelMatrix) {
        Object myOBJ;
        myOBJ.vertices = vf.first;
        myOBJ.indices = vf.second;
        myOBJ.modelMatrix = modelMatrix;

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Set vertex attribute pointers for color
        // Set vertex attribute pointers for color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Set vertex attribute pointers for normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));  // Normal
        glEnableVertexAttribArray(2);

        // Bind the EBO and set index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myOBJ.IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, myOBJ.indices.size() * sizeof(unsigned int), myOBJ.indices.data(), GL_STATIC_DRAW);

        // Unbind VAO to prevent accidentally modifying it elsewhere
        glBindVertexArray(0);

        return myOBJ;
    }


    void RenderingLayer::AddObject(const char* name, Object theObject) {
        theObject.name = (char*)name;
        Objects.push_back(theObject);
    }

    Object* RenderingLayer::returnObject(const char* name) {
        for (int i = 0; i < Objects.size(); i++) {
            if (Objects[i].name == name) {
                return &Objects[i];
            }
        }
        return nullptr;
    }

    void RenderingLayer::RemoveObject(const char* name) {
        for (int i = 0; i < Objects.size(); i++) {
            if (Objects[i].name == name) {
                Objects.erase(Objects.begin() + i);
            }
        }
    }

    void RenderingLayer::Render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
        GLuint projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");

        glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        for (const auto& obje : Objects) {
            GLuint modelMatrixLoc = glGetUniformLocation(shaderProgram, "modelMatrix");
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(obje.modelMatrix));

            glBindVertexArray(obje.VertexArray);
            glDrawElements(GL_TRIANGLES, obje.indices.size(), GL_UNSIGNED_INT, 0);
        }

        glUniform1i(glGetUniformLocation(shaderProgram, "numLights"), Lights.size());
        for (int i = 0; i < Lights.size(); ++i) {
            glUniform3fv(glGetUniformLocation(shaderProgram, ("lights[" + std::to_string(i) + "].pos_or_dir").c_str()), 1, &Lights[i].pos_or_dir[0]);
            glUniform3fv(glGetUniformLocation(shaderProgram, ("lights[" + std::to_string(i) + "].color").c_str()), 1, &Lights[i].color[0]);
            glUniform1f(glGetUniformLocation(shaderProgram, ("lights[" + std::to_string(i) + "].intensity").c_str()), Lights[i].intensity);
            glUniform3fv(glGetUniformLocation(shaderProgram, ("lights[" + std::to_string(i) + "].ambient").c_str()), 1, &Lights[i].ambient[0]);
            glUniform3fv(glGetUniformLocation(shaderProgram, ("lights[" + std::to_string(i) + "].specular").c_str()), 1, &Lights[i].specular[0]);
            glUniform3fv(glGetUniformLocation(shaderProgram, ("lights[" + std::to_string(i) + "].diffuse").c_str()), 1, &Lights[i].diffuse[0]);
            glUniform1i(glGetUniformLocation(shaderProgram, ("lights[" + std::to_string(i) + "].isDirectional").c_str()), Lights[i].isDirectional);
            glUniform1i(glGetUniformLocation(shaderProgram, ("lights[" + std::to_string(i) + "].isPoint").c_str()), Lights[i].isPoint);
            glUniform1f(glGetUniformLocation(shaderProgram, ("lights[" + std::to_string(i) + "].constantPoint").c_str()), Lights[i].constantPoint);
            glUniform1f(glGetUniformLocation(shaderProgram, ("lights[" + std::to_string(i) + "].linearPoint").c_str()), Lights[i].linearPoint);
            glUniform1f(glGetUniformLocation(shaderProgram, ("lights[" + std::to_string(i) + "].quadraticPoint").c_str()), Lights[i].quadraticPoint);
            glUniform1f(glGetUniformLocation(shaderProgram, ("lights[" + std::to_string(i) + "].ID").c_str()), Lights[i].ID);
        }

        glUseProgram(0);
    }

    void RenderingLayer::AddLight(float id, Light theLight) {
        Light a = theLight;
        a.ID = id;
        Lights.push_back(a);
    }

    Light* RenderingLayer::returnLight(float id) {
        for (int i = 0; i < Lights.size(); i++) {
            if (Lights[i].ID == id) {
                return &Lights[i];
            }
        }
        return nullptr;
    }

    void RenderingLayer::RemoveLight(float id) {
        for (int i = 0; i < Lights.size(); i++) {
            if (Lights[i].ID == id) {
                Lights.erase(Lights.begin() + i);
            }
        }
    }
}