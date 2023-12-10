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
        void DrawVerts(std::pair<std::vector<float>, std::vector<unsigned int>> vf, glm::mat4 modelMatrix = glm::mat4(1.0f));

    protected:
        bool show = true;

    private:
        GLuint shaderProgram;
        std::string name;
        Window* pWindow;
        Object objects;

		const char* vertexSrc = "(#version 330 core\n\nlayout(location = 0) in vec3 a_Position;\nout vec3 v_Position;\nvoid main()\n{\nv_Position = a_Position;\ngl_Position = vec4(a_Position, 1.0);\n}\n)";

		const char* fragmentSrc = "(#version 330 core\n\nlayout(location = 0) out vec4 color;\nin vec3 v_Position;\nvoid main()\n{\ncolor = vec4(v_Position * 0.5 + 0.5, 1.0);\n})";
	};
}