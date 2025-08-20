#include "pch.hpp"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//     glViewport(0, 0, width, height);
// }

int main() {
    printSystemInfo();

    GLFWwindow* window;
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // cout << "default shader lang: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    // select opengl version
    // int major, minor, rev;
    // glfwGetVersion(&major, &minor, &rev);
    // cout << "glfw major.minor " << major << "." << minor << "." << rev << endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD");
        exit(EXIT_FAILURE);
    }

    // LOG_INFO(glGetString(GL_SHADING_LANGUAGE_VERSION));

    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window)) {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        // ratio = width / (float) height;
        // glViewport(0, 0, width, height);
        // glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}