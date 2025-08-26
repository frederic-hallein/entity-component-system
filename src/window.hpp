#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "pch.hpp"

class Window {
public:
    Window(cstr title, u16 width, u16 height)
        : mTitle(title), mWidth(width), mHeight(height), mWindow(nullptr) {

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

        LOG_INFO("Window initialized successfully");
        LOG_INFO("OpenGL Version: ", glGetString(GL_VERSION));
    }

    void render() {
        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers and poll events
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
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