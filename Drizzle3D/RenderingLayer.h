#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Layer.h"

namespace Drizzle3D {
    std::pair<std::vector<float>, std::vector<unsigned int>> LoadObjFile(const std::string& objFilePath, const std::string& materialFilePath = "");
    struct Object {
        GLuint VertexArray, VertexBuffer, IndexBuffer;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        glm::mat4 modelMatrix;
        char* name;
    };

    struct Light {
        glm::vec3 pos_or_dir;
        glm::vec3 color;
        float intensity;

        glm::vec3 ambient;
        glm::vec3 specular;
        glm::vec3 diffuse;

        bool isDirectional;
        bool isPoint;

        float constantPoint;
        float linearPoint;
        float quadraticPoint;

        float ID;
    };


    class RenderingLayer : public Layer {
    public:
        RenderingLayer(Window* window) : name("3DLayer"), show(true), pWindow(window) {}

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

    protected:
        bool show = true;

    private:
        GLuint shaderProgram;
        std::string name;
        Window* pWindow;
        std::vector<Object> Objects;
        std::vector<Light> Lights;
        GLuint lightsBuffer;
	};
}