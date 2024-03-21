/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Layer.h"
#include "Material.h"
#include "ResourceManager.h"
#include "base.h"

namespace Drizzle3D {

    Drizzle3D_API std::pair<std::vector<float>, std::vector<unsigned int>> LoadObjFile(const std::string& filePath);
    Drizzle3D_API GLuint GetTexture(const char* texturePath);

    enum Lights {
        Directional,
        Point
    };

    struct Light {
        glm::vec3 direction;
        glm::vec3 position;
        glm::vec3 color;
        float strength;
        float SpecularStrength;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        Lights type;
        int id;

        float constant;
        float linear;
        float quadratic;
    };

    struct Object {
        GLuint VertexArray, VertexBuffer, IndexBuffer;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        glm::mat4 modelMatrix;
        GLuint textureID = NULL;
        GLuint mat = 0;
        char* name;
    };

    struct Camera {
        glm::vec3 position;
        glm::vec3 look_at_position;
        glm::vec3 up;
        char* ID;
    };

    class Drizzle3D_API RenderingLayer : public Layer {
    public:
        RenderingLayer(Window* window, std::shared_ptr<ResourceManager> resmgr) : name("3DLayer"), show(true), pWindow(window), resourcemgr(resmgr) {}

        void OnAttach() override;
        void OnDetach() override {}
        void Render() override;

        bool IsShown() const override { return show; }
        const std::string& GetName() const override { return name; }
        void SetShow(bool value) override { show = value; }

		void Create_Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
        Object DrawVerts(std::pair<std::vector<float>, std::vector<unsigned int>> vf, glm::mat4 modelMatrix = glm::mat4(1.0f));
        void AddObject(const char* name, Object theObject);
        Object* returnObject(const char* name);
        void RemoveObject(const char* name);
        void AddLight(float id, Light theLight);
        Light* returnLight(float id);
        void RemoveLight(float id);
        void SwitchCamera(const char* name);
        void AddCamera(const char* id, Camera theCamera);
        Camera* returnCamera(const char* id);
        void RemoveCamera(const char* id);
        char* GetActiveCamera() { return current_camera; }
        Camera ReturnActiveCamera();
        Camera GetCameraFromID(char* cam);

        bool Lighting = true;

    private:
        bool show;
        GLuint shaderProgram;
        GLuint OldshaderProgram;
        std::string name;
        Window* pWindow;
        std::vector<Object> Objects;
        std::vector<Light> Lights;
        std::vector<Camera> Cameras;

        GLuint lightsBuffer;
        char* current_camera;
        std::shared_ptr<ResourceManager> resourcemgr;
	};
}