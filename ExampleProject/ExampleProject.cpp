#include <iostream>
#include "Drizzle3D.h"

glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 modelMatrixa = glm::mat4(1.0f);
glm::vec3 light_pos;
glm::vec3 camera_pos;
glm::vec3 camera_up_pos;
glm::vec3 camera_la_pos;
char* cam;
float streg = 0.0f;
bool use_ios = true;
bool sld = false;
bool sh = true;

void Closed(GLFWwindow* window, std::unique_ptr<Drizzle3D::Event> ev, std::any a) {
    std::cout << "Closeda";
}

void ImGUICode(std::shared_ptr<Drizzle3D::ImGuiLayer> rend) {
    ImGui::SetCurrentContext(rend->imguiContext);
    glm::vec3 rotation = glm::degrees(glm::eulerAngles(glm::quat_cast(modelMatrix)));
    glm::vec3 position = modelMatrix[3];

    // Create sliders for x, y, and z rotation values
    ImGui::SliderFloat("Rotation X", &rotation.x, 0.0f, 360.0f);
    ImGui::SliderFloat("Rotation Y", &rotation.y, 0.0f, 360.0f);
    ImGui::SliderFloat("Rotation Z", &rotation.z, 0.0f, 360.0f);

    ImGui::SliderFloat("Position X", &position.x, 0.0f, 360.0f);
    ImGui::SliderFloat("Position Y", &position.y, 0.0f, 360.0f);
    ImGui::SliderFloat("Position Z", &position.z, -360.0f, 360.0f);

    if (ImGui::Button("Ios Texture")) {
        use_ios = true;
    }

    if (ImGui::Button("Duck Texture")) {
        use_ios = false;
    }

    // Update the model matrix with the new rotation values
    modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    modelMatrix = glm::translate(modelMatrix, position);

    ImGui::SliderFloat("Light X", &light_pos.x, -50.0f, 50.0f);
    ImGui::SliderFloat("Light Y", &light_pos.y, -50.0f, 50.0f);
    ImGui::SliderFloat("Light Z", &light_pos.z, -50.0f, 500.0f);

    ImGui::SliderFloat("Light Intensity", &streg, 0.0f, 360.0f);

    ImGui::SliderFloat("Camera X", &camera_pos.x, 0.0f, 50.0f);
    ImGui::SliderFloat("Camera Y", &camera_pos.y, 0.0f, 50.0f);
    ImGui::SliderFloat("Camera Z", &camera_pos.z, 0.0f, 50.0f);

    ImGui::SliderFloat("Camera Up X", &camera_up_pos.x, 0.0f, 50.0f);
    ImGui::SliderFloat("Camera Up Y", &camera_up_pos.y, 0.0f, 50.0f);
    ImGui::SliderFloat("Camera Up Z", &camera_up_pos.z, 0.0f, 50.0f);

    ImGui::SliderFloat("Camera Look-At X", &camera_la_pos.x, 0.0f, 50.0f);
    ImGui::SliderFloat("Camera Look-At Y", &camera_la_pos.y, 0.0f, 50.0f);
    ImGui::SliderFloat("Camera Look-At Z", &camera_la_pos.z, 0.0f, 50.0f);

    if (ImGui::Button("Fullscreen")) {
        sld = !sld;
    }

    if (ImGui::Button("Show")) {
        sh = !sh;
    }
}

int main() {

    /*
    * NOTE:
    * Key Released and Mouse Released
    */
    std::shared_ptr<Drizzle3D::App> app = std::make_shared<Drizzle3D::App>();

    app->GetRenderingLayer()->GetFlags()->ChangeFlag("Lighting", false);
    app->GetRenderingLayer()->GetFlags()->ChangeFlag("UseOpenGL", false);
    app->GetRenderingLayer()->GetFlags()->ChangeFlag("UseVulkan", true);

    Drizzle3D::FirstPersonCamera fpc(app);
    Drizzle3D::Skybox sky(app, "skybox.png");
    Drizzle3D::Material mat1(app->GetResourceManager(), "Scene1_vertex.glsl", "Scene1_fragment.glsl");

    app->dispatcher()->AddEventListener(EMouseMoved, [](GLFWwindow* window, std::unique_ptr<Drizzle3D::Event> ev, std::any a) {
        std::cout << "Mouse Moved\n";
        });

    app->dispatcher()->AddEventListener(EWindowClose, Closed);

    app->ImguiLayer()->code = ImGUICode;
    app->GetRenderingLayer()->AddObject("Cube", app->GetRenderingLayer()->DrawVerts(Drizzle3D::LoadObjFile("Scene1_Cube.obj"), modelMatrix));
    app->GetRenderingLayer()->returnObject("Cube")->textureID = Drizzle3D::GetTexture("duck.png");
    app->GetRenderingLayer()->returnObject("Cube")->mat = mat1.GetShaderProgram();

    app->GetRenderingLayer()->AddObject("Plane", app->GetRenderingLayer()->DrawVerts(Drizzle3D::LoadObjFile("Scene1_Plane.obj"), modelMatrix));
    app->GetRenderingLayer()->returnObject("Plane")->textureID = Drizzle3D::GetTexture("duck.png");

    app->GetRenderingLayer()->AddObject("Cylinder", app->GetRenderingLayer()->DrawVerts(Drizzle3D::LoadObjFile("Scene1_Cylinder.obj"), modelMatrix));
    app->GetRenderingLayer()->returnObject("Cylinder")->textureID = Drizzle3D::GetTexture("duck.png");
    //app.ImguiLayer()->SetShow(false);    

    Drizzle3D::Camera aCamera = { glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) };
    camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    camera_up_pos = glm::vec3(0.0f, 1.0f, 0.0f);
    app->GetRenderingLayer()->AddCamera("Acam", aCamera);
    //app.GetRenderingLayer()->SwitchCamera("Acam");
    app->GetRenderingLayer()->SwitchCamera("FirstPersonCamera");

    while (app->Run()) {
        app->GetRenderingLayer()->returnObject("Cube")->modelMatrix = modelMatrix;

        if (use_ios == true)
            app->GetRenderingLayer()->returnObject("Cube")->textureID = Drizzle3D::GetTexture("ios.png");

        if (use_ios == false)
            app->GetRenderingLayer()->returnObject("Cube")->textureID = Drizzle3D::GetTexture("duck.png");

        app->GetRenderingLayer()->returnLight(1)->position = light_pos;
        app->GetRenderingLayer()->returnLight(1)->strength = streg;

        app->GetRenderingLayer()->returnCamera("Acam")->position = camera_pos;
        app->GetRenderingLayer()->returnCamera("Acam")->up = camera_up_pos;
        app->GetRenderingLayer()->returnCamera("Acam")->look_at_position = camera_la_pos;
        app->GetRenderingLayer()->GetFlags()->ChangeFlag("Fullscreen", sld);
        app->GetRenderingLayer()->GetFlags()->ChangeFlag("Show", sh);
    }

    return 0;
}

int maina() {
    TestProgram();
    return 0;
}