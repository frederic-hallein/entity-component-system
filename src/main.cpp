#include "pch.hpp"

#include "entityManager.hpp"
#include "componentManager.hpp"

// static void processInput(GLFWwindow *window) {
//     if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         LOG_DEBUG("Escape key pressed, closing window");
//         glfwSetWindowShouldClose(window, true);
// }

// static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//     glViewport(0, 0, width, height);
// }

constexpr u64 MAX_ENTITIES = 1000000;
using EntityIDType = std::remove_const_t<decltype(MAX_ENTITIES)>;

constexpr u32 MAX_COMPONENTS = 100;

int main() {
    printInfo();

    ecs::EntityManager<EntityIDType, MAX_ENTITIES> entityManager;
    ecs::ComponentManager<EntityIDType, MAX_ENTITIES> componentManager;

    // auto entity = entityManager.createEntity();

    std::vector<ecs::Position> positions = {{1, 2}, {3, 4}};
    // componentManager.setComponents(entity.id, positions);






//     // GLFW
//     GLFWwindow* window;
//     if (!glfwInit())
//         exit(EXIT_FAILURE);

//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//     window = glfwCreateWindow(1080, 720, "ECS", NULL, NULL);
//     if (!window) {
//         LOG_ERROR("Failed to create GLFW window");
//         glfwTerminate();
//         exit(EXIT_FAILURE);
//     }
//     glfwMakeContextCurrent(window);
//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//         LOG_ERROR("Failed to initialize GLAD");
//         exit(EXIT_FAILURE);
//     }

//     // select opengl version
//     int major, minor, rev;
//     glfwGetVersion(&major, &minor, &rev);
//     LOG_INFO("GLFW Version: ", major, '.' , minor, rev);
//     LOG_INFO("GLSL Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION), '\n');

//     while (!glfwWindowShouldClose(window)) {
//         // input
//         processInput(window);

//         // render
//         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT);

//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }
//     glfwDestroyWindow(window);
//     glfwTerminate();
//     exit(EXIT_SUCCESS);
    return 0;
}