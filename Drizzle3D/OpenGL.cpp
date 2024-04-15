#include "RenderingLayer.h"

namespace Drizzle3D {
	void RenderingLayer::InitGlRendering() {
        glEnable(GL_DEPTH_TEST);
        glGenBuffers(1, &lightsBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, lightsBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * 500, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightsBuffer);  // Binding to the binding point 0
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        GLuint projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");
        const char* vertexshader = "#version 330 core\n\nlayout(location = 0) in vec3 aPos;\nlayout(location = 1) in vec2 aTexCoord;\nlayout(location = 2) in vec3 aNormal;\n\nout vec2 texCoord;\nout vec3 Normal;\nout vec3 FragPos;\n\nuniform mat4 projectionMatrix;\nuniform mat4 viewMatrix;\nuniform mat4 modelMatrix;\n\nvoid main() {\n    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);\n    texCoord = aTexCoord;\n    Normal = aNormal;\n    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));\n}";
        const char* fragshader = "#version 330 core\n#define MAX_LIGHTS 10\nin vec2 texCoord;in vec3 Normal;in vec3 FragPos;out vec4 FragColor;struct Light {vec3 direction;vec3 position;vec3 color;float strength;float SpecularStrength;vec3 ambient;vec3 diffuse;vec3 specular;int type;int id;float constant;float linear;float quadratic;};uniform sampler2D textureSampler;uniform vec3 viewPos;uniform Light lights[MAX_LIGHTS];uniform int numLights;uniform bool IsSkyBox;void main() {vec3 ambient = vec3(0.0);vec3 diffuse = vec3(0.0);vec3 specular = vec3(0.0);vec3 norm = normalize(Normal);vec3 viewDir = normalize(viewPos - FragPos);for (int i = 0; i < numLights; ++i) {vec3 lightDir;float attenuation = 1.0;if (lights[i].type == 0) {lightDir = normalize(-lights[i].direction);} else if (lights[i].type == 1) {lightDir = normalize(lights[i].position - FragPos);float distance = length(lights[i].position - FragPos);attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));}float adjustedStrength = lights[i].strength * attenuation;ambient += adjustedStrength * lights[i].color * lights[i].ambient;float diff = max(dot(norm, lightDir), 0.0);diffuse += diff * lights[i].diffuse * lights[i].color;vec3 reflectDir = reflect(-lightDir, norm);float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);specular += attenuation * lights[i].SpecularStrength * spec * lights[i].specular * lights[i].color;}vec3 result = ambient + diffuse + specular;if (IsSkyBox == true) {FragColor = texture(textureSampler, texCoord);} else {FragColor = vec4(result, 1.0f) * texture(textureSampler, texCoord);}}";

        shaderProgram = std::get<GLuint>(Create_DefaultShader(vertexshader, fragshader));
        OldshaderProgram = shaderProgram;
        glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUseProgram(0);
	}
    
    void RenderingLayer::DrawVertGLRendering(Object &myOBJ) {

        glGenVertexArrays(1, &myOBJ.VertexArray);
        glGenBuffers(1, &myOBJ.VertexBuffer);
        glGenBuffers(1, &myOBJ.IndexBuffer);

        // Bind the VAO
        glBindVertexArray(myOBJ.VertexArray);

        // Bind the VBO and set vertex data
        glBindBuffer(GL_ARRAY_BUFFER, myOBJ.VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, myOBJ.vertices.size() * sizeof(float), myOBJ.vertices.data(), GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Set vertex attribute pointers for Tex Coords
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Bind the EBO and set index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myOBJ.IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, myOBJ.indices.size() * sizeof(unsigned int), myOBJ.indices.data(), GL_STATIC_DRAW);

        // Unbind VAO to prevent accidentally modifying it elsewhere
        glBindVertexArray(0);
    }

    void RenderingLayer::RenderInitGlRendering() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 150.0f);
        glm::mat4 viewMatrix = glm::lookAt(returnCamera(current_camera)->position, returnCamera(current_camera)->look_at_position, returnCamera(current_camera)->up);
        //glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 cameraPosition = glm::inverse(viewMatrix)[3];

        GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
        GLuint projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");
        GLuint viewPos = glGetUniformLocation(shaderProgram, "viewPos");

        glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(viewPos, 1, GL_FALSE, glm::value_ptr(cameraPosition));

        GLuint numLightsLoc = glGetUniformLocation(shaderProgram, "numLights");
        GLuint lightsArrayLoc = glGetUniformLocation(shaderProgram, "lights");

        // Set the uniform values for the number of lights
        glUniform1i(numLightsLoc, static_cast<int>(Lights.size()));

        // Loop through each light in the array and set its values
        for (size_t i = 0; i < Lights.size(); ++i) {
            std::string lightPrefix = "lights[" + std::to_string(i) + "]";

            glUniform3fv(glGetUniformLocation(shaderProgram, (lightPrefix + ".direction").c_str()), 1, glm::value_ptr(Lights[i].direction));
            glUniform3fv(glGetUniformLocation(shaderProgram, (lightPrefix + ".position").c_str()), 1, glm::value_ptr(Lights[i].position));
            glUniform3fv(glGetUniformLocation(shaderProgram, (lightPrefix + ".color").c_str()), 1, glm::value_ptr(Lights[i].color));
            glUniform1f(glGetUniformLocation(shaderProgram, (lightPrefix + ".strength").c_str()), Lights[i].strength);
            glUniform1f(glGetUniformLocation(shaderProgram, (lightPrefix + ".specularStrength").c_str()), Lights[i].SpecularStrength);
            glUniform3fv(glGetUniformLocation(shaderProgram, (lightPrefix + ".ambient").c_str()), 1, glm::value_ptr(Lights[i].ambient));
            glUniform3fv(glGetUniformLocation(shaderProgram, (lightPrefix + ".diffuse").c_str()), 1, glm::value_ptr(Lights[i].diffuse));
            glUniform3fv(glGetUniformLocation(shaderProgram, (lightPrefix + ".specular").c_str()), 1, glm::value_ptr(Lights[i].specular));
            glUniform1i(glGetUniformLocation(shaderProgram, (lightPrefix + ".type").c_str()), Lights[i].type);
            glUniform1i(glGetUniformLocation(shaderProgram, (lightPrefix + ".id").c_str()), Lights[i].id);
            glUniform1f(glGetUniformLocation(shaderProgram, (lightPrefix + ".constant").c_str()), Lights[i].constant);
            glUniform1f(glGetUniformLocation(shaderProgram, (lightPrefix + ".linear").c_str()), Lights[i].linear);
            glUniform1f(glGetUniformLocation(shaderProgram, (lightPrefix + ".quadratic").c_str()), Lights[i].quadratic);
        }
        for (const auto& obje : Objects) {
        if (obje.hide) {
            continue;
        } else {
            if (obje.mat != shaderProgram) {
                glUseProgram(obje.mat);
                GLuint viewMatrixLoc = glGetUniformLocation(obje.mat, "viewMatrix");
                GLuint projectionMatrixLoc = glGetUniformLocation(obje.mat, "projectionMatrix");
                GLuint viewPos = glGetUniformLocation(obje.mat, "viewPos");

                glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
                glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
                glUniformMatrix4fv(viewPos, 1, GL_FALSE, glm::value_ptr(cameraPosition));

                GLuint numLightsLoc = glGetUniformLocation(obje.mat, "numLights");
                GLuint lightsArrayLoc = glGetUniformLocation(obje.mat, "lights");

                // Set the uniform values for the number of lights
                glUniform1i(numLightsLoc, static_cast<int>(Lights.size()));

                // Loop through each light in the array and set its values
                for (size_t i = 0; i < Lights.size(); ++i) {
                    std::string lightPrefix = "lights[" + std::to_string(i) + "]";

                    glUniform3fv(glGetUniformLocation(obje.mat, (lightPrefix + ".direction").c_str()), 1, glm::value_ptr(Lights[i].direction));
                    glUniform3fv(glGetUniformLocation(obje.mat, (lightPrefix + ".position").c_str()), 1, glm::value_ptr(Lights[i].position));
                    glUniform3fv(glGetUniformLocation(obje.mat, (lightPrefix + ".color").c_str()), 1, glm::value_ptr(Lights[i].color));
                    glUniform1f(glGetUniformLocation(obje.mat, (lightPrefix + ".strength").c_str()), Lights[i].strength);
                    glUniform1f(glGetUniformLocation(obje.mat, (lightPrefix + ".specularStrength").c_str()), Lights[i].SpecularStrength);
                    glUniform3fv(glGetUniformLocation(obje.mat, (lightPrefix + ".ambient").c_str()), 1, glm::value_ptr(Lights[i].ambient));
                    glUniform3fv(glGetUniformLocation(obje.mat, (lightPrefix + ".diffuse").c_str()), 1, glm::value_ptr(Lights[i].diffuse));
                    glUniform3fv(glGetUniformLocation(obje.mat, (lightPrefix + ".specular").c_str()), 1, glm::value_ptr(Lights[i].specular));
                    glUniform1i(glGetUniformLocation(obje.mat, (lightPrefix + ".type").c_str()), Lights[i].type);
                    glUniform1i(glGetUniformLocation(obje.mat, (lightPrefix + ".id").c_str()), Lights[i].id);
                    glUniform1f(glGetUniformLocation(obje.mat, (lightPrefix + ".constant").c_str()), Lights[i].constant);
                    glUniform1f(glGetUniformLocation(obje.mat, (lightPrefix + ".linear").c_str()), Lights[i].linear);
                    glUniform1f(glGetUniformLocation(obje.mat, (lightPrefix + ".quadratic").c_str()), Lights[i].quadratic);
                }
                if (obje.name == "Skybox") {
                    GLuint IsSkyBox = glGetUniformLocation(obje.mat, "IsSkyBox");

                    glUniform1i(IsSkyBox, static_cast<bool>(true));
                }
                else {
                    GLuint IsSkyBox = glGetUniformLocation(obje.mat, "IsSkyBox");

                    glUniform1i(IsSkyBox, static_cast<bool>(false));
                }
                if (Lighting == false) {
                    GLuint IsSkyBox = glGetUniformLocation(obje.mat, "IsSkyBox");

                    glUniform1i(IsSkyBox, static_cast<bool>(true));
                }
                glBindTexture(GL_TEXTURE_2D, obje.textureID);
                glUniform1i(glGetUniformLocation(obje.mat, "textureSampler"), 0);

                GLuint modelMatrixLoc = glGetUniformLocation(obje.mat, "modelMatrix");
                glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(obje.modelMatrix));
            }
            else {
                glUseProgram(shaderProgram);
                if (obje.name == "Skybox") {
                    GLuint IsSkyBox = glGetUniformLocation(shaderProgram, "IsSkyBox");

                    glUniform1i(IsSkyBox, static_cast<bool>(true));
                }
                else {
                    GLuint IsSkyBox = glGetUniformLocation(shaderProgram, "IsSkyBox");

                    glUniform1i(IsSkyBox, static_cast<bool>(false));
                }

                if (Lighting == false) {
                    GLuint IsSkyBox = glGetUniformLocation(shaderProgram, "IsSkyBox");

                    glUniform1i(IsSkyBox, static_cast<bool>(true));
                }
                glBindTexture(GL_TEXTURE_2D, obje.textureID);
                glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);

                GLuint modelMatrixLoc = glGetUniformLocation(shaderProgram, "modelMatrix");
                glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(obje.modelMatrix));
            }

            glBindVertexArray(obje.VertexArray);
            glDrawElements(GL_TRIANGLES, obje.indices.size(), GL_UNSIGNED_INT, 0);
        }
        }

        glUseProgram(0);
        glDisable(GL_CULL_FACE);
    }

    GLuint RenderingLayer::Create_OpenGLShader(const char* fname, const char* fgname) {
        Resource frag = resourcemgr->loadFile(fname, "r");

        const char* VSSource = frag.content.c_str();

        Resource vert = resourcemgr->loadFile(fgname, "r");

        const char* FSSource = vert.content.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &VSSource, 0);
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            std::cerr << "[Drizzle3D::Core::RenderingLayer] Error: Error compiling Vertex Shader" << std::endl;

            // Print compilation log
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);
            std::cerr << "Vertex Shader Compilation Log:\n" << &errorLog[0] << std::endl;

            return (GLuint)0;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &FSSource, NULL);
        glCompileShader(fragmentShader);

        GLint isCompileda = 0;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompileda);
        if (isCompileda == GL_FALSE) {
            std::cerr << "[Drizzle3D::Core::RenderingLayer] Error: Error compiling Fragment Shader" << std::endl;

            // Print compilation log
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);
            std::cerr << "Fragment Shader Compilation Log:\n" << &errorLog[0] << std::endl;

            return (GLuint)0;
        }

        GLuint shadrProgram = glCreateProgram();
        glAttachShader(shadrProgram, vertexShader);
        glAttachShader(shadrProgram, fragmentShader);
        glLinkProgram(shadrProgram);

        return shadrProgram;
    }

    GLuint RenderingLayer::Create_DefaultOpenGLShader(const char* fname, const char* fgname) {
        const char* VSSource = fname;
        const char* FSSource = fgname;

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &VSSource, 0);
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            std::cerr << "[Drizzle3D::Core::RenderingLayer] Error: Error compiling Vertex Shader" << std::endl;

            // Print compilation log
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);
            std::cerr << "Vertex Shader Compilation Log:\n" << &errorLog[0] << std::endl;

            return (GLuint)0;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &FSSource, NULL);
        glCompileShader(fragmentShader);

        GLint isCompileda = 0;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompileda);
        if (isCompileda == GL_FALSE) {
            std::cerr << "[Drizzle3D::Core::RenderingLayer] Error: Error compiling Fragment Shader" << std::endl;

            // Print compilation log
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);
            std::cerr << "Fragment Shader Compilation Log:\n" << &errorLog[0] << std::endl;

            return (GLuint)0;
        }

        GLuint shadrProgram = glCreateProgram();
        glAttachShader(shadrProgram, vertexShader);
        glAttachShader(shadrProgram, fragmentShader);
        glLinkProgram(shadrProgram);

        return shadrProgram;
    }
}
