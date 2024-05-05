#pragma once

namespace Drizzle3D {
    struct Object {
        GLuint VertexArray, VertexBuffer, IndexBuffer = 0;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        glm::mat4 modelMatrix;
        glm::mat4 OldmodelMatrix;
        GLuint textureID = NULL;
        GLuint mat = 0;
        char* name = (char*)"PLZ_SPECIFY_A_NAME";
        bool hide = false;
        VkPipeline Pipeline;
    };
}