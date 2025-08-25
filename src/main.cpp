#include "pch.hpp"

#include "entityManager.hpp"
#include "componentManager.hpp"
#include "systemManager.hpp"
#include "systems.hpp"

// static void processInput(GLFWwindow *window) {
//     if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         LOG_DEBUG("Escape key pressed, closing window");
//         glfwSetWindowShouldClose(window, true);
// }

// static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//     glViewport(0, 0, width, height);
// }

constexpr u64 MAX_ENTITIES = 1000000;
constexpr u32 MAX_COMPONENTS = 100;

using namespace ecs;

int main() {
    printInfo();

    EntityManager<MAX_ENTITIES> entityManager;
    ComponentManager<MAX_ENTITIES> componentManager;
    SystemManager<MAX_ENTITIES> systemManager(&componentManager);

    // Register systems
    systemManager.registerSystem<MovementSystem<MAX_ENTITIES>>();
    systemManager.registerSystem<HealthSystem<MAX_ENTITIES>>();
    systemManager.registerSystem<RenderSystem<MAX_ENTITIES>>();

    Position pos{100, 200};
    Position pos2{130, 560};
    Velocity vel{0, -10};

    u64 playerId = 1;
    componentManager.setComponent(playerId, pos);
    componentManager.setComponent(playerId, pos2); //warn
    componentManager.setComponent(playerId, vel);

    // Game loop simulation
    float deltaTime = 0.016f; // ~60 FPS
    for (int frame = 0; frame < 100; ++frame) {
        LOG_INFO("=== Frame ", frame, " ===");
        systemManager.updateSystems(deltaTime);

        // Add some delay to see the output
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    systemManager.shutdown();



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