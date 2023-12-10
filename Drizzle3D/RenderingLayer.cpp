#include "RenderingLayer.h"

namespace Drizzle3D {
    std::pair<std::vector<float>, std::vector<unsigned int>> LoadObjFile(const std::string& objFilePath, const std::string& materialFilePath) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

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

                    // Append vertex position and color to the vertices vector
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);
                    vertices.insert(vertices.end(), defaultColor, defaultColor + 3);
                }
                else if (token == "f") {
                    // Parse face indices
                    unsigned int index1, index2, index3;
                    iss >> index1 >> index2 >> index3;

                    // Indices in OBJ format start from 1, so subtract 1 to convert to 0-based indices
                    indices.push_back(index1 - 1);
                    indices.push_back(index2 - 1);
                    indices.push_back(index3 - 1);
                }
            }

            objFile.close();
        }
        else {
            std::cerr << "Error: Unable to open OBJ file " << objFilePath << std::endl;
        }

        return std::make_pair(vertices, indices);
    }
    void RenderingLayer::OnAttach() {
        // Vertex Array
        // Vertex Buffer
        // Index Buffer
        // Shader (not needed)

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        GLuint projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");
        Create_Shader("VertexShader.glsl", "FragmentShader.glsl");
        glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUseProgram(0);
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Set vertex attribute pointers for color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Bind the EBO and set index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myOBJ.IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, myOBJ.indices.size() * sizeof(unsigned int), myOBJ.indices.data(), GL_STATIC_DRAW);
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

        for (int i = 0; i < Objects.size(); i++) {
            GLuint modelMatrixLoc = glGetUniformLocation(shaderProgram, "modelMatrix");
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(Objects[i].modelMatrix));

            glBindVertexArray(Objects[i].VertexArray);
            glDrawElements(GL_TRIANGLES, Objects[i].indices.size(), GL_UNSIGNED_INT, 0);
        }

        glUseProgram(0);
    }

}