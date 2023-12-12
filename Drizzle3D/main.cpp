#include <GLAD/glad.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "App.h"

std::vector<float> vertices = {
    // Front face
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,

    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,

    // Top face
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,

    // Bottom face
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

    // Right face
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,

     // Left face
     -0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
     -0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
};

// Indices for a simple cube
std::vector<unsigned int> indices = {
    0, 1, 2, 2, 3, 0,  // Front face
    4, 5, 6, 6, 7, 4,  // Back face
    8, 9, 10, 10, 11, 8,  // Top face
    12, 13, 14, 14, 15, 12,  // Bottom face
    16, 17, 18, 18, 19, 16,  // Right face
    20, 21, 22, 22, 23, 20,  // Left face
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
    app.GetRenderingLayer()->AddObject("Duck", app.GetRenderingLayer()->DrawVerts(vf, modelMatrix));

    //app.ImguiLayer()->SetShow(false);    

    app.Run();

    return 0;
}