#pragma once
#include <iostream>
#include <memory>
#include <functional>
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
#include <map>
#include <vector>
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

#ifdef Drizzle3D_LINUX
#define Drizzle3D_API 
#else
#ifdef Drizzle3D_EXPORTS
#define Drizzle3D_API __declspec(dllexport)
#else
#define Drizzle3D_API __declspec(dllimport)
#endif
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
    * Flags
    */

    class Flags {
	public:
		Drizzle3D_API Flags() {}

		Drizzle3D_API void AddFlag(const std::string& name, bool& pointer);

        Drizzle3D_API void ChangeFlag(const std::string& name, const bool newValue);

		Drizzle3D_API bool GetFlag(const std::string& name);

	private:
		std::map<std::string, bool*> flagMap;
		Logging log;
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

    class Event {
    public:
        Drizzle3D_API virtual ~Event() = default;

        bool Handled = false;

        Drizzle3D_API virtual EventType GetEventType() = 0;
    };

    class EventDispatcher {
    public:
        Drizzle3D_API EventDispatcher(const EventDispatcher&) = delete;
        Drizzle3D_API EventDispatcher& operator=(const EventDispatcher&) = delete;
        Drizzle3D_API EventDispatcher() = default;
        Drizzle3D_API EventDispatcher& operator=(EventDispatcher&&) = default;
        Drizzle3D_API EventDispatcher(EventDispatcher&&) = default;
        typedef void (*EventCallback)(GLFWwindow* app, std::unique_ptr<Event> events, std::any);
        Drizzle3D_API void AddEvent(std::unique_ptr<Event> newEvent);
        Drizzle3D_API std::unique_ptr<Event> GetEvent(EventType eventType);
        Drizzle3D_API void RemoveHandledEvents();
        Drizzle3D_API bool isUnhandledEvent(EventType eventType);
        Drizzle3D_API void ResetEvents() { events.clear(); }
        Drizzle3D_API void AddEventListener(EventType eventType, EventCallback callback, std::any a = NULL);
        Drizzle3D_API void RemoveEventListener(EventType eventType, EventCallback callback);
        Drizzle3D_API void DispatchEvent(GLFWwindow* window);
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

    class WindowResizeEvent : public Event {
    public:
        Drizzle3D_API WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

        Drizzle3D_API unsigned int GetWidth() const { return m_Width; }
        Drizzle3D_API unsigned int GetHeight() const { return m_Height; }

        Drizzle3D_API EventType GetEventType() override { return EventType::WindowResize; }
    private:
        unsigned int m_Width, m_Height;
    };

    class WindowMoveEvent : public Event {
    public:
        Drizzle3D_API WindowMoveEvent(unsigned int x, unsigned int y) : m_x(x), m_y(y) {}

        Drizzle3D_API unsigned int GetWidth() const { return m_x; }
        Drizzle3D_API unsigned int GetHeight() const { return m_y; }
        Drizzle3D_API EventType GetEventType() override { return EventType::WindowMoved; }
    private:
        unsigned int m_x, m_y;
    };

    class WindowFocusEvent : public Event {
    public:
        Drizzle3D_API WindowFocusEvent(bool isFocused) : m_is_focused(isFocused) {}

        Drizzle3D_API bool IsFocused() const { return m_is_focused; }
        Drizzle3D_API EventType GetEventType() override { return EventType::WindowFocus; }
    private:
        bool m_is_focused;
    };

    class WindowCloseEvent : public Event {
    public:
        Drizzle3D_API WindowCloseEvent() = default;

        Drizzle3D_API EventType GetEventType() override { return EventType::WindowClose; }
    };

    class AppTickEvent : public Event {
    public:
        Drizzle3D_API AppTickEvent() = default;

        Drizzle3D_API EventType GetEventType() override { return EventType::AppTick; }
    };

    class AppUpdateEvent : public Event {
    public:
        Drizzle3D_API AppUpdateEvent() = default;

        Drizzle3D_API EventType GetEventType() override { return EventType::AppUpdate; }
    };

    class AppRenderEvent : public Event {
    public:
        Drizzle3D_API AppRenderEvent() = default;

        Drizzle3D_API EventType GetEventType() override { return EventType::AppRender; }
    };

    class KeyPressedEvent : public Event {
    public:
        Drizzle3D_API KeyPressedEvent(const KeyCode keycode) : m_KeyCode(keycode) {}

        Drizzle3D_API KeyCode GetKeyCode() { return m_KeyCode; }

        Drizzle3D_API EventType GetEventType() override { return EventType::KeyPressed; }
    private:
        KeyCode m_KeyCode;
    };

    class KeyReleasedEvent : public Event {
    public:
        Drizzle3D_API KeyReleasedEvent(const KeyCode keycode) : m_KeyCode(keycode) {}

        Drizzle3D_API KeyCode GetKeyCode() { return m_KeyCode; }

        Drizzle3D_API EventType GetEventType() override { return EventType::KeyReleased; }
    private:
        KeyCode m_KeyCode;
    };

    class MouseMovedEvent : public Event {
    public:
        Drizzle3D_API MouseMovedEvent(const float x, const float y) : m_MouseX(x), m_MouseY(y) {}

        Drizzle3D_API float GetX() const { return m_MouseX; }
        Drizzle3D_API float GetY() const { return m_MouseY; }

        Drizzle3D_API EventType GetEventType() override { return EventType::MouseMoved; }
    private:
        float m_MouseX, m_MouseY;
    };

    class MouseScrolledEvent : public Event {
    public:
        Drizzle3D_API MouseScrolledEvent(const float xOffset, const float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

        Drizzle3D_API float GetXOffset() const { return m_XOffset; }
        Drizzle3D_API float GetYOffset() const { return m_YOffset; }

        Drizzle3D_API EventType GetEventType() override { return EventType::MouseScrolled; }
    private:
        float m_XOffset, m_YOffset;
    };

    class MouseButtonPressedEvent : public Event {
    public:
        Drizzle3D_API MouseButtonPressedEvent(const MouseCode button) : m_Button(button) {}

        Drizzle3D_API MouseCode GetMouseButton() const { return m_Button; }

        Drizzle3D_API EventType GetEventType() override { return EventType::MouseButtonPressed; }
    private:
        MouseCode m_Button;
    };

    class MouseButtonReleasedEvent : public Event {
    public:
        Drizzle3D_API MouseButtonReleasedEvent(const MouseCode button) : m_Button(button) {}

        Drizzle3D_API MouseCode GetMouseButton() const { return m_Button; }

        Drizzle3D_API EventType GetEventType() override { return EventType::MouseButtonReleased; }
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

    class ResourceManager {
    public:
        Drizzle3D_API Resource loadFile(const std::string& filePath, const char mode[2]);

        Drizzle3D_API bool fileExists(const std::string& filePath) const;

        Drizzle3D_API void writeFile(const std::string& filePath, const std::string& content);

        Drizzle3D_API std::string& getTempFileContent(const std::string& filePath);
    private:
        std::unordered_map<std::string, std::string> resources;
    };

    /*
    * Window
    */

    class Window {
    public:
        Drizzle3D_API Window(EventDispatcher* dispatch, char* WindowName = (char*)"New Drizzle3D Game", int width = 800, int height = 600);
        Drizzle3D_API ~Window();

        Drizzle3D_API GLFWwindow* returnwindow() { return window; };
        Drizzle3D_API int returnWidth() { return winwidth; }
        Drizzle3D_API int returnHeight() { return winheight; }
        Drizzle3D_API int returnX() { return winx; }
        Drizzle3D_API int returnY() { return winy; }
        Drizzle3D_API std::vector<int> returnKeyPressedCodes() { return key_codes; }
        Drizzle3D_API void clearKeyCodes() { key_codes.clear(); }
        Drizzle3D_API std::vector<int> returnKeyReleasedCodes() { return keyRel_codes; }
        Drizzle3D_API void clearKeyReleasedCodes() { keyRel_codes.clear(); }
        Drizzle3D_API double returnMouseX() { return lastMouseX; }
        Drizzle3D_API double returnMouseY() { return lastMouseY; }

        Drizzle3D_API void ProcessEvents();
        Drizzle3D_API void Render();

        EventDispatcher* dispatcher;
    private:
        GLFWwindow* window = NULL;
        int winwidth;
        int winheight;
        int winx;
        int winy;
        std::vector<int> key_codes;
        std::vector<int> keyRel_codes;
        bool wasLeftMouseButtonPressed = false;
        bool wasRightMouseButtonPressed = false;
        double lastMouseX = 0.0;
        double lastMouseY = 0.0;
        double lastSMouseX = 0.0;
        double lastSMouseY = 0.0;
    };

    /*
    * Layer
    */

    class Layer {
    public:
        Drizzle3D_API Layer() {}

        Drizzle3D_API Layer(Window* window) : name("Layer"), pWindow(window) {}

        Drizzle3D_API virtual ~Layer() = default;
        Drizzle3D_API virtual void OnAttach() { }
        Drizzle3D_API virtual void OnDetach() { }
        Drizzle3D_API virtual void Render() { }

        Drizzle3D_API virtual bool IsShown() const { return show; }
        Drizzle3D_API virtual const std::string& GetName() const { return name; }
        Drizzle3D_API virtual void SetShow(bool value) { show = value; }
    private:
        bool show = false;
        std::string name;
        Window* pWindow = NULL;
    };

    class LayerDispatch {
    public:
        Drizzle3D_API void AddLayer(std::shared_ptr<Layer> layer);
        Drizzle3D_API void RemoveLayerByName(const std::string& name);
        Drizzle3D_API void ShowHideLayerByName(const std::string& name, bool show);
        Drizzle3D_API void PushFront(const std::string& name);
        Drizzle3D_API void PushForward(const std::string& name);
        Drizzle3D_API void PushBack(const std::string& name);
        Drizzle3D_API void PushBackward(const std::string& name);

        Drizzle3D_API void DispatchLayerRender();
        Drizzle3D_API void DispatchLayerDetach();
        Drizzle3D_API void DispatchLayerAttach();

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

    class RenderingLayer : public Layer {
    public:
        Drizzle3D_API RenderingLayer(Window* window, std::shared_ptr<ResourceManager> resmgr);

        Drizzle3D_API void OnAttach() override;
        Drizzle3D_API void OnDetach() override {}
        Drizzle3D_API void Render() override;

        Drizzle3D_API bool IsShown() const override { return show; }
        Drizzle3D_API const std::string& GetName() const override { return name; }
        Drizzle3D_API void SetShow(bool value) override { show = value; }

        Drizzle3D_API void Create_Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
        Drizzle3D_API Object DrawVerts(std::pair<std::vector<float>, std::vector<unsigned int>> vf, glm::mat4 modelMatrix = glm::mat4(1.0f));
        Drizzle3D_API void AddObject(const char* name, Object theObject);
        Drizzle3D_API Object* returnObject(const char* name);
        Drizzle3D_API void RemoveObject(const char* name);
        Drizzle3D_API void AddLight(float id, Light theLight);
        Drizzle3D_API Light* returnLight(float id);
        Drizzle3D_API void RemoveLight(float id);
        Drizzle3D_API void SwitchCamera(const char* name);
        Drizzle3D_API void AddCamera(const char* id, Camera theCamera);
        Drizzle3D_API Camera* returnCamera(const char* id);
        Drizzle3D_API void RemoveCamera(const char* id);
        Drizzle3D_API char* GetActiveCamera() { return current_camera; }
        Drizzle3D_API Camera ReturnActiveCamera();
        Drizzle3D_API Camera GetCameraFromID(char* cam);
        Drizzle3D_API Flags* GetFlags() { return &flags; }

        bool Lighting = true;
    private:
        bool show;
        GLuint shaderProgram = 0;
        GLuint OldshaderProgram = 0;
        std::string name;
        Window* pWindow;
        std::vector<Object> Objects;
        std::vector<Light> Lights;
        std::vector<Camera> Cameras;
        Flags flags;

        GLuint lightsBuffer = 0;
        char* current_camera = (char*)"Default";
        std::shared_ptr<ResourceManager> resourcemgr;
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

    class ImGuiLayer : public Layer {
    public:
        Drizzle3D_API ImGuiLayer(Window* window) : name("ImGUI"), show(true), pWindow(window) {}

        typedef void (*ImGUICode)(std::shared_ptr<ImGuiLayer> igui);

        ImGUICode code = [](std::shared_ptr<ImGuiLayer> igui) {};

        Drizzle3D_API void OnAttach() override;
        Drizzle3D_API void OnDetach() { }
        Drizzle3D_API void Render() override;

        Drizzle3D_API bool IsShown() const override { return show; }
        Drizzle3D_API const std::string& GetName() const override { return name; }
        Drizzle3D_API void SetShow(bool value) override { show = value; }
        Drizzle3D_API void setIGUI(std::shared_ptr<ImGuiLayer> ig) { igui = ig; }
        Drizzle3D_API void IterateSliderFloat();
        Drizzle3D_API void GUISliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = NULL, int flags = NULL);
        ImGuiContext* imguiContext = NULL;

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

    class App {
    public:
        Drizzle3D_API App(char* WindowName = (char*)"New Drizzle3D Game", int width = 800, int height = 600);

        Drizzle3D_API bool Run();

        Drizzle3D_API Window* window() { return &D3DWindow; }
        Drizzle3D_API std::shared_ptr<ImGuiLayer> ImguiLayer() { return imguilayer; }
        Drizzle3D_API std::shared_ptr<RenderingLayer> GetRenderingLayer() { return renderinglayer; }
        Drizzle3D_API std::shared_ptr<ResourceManager> GetResourceManager() { return resourcemgr; }
        Drizzle3D_API EventDispatcher* dispatcher() { return &dispatch; }

    private:
        // Managers
        std::shared_ptr<ResourceManager> resourcemgr;

        Window D3DWindow;

        // Layers
        std::shared_ptr<ImGuiLayer> imguilayer;
        std::shared_ptr<RenderingLayer> renderinglayer;

        // Dispatchers
        EventDispatcher dispatch;
        LayerDispatch LayerDispatcher;
    };

    /*
    * First Person Camera
    */

    class FirstPersonCamera {
    public:
        Drizzle3D_API FirstPersonCamera(std::shared_ptr<App> app);

        glm::vec3 position = glm::vec3(0, 0, 5);
        float horizontalAngle = 0.0f;
        float verticalAngle = 0.0f;
        float initialFoV = 450.0f;

        float speed = 1.0f; // 3 units / second
        float mouseSpeed = 0.005f;
        std::shared_ptr<App> application;
        bool capture = true;
        double xpos, ypos = 0.0;
    };

    /*
    * Skybox
    */

    class Skybox {
    public:
        Drizzle3D_API Skybox(std::shared_ptr<App> app, const char* skyboxtex, float size = 100.0f);
        Drizzle3D_API void Update();

    private:
        std::shared_ptr<App> application;
        glm::vec3 pos;
    };

    /*
    * Material
    */

    class Material {
    public:
        Drizzle3D_API Material(std::shared_ptr<ResourceManager> resourcemgr, const char* fname, const char* fgname);
        Drizzle3D_API GLuint GetShaderProgram();
    };
}