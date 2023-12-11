#include <GLAD/glad.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "App.h"

std::vector<float> vertices = {
    // Front face
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  // Vertex 0
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  // Vertex 1
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  // Vertex 2
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  // Vertex 3

    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  // Vertex 4
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  // Vertex 5
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  // Vertex 6
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  // Vertex 7

    // Bottom face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // Vertex 8
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // Vertex 9
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // Vertex 10
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // Vertex 11

    // Top face
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // Vertex 12
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // Vertex 13
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // Vertex 14
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // Vertex 15

    // Left face
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // Vertex 16
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // Vertex 17
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // Vertex 18
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // Vertex 19

    // Right face
     0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  // Vertex 20
     0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  // Vertex 21
     0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 0.0f,  // Vertex 22
     0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f   // Vertex 23
};


// Indices for a cube with front, back, and side faces
std::vector<unsigned int> indices = {
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    8, 9, 10,
    10, 11, 8,

    12, 13, 14,
    14, 15, 12,

    16, 17, 18,
    18, 19, 16,

    20, 21, 22,
    22, 23, 20
};

glm::mat4 modelMatrix = glm::mat4(1.0f);

void Closed(GLFWwindow* window) {
    std::cout << "Closeda";
}

void Update(Drizzle3D::App* app) {
    app->GetRenderingLayer()->returnObject("Duck")->modelMatrix = modelMatrix;
}

void ImGUICode(Drizzle3D::ImGuiLayer* rend) {
    glm::vec3 rotation = glm::degrees(glm::eulerAngles(glm::quat_cast(modelMatrix)));

    // Create sliders for x, y, and z rotation values
    ImGui::SliderFloat("Rotation X", &rotation.x, 0.0f, 360.0f);
    ImGui::SliderFloat("Rotation Y", &rotation.y, 0.0f, 360.0f);
    ImGui::SliderFloat("Rotation Z", &rotation.z, 0.0f, 360.0f);

    // Update the model matrix with the new rotation values
    modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

int main() {

    /*
    * NOTE:
    * Fragment Shader is Not Working for Lighting.
    */
    Drizzle3D::App app;

    app.AddEventCallback(EWindowClose, [](GLFWwindow* window) {
        std::cout << "Closed";
    });

    app.AddEventCallback(EWindowMoved, [](GLFWwindow* window) {
        std::cout << "Moved\n";
    });

    app.AddEventCallback(EWindowClose, Closed);

    app.update = Update;

    app.ImguiLayer()->code = ImGUICode;

    std::pair<std::vector<float>, std::vector<unsigned int>> vf(vertices, indices);
    app.GetRenderingLayer()->AddObject("Duck", app.GetRenderingLayer()->DrawVerts(Drizzle3D::LoadObjFile("duck.obj"), modelMatrix));

    //app.ImguiLayer()->SetShow(false);

    app.Run();

    return 0;
}