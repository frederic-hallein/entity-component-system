#ifndef WINDOW_SYSTEM_HPP
#define WINDOW_SYSTEM_HPP

#include "system.hpp"

namespace ecs {

    template<u64 MaxEntities, u16 Width, u16 Height>
    class WindowSystem : public System<MaxEntities> {
    public:
        WindowSystem(cstr title = "ECS Game")
            : mTitle(title), mWindow(nullptr) {}

        void init() override {
            if (!glfwInit()) {
                LOG_ERROR("Failed to initialize GLFW");
                return;
            }

            // Set OpenGL context hints
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            #ifdef __APPLE__
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            #endif

            // Create window
            // mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, nullptr, nullptr);
            mWindow = glfwCreateWindow(Width, Height, mTitle, nullptr, nullptr);
            if (!mWindow) {
                LOG_ERROR("Failed to create GLFW window");
                glfwTerminate();
                return;
            }

            glfwMakeContextCurrent(mWindow);
            glfwSetKeyCallback(mWindow, keyCallback);
            glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);

            // Initialize GLAD
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                LOG_ERROR("Failed to initialize GLAD");
                return;
            }

            LOG_INFO("Window system initialized successfully");
            LOG_INFO("OpenGL Version: ", glGetString(GL_VERSION));
        }

        void update(float deltaTime) override {

            // Clear screen
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Render entities with Position components here
            // TODO : implemtn a RenderSystem
            renderEntities();

            // Swap buffers and poll events
            glfwSwapBuffers(mWindow);
            glfwPollEvents();
        }

        void shutdown() override {
            if (mWindow) {
                glfwDestroyWindow(mWindow);
                mWindow = nullptr;
            }
            glfwTerminate();
            LOG_INFO("Window system shut down");
        }

        bool shouldClose() const {
            return mWindow ? glfwWindowShouldClose(mWindow) : true;
        }

        GLFWwindow* getWindow() const { return mWindow; }

    private:
        // constexpr static u16 mWidth = Width;
        // constexpr static u16 mHeight = Height;
        cstr mTitle;
        GLFWwindow* mWindow;

        void renderEntities() {
            // Simple rendering - iterate through entities with Position
            for (u64 entityId = 0; entityId < 1000; ++entityId) {
                if (this->template entityHasComponents<Position>(entityId)) {
                    auto* position = this->mComponentManager->template getComponent<Position>(entityId);
                    if (position) {
                        LOG_DEBUG("Rendering entity ", entityId, " at (", position->x, ", ", position->y, ")");
                    }
                }
            }
        }

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

}

#endif