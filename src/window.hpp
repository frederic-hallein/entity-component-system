#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "pch.hpp"
#include "imGuiLayer.hpp"

class Window {
public:
    void addImGuiWindows() {
        mImGuiLayer->addWindow([this]() {
            ImGui::Begin("Debug Window");

            ImGui::Text("Statistics:");
            ImGui::Text("FPS: %.2f", mFPS);
            ImGui::Text("dt: %.4f ms", mDeltaTime * 1000.0f);

            static std::deque<f32> fpsHistory;

            fpsHistory.push_back(mFPS);
            if (fpsHistory.size() > 120) {
                fpsHistory.pop_front();
            }

            std::vector<f32> fpsPlot(fpsHistory.begin(), fpsHistory.end());
            ImVec2 plotSize(ImGui::GetContentRegionAvail().x, 100);
            if (!fpsHistory.empty()) {
                ImVec2 plotPos = ImGui::GetCursorScreenPos();
                ImGui::PlotLines(
                    "##fpsplot",
                    fpsPlot.data(),
                    fpsPlot.size(),
                    0,
                    nullptr,
                    0.0f, 120.0f,
                    plotSize
                );

                f32 y60 = plotSize.y * (1.0f - 60.0f / 120.0f);
                ImVec2 p0 = ImGui::GetItemRectMin();
                ImVec2 p1 = ImGui::GetItemRectMax();
                ImGui::GetWindowDrawList()->AddLine(
                    ImVec2(p0.x, p0.y + y60),
                    ImVec2(p1.x, p0.y + y60),
                    IM_COL32(255, 255, 0, 255),
                    0.5f
                );
            }
            ImGui::End();
        });
    }

    Window(cstr title, u16 width, u16 height)
        : mTitle(title), mWidth(width), mHeight(height), mWindow(nullptr) {

        // Init GLFW
        if (!glfwInit()) {
            LOG_ERROR("Failed to initialize GLFW");
            return;
        }

        // Set OpenGL context hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        // Create window
        mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, nullptr, nullptr);
        if (!mWindow) {
            LOG_ERROR("Failed to create GLFW window");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(mWindow);
        glfwSwapInterval(1);
        glfwSetKeyCallback(mWindow, keyCallback);
        glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_ERROR("Failed to initialize GLAD");
            return;
        }

        // Create ImGui layer
        mImGuiLayer = std::make_unique<ImGuiLayer>(mWindow);
        addImGuiWindows();

        LOG_INFO("Window initialized successfully");
        LOG_INFO("OpenGL Version: ", glGetString(GL_VERSION));
    }

    void setStats(float deltaTime, float fps) {
        mDeltaTime = deltaTime;
        mFPS = fps;
    }

    void render() {
        // Render background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render ImGUI layers
        mImGuiLayer->begin();
        mImGuiLayer->renderAll();
        mImGuiLayer->end();

        // Swap buffers and poll events
        glfwSwapBuffers(mWindow);
        glfwPollEvents();

        // Multi-viewport support
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            // Restore OpenGL context for main window
            glfwMakeContextCurrent(mWindow);
        }
    }

    void shutdown() {
        if (mWindow) {
            glfwDestroyWindow(mWindow);
            mWindow = nullptr;
        }
        glfwTerminate();
        LOG_INFO("Window shut down");
    }

    bool shouldClose() const {
        return mWindow ? glfwWindowShouldClose(mWindow) : true;
    }

    GLFWwindow* getWindow() const { return mWindow; }

private:
    cstr mTitle;
    u16 mWidth, mHeight;
    GLFWwindow* mWindow;
    f32 mDeltaTime;
    f32 mFPS;
    std::unique_ptr<ImGuiLayer> mImGuiLayer;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            LOG_INFO("Escape key callback - closing window");
            glfwSetWindowShouldClose(window, true);
        }
    }

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};

#endif