#ifndef IMGUI_LAYER_HPP
#define IMGUI_LAYER_HPP

#include "pch.hpp"

class ImGuiLayer {
public:
    using DrawCallback = std::function<void()>;
    ImGuiLayer(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    ~ImGuiLayer() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void addWindow(DrawCallback cb) {
        mDrawCallbacks.push_back(cb);
    }

    void renderAll() {
        for (auto& cb : mDrawCallbacks) {
            cb();
        }
    }

    void begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void end() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

private:
    std::vector<DrawCallback> mDrawCallbacks;
};

#endif