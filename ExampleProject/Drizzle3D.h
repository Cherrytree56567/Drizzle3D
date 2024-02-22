#pragma once
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/mat4x3.hpp"
#include "glm/mat4x2.hpp"
#include "glm/gtc/quaternion.hpp"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include <any>
//#include <GLAD/glad.h>
#include <vector>
//#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui.h"
typedef unsigned int GLuint;
typedef struct GLFWwindow GLFWwindow;
#define AddEventCallback dispatcher()->AddEventListener
#define EWindowClose Drizzle3D::EventType::WindowClose
#define EWindowResize Drizzle3D::EventType::WindowResize
#define EWindowFocus Drizzle3D::EventType::WindowFocus
#define EWindowLostFocus Drizzle3D::EventType::WindowLostFocus
#define EWindowMoved Drizzle3D::EventType::WindowMoved
#define EAppTick Drizzle3D::EventType::AppTick
#define EAppUpdate Drizzle3D::EventType::AppUpdate
#define EAppRender Drizzle3D::EventType::AppRender
#define EKeyPressed Drizzle3D::EventType::KeyPressed
#define EKeyReleased Drizzle3D::EventType::KeyReleased
#define EMouseLeftButtonPressed Drizzle3D::EventType::MouseLeftButtonPressed
#define EMouseLeftButtonReleased Drizzle3D::EventType::MouseLeftButtonReleased
#define EMouseRightButtonPressed Drizzle3D::EventType::MouseRightButtonPressed
#define EMouseRightButtonReleased Drizzle3D::EventType::MouseRightButtonReleased
#define EMouseMoved Drizzle3D::EventType::MouseMoved

#ifdef Drizzle3D_EXPORTS
#define Drizzle3D_API __declspec(dllexport)
#else
#define Drizzle3D_API __declspec(dllimport)
#endif

Drizzle3D_API int TestProgram();

namespace Drizzle3D {
    /*
    * Logging
    */

    class Drizzle3D_API Logging {
    public:
        Logging() {}

        void Error(std::string message, std::string who = "[Drizzle3D::Core] ");
        void Warning(std::string message, std::string who = "[Drizzle3D::Core] ");
        void Info(std::string message, std::string who = "[Drizzle3D::Core] ");
    };

    /*
    * Event
    */

    enum EventType {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    class Drizzle3D_API Event {
    public:
        virtual ~Event() = default;

        bool Handled = false;

        virtual EventType GetEventType() = 0;
    };

    class Drizzle3D_API EventDispatcher {
    public:
        EventDispatcher(const EventDispatcher&) = delete;
        EventDispatcher& operator=(const EventDispatcher&) = delete;
        EventDispatcher() = default;
        EventDispatcher& operator=(EventDispatcher&&) = default;
        EventDispatcher(EventDispatcher&&) = default;
        typedef void (*EventCallback)(GLFWwindow* app, std::unique_ptr<Event> events, std::any);
        void AddEvent(std::unique_ptr<Event> newEvent);
        std::unique_ptr<Event> GetEvent(EventType eventType);
        void RemoveHandledEvents();
        bool isUnhandledEvent(EventType eventType);
        void ResetEvents();
        void AddEventListener(EventType eventType, EventCallback callback, std::any a = NULL);
        void RemoveEventListener(EventType eventType, EventCallback callback);
        void DispatchEvent(GLFWwindow* window);
    private:
        std::vector<std::unique_ptr<Event>> events;
        std::unordered_map<EventType, std::vector<std::pair<EventCallback, std::any>>> eventCallbacks;
        Logging log;
    };

    /*
    * Event Types
    */

    enum KeyCode {
        Space = 32,
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,

        D0 = 48,
        D1 = 49,
        D2 = 50,
        D3 = 51,
        D4 = 52,
        D5 = 53,
        D6 = 54,
        D7 = 55,
        D8 = 56,
        D9 = 57,

        Semicolon = 59,
        Equal = 61,

        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,

        LeftBracket = 91,
        Backslash = 92,
        RightBracket = 93,
        GraveAccent = 96,

        World1 = 161,
        World2 = 162,

        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,

        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,
        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,

        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    };

    enum MouseCode {
        Button1 = 0,
        Button2 = 1,
        Button3 = 2,
        Button4 = 3,
        Button5 = 4,
        Button6 = 5,
        Button7 = 6,
        Button8 = 7,

        ButtonLast = Button8,
        ButtonLeft = Button1,
        ButtonRight = Button2,
        ButtonMiddle = Button3
    };
    
    class Drizzle3D_API WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

        unsigned int GetWidth() const { return m_Width; }
        unsigned int GetHeight() const { return m_Height; }

        EventType GetEventType() override { return EventType::WindowResize; }
    private:
        unsigned int m_Width, m_Height;
    };

    class Drizzle3D_API WindowMoveEvent : public Event {
    public:
        WindowMoveEvent(unsigned int x, unsigned int y) : m_x(x), m_y(y) {}

        unsigned int GetWidth() const { return m_x; }
        unsigned int GetHeight() const { return m_y; }
        EventType GetEventType() override { return EventType::WindowMoved; }
    private:
        unsigned int m_x, m_y;
    };

    class Drizzle3D_API WindowFocusEvent : public Event {
    public:
        WindowFocusEvent(bool isFocused) : m_is_focused(isFocused) {}

        bool IsFocused() const { return m_is_focused; }
        EventType GetEventType() override { return EventType::WindowFocus; }
    private:
        bool m_is_focused;
    };

    class Drizzle3D_API WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;

        EventType GetEventType() override { return EventType::WindowClose; }
    };

    class Drizzle3D_API AppTickEvent : public Event {
    public:
        AppTickEvent() = default;

        EventType GetEventType() override { return EventType::AppTick; }
    };

    class Drizzle3D_API AppUpdateEvent : public Event {
    public:
        AppUpdateEvent() = default;

        EventType GetEventType() override { return EventType::AppUpdate; }
    };

    class Drizzle3D_API AppRenderEvent : public Event {
    public:
        AppRenderEvent() = default;

        EventType GetEventType() override { return EventType::AppRender; }
    };

    class Drizzle3D_API KeyPressedEvent : public Event {
    public:
        KeyPressedEvent(const KeyCode keycode) : m_KeyCode(keycode) {}

        KeyCode GetKeyCode() { return m_KeyCode; }

        EventType GetEventType() override { return EventType::KeyPressed; }
    private:
        KeyCode m_KeyCode;
    };

    class Drizzle3D_API KeyReleasedEvent : public Event {
    public:
        KeyReleasedEvent(const KeyCode keycode) : m_KeyCode(keycode) {}

        KeyCode GetKeyCode() { return m_KeyCode; }

        EventType GetEventType() override { return EventType::KeyReleased; }
    private:
        KeyCode m_KeyCode;
    };

    class Drizzle3D_API MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(const float x, const float y) : m_MouseX(x), m_MouseY(y) {}

        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }

        EventType GetEventType() override { return EventType::MouseMoved; }
    private:
        float m_MouseX, m_MouseY;
    };

    class Drizzle3D_API MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(const float xOffset, const float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

        float GetXOffset() const { return m_XOffset; }
        float GetYOffset() const { return m_YOffset; }

        EventType GetEventType() override { return EventType::MouseScrolled; }
    private:
        float m_XOffset, m_YOffset;
    };

    class Drizzle3D_API MouseButtonPressedEvent : public Event {
    public:
        MouseButtonPressedEvent(const MouseCode button) : m_Button(button) {}

        MouseCode GetMouseButton() const { return m_Button; }

        EventType GetEventType() override { return EventType::MouseButtonPressed; }
    private:
        MouseCode m_Button;
    };

    class Drizzle3D_API MouseButtonReleasedEvent : public Event {
    public:
        MouseButtonReleasedEvent(const MouseCode button) : m_Button(button) {}

        MouseCode GetMouseButton() const { return m_Button; }

        EventType GetEventType() override { return EventType::MouseButtonReleased; }
    private:
        MouseCode m_Button;
    };

    /*
    * Resource Manager
    */
    struct Resource {
        std::string content;
        const char mode[2];
    };

    class Drizzle3D_API ResourceManager {
    public:
        Resource loadFile(const std::string& filePath, const char mode[2]);

        bool fileExists(const std::string& filePath) const;

        void writeFile(const std::string& filePath, const std::string& content);

        std::string& getTempFileContent(const std::string& filePath);
    private:
        std::unordered_map<std::string, std::string> resources;
    };

    /*
    * Window
    */

    class Drizzle3D_API Window {
    public:
        Window(EventDispatcher* dispatch, char* WindowName = (char*)"New Drizzle3D Game", int width = 800, int height = 600);
        ~Window();

        GLFWwindow* returnwindow();
        int returnWidth();
        int returnHeight();
        int returnX();
        int returnY();
        std::vector<int> returnKeyPressedCodes();
        void clearKeyCodes();
        std::vector<int> returnKeyReleasedCodes();
        void clearKeyReleasedCodes();
        double returnMouseX();
        double returnMouseY();

        void ProcessEvents();
        void Render();

        EventDispatcher* dispatcher;
    };

    /*
    * Layer
    */

    class Drizzle3D_API Layer {
    public:
        Layer() {}

        Layer(Window* window) : name("Layer"), pWindow(window) {}

        virtual ~Layer() = default;
        virtual void OnAttach() { }
        virtual void OnDetach() { }
        virtual void Render() { }

        virtual bool IsShown() const { return show; }
        virtual const std::string& GetName() const { return name; }
        virtual void SetShow(bool value) { show = value; }
    private:
        bool show;
        std::string name;
        Window* pWindow;
    };

    class Drizzle3D_API LayerDispatch {
    public:
        void AddLayer(std::shared_ptr<Layer> layer);
        void RemoveLayerByName(const std::string& name);
        void ShowHideLayerByName(const std::string& name, bool show);
        void PushFront(const std::string& name);
        void PushForward(const std::string& name);
        void PushBack(const std::string& name);
        void PushBackward(const std::string& name);

        void DispatchLayerRender();
        void DispatchLayerDetach();
        void DispatchLayerAttach();

    private:
        std::vector<std::shared_ptr<Layer>> layers;
    };

    /*
    * Rendering Layer
    */

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
        GLuint mat = NULL;
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
        RenderingLayer(Window* window, ResourceManager* resmgr);

        void OnAttach() override;
        void OnDetach() override;
        void Render() override;

        bool IsShown() const override;
        const std::string& GetName() const override;
        void SetShow(bool value) override;

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
        char* GetActiveCamera();
        Camera ReturnActiveCamera();
        Camera GetCameraFromID(char* cam);
        ResourceManager& getResourceManager();

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
        ResourceManager* resourcemgr;
    };

    /*
    * ImGui Layer
    */

    struct SliderFloat {
        const char* label;
        float* v;
        float v_min;
        float v_max;
        const char* format = NULL;
        int flags = NULL;
    };

    class Drizzle3D_API ImGuiLayer : public Layer {
    public:
        ImGuiLayer(Window* window) : name("ImGUI"), show(true), pWindow(window) {}

        typedef void (*ImGUICode)(std::shared_ptr<ImGuiLayer> igui);

        ImGUICode code = [](std::shared_ptr<ImGuiLayer> igui) {};

        void OnAttach() override;
        void OnDetach() { }
        void Render() override;

        bool IsShown() const override { return show; }
        const std::string& GetName() const override { return name; }
        void SetShow(bool value) override { show = value; }
        void setIGUI(std::shared_ptr<ImGuiLayer> ig) { igui = ig; }
        void IterateSliderFloat();
        void GUISliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = NULL, int flags = NULL);
        ImGuiContext* imguiContext = ImGui::GetCurrentContext();

    private:
        bool show;
        std::string name;
        Window* pWindow;
        std::shared_ptr<ImGuiLayer> igui;
        std::vector<SliderFloat> SliderFloats;
    };

    /*
    * App
    */

    class Drizzle3D_API App {
    public:
        App(char* WindowName = (char*)"New Drizzle3D Game", int width = 800, int height = 600);

        void Run();

        Window* window() { return &D3DWindow; }
        std::shared_ptr<ImGuiLayer> ImguiLayer() { return imguilayer; }
        std::shared_ptr<RenderingLayer> GetRenderingLayer() { return renderinglayer; }
        EventDispatcher* dispatcher() { return &dispatch; }

        typedef void(*UpdateFunc)(App* myApp);
        UpdateFunc update = [](App* myApp) {};

    private:
        Window D3DWindow;

        // Layers
        std::shared_ptr<ImGuiLayer> imguilayer;
        std::shared_ptr<RenderingLayer> renderinglayer;

        // Dispatchers
        EventDispatcher dispatch;
        LayerDispatch LayerDispatcher;

        // Managers
        ResourceManager resourcemgr;
    };

    /*
    * First Person Camera
    */

    class Drizzle3D_API FirstPersonCamera {
    public:
        FirstPersonCamera(App& app);

        glm::vec3 position = glm::vec3(0, 0, 5);
        float horizontalAngle = 0.0f;
        float verticalAngle = 0.0f;
        float initialFoV = 450.0f;

        float speed = 1.0f; // 3 units / second
        float mouseSpeed = 0.005f;
        App& application;
        bool capture = true;
        double xpos, ypos = 0.0;
    };

    /*
    * Skybox
    */

    class Drizzle3D_API Skybox {
    public:
        Skybox(App& app, const char* skyboxtex, float size = 100.0f);
        void Update();

    private:
        App& application;
        glm::vec3 pos;
    };

    /*
    * Material
    */

    class Drizzle3D_API Material {
    public:
        Material(ResourceManager resourcemgr, const char* fname, const char* fgname);
        GLuint GetShaderProgram();
    };
}
