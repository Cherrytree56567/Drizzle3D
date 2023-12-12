#include "RenderingLayer.h"

namespace Drizzle3D {
    std::pair<std::vector<float>, std::vector<unsigned int>> LoadObjFile(const std::string& objFilePath, const std::string& materialFilePath) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        std::vector<glm::vec3> normals;

        // Default color (white)
        float defaultColor[3] = { 1.0f, 1.0f, 1.0f };

        // Load material color if a material file is provided
        if (!materialFilePath.empty()) {
            std::ifstream materialFile(materialFilePath);
            if (materialFile.is_open()) {
                for (int i = 0; i < 3; ++i) {
                    materialFile >> defaultColor[i];
                }
                materialFile.close();
            }
            else {
                std::cerr << "Error: Unable to open material file " << materialFilePath << std::endl;
            }
        }

        // Load OBJ file
        std::ifstream objFile(objFilePath);
        if (objFile.is_open()) {
            std::string line;
            while (std::getline(objFile, line)) {
                std::istringstream iss(line);
                std::string token;
                iss >> token;

                if (token == "v") {
                    float x, y, z;
                    iss >> x >> y >> z;

                    // Append vertex position to the vertices vector
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);

                    // Append default color to the vertices vector
                    vertices.insert(vertices.end(), defaultColor, defaultColor + 3);

                    // Initialize vertex normal to zero vector
                    normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
                }
                else if (token == "vn") {
                    float nx, ny, nz;
                    iss >> nx >> ny >> nz;

                    // Store vertex normals
                    normals.push_back(glm::vec3(nx, ny, nz));
                }
                else if (token == "f") {
                    // Parse face indices
                    unsigned int index1, index2, index3;
                    unsigned int normalIndex1, normalIndex2, normalIndex3;
                    iss >> index1 >> std::ws;
                    iss.ignore(); // skip the '/'
                    iss >> normalIndex1;
                    iss >> index2 >> std::ws;
                    iss.ignore();
                    iss >> normalIndex2;
                    iss >> index3 >> std::ws;
                    iss.ignore();
                    iss >> normalIndex3;

                    // Adjust indices (OBJ format uses 1-based indices)
                    indices.push_back(index1 - 1);
                    indices.push_back(index2 - 1);
                    indices.push_back(index3 - 1);
                }
            }

            objFile.close();

            // Normalize vertex normals
            for (size_t i = 0; i < normals.size(); ++i) {
                normals[i] = glm::normalize(normals[i]);
            }
        }
        else {
            std::cerr << "Error: Unable to open OBJ file " << objFilePath << std::endl;
        }

        // Combine vertex positions and normals into a single vector
        std::vector<float> combinedVertices;
        for (size_t i = 0; i < indices.size(); ++i) {
            size_t vertexIndex = indices[i] * 3; // Each vertex has 3 components (x, y, z)

            combinedVertices.push_back(vertices[vertexIndex]);
            combinedVertices.push_back(vertices[vertexIndex + 1]);
            combinedVertices.push_back(vertices[vertexIndex + 2]);

            size_t normalIndex = indices[i] * 3; // Each normal has 3 components (nx, ny, nz)

            combinedVertices.push_back(normals[indices[i]].x);
            combinedVertices.push_back(normals[indices[i]].y);
            combinedVertices.push_back(normals[indices[i]].z);
        }

        return std::make_pair(combinedVertices, indices);
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