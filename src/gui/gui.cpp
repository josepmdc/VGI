#include "gui.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace GUI {

void SetUp(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void NewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void DrawControls(std::vector<Planet*> planets, State& state) {
    ImGui::Begin("Controls");

    if (ImGui::Button("Change mode")) {
        state.ToggleRealisticMode();
        for (Planet* planet : planets) {
            planet->GenerateOrbit(
                state.RealisticModeEnabled() ? planet->GetOrbitRadius() : state.GetOrbitRadius() //
            );
        }
    }

    ImGui::Text(state.RealisticModeEnabled() ? "Current Mode: Realistic" : "Current Mode: Academic");

    int selectedPlanetIndex = state.GetSelectedPlanetIndex();

    if (ImGui::BeginCombo("Change camara view to: ", planets[selectedPlanetIndex]->GetName().c_str(), 0)) {
        for (int i = 0; i < planets.size(); i++) {
            const bool isSelected = (selectedPlanetIndex == i);
            if (ImGui::Selectable(planets[i]->GetName().c_str(), isSelected)) {
                state.SetSelectedPlanet(i, planets[i]->GetName());
                std::cout << planets[i]->GetName() << std::endl;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    float radius = state.GetOrbitRadius();
    if (ImGui::SliderFloat("Radius", &radius, 0.0f, 10.0f)) {
        state.SetOrbitRadius(radius);
        for (Planet* planet : planets) {
            planet->GenerateOrbit(radius);
        }
    }

    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Controls:");
    ImGui::BeginChild("Scrolling");
    ImGui::Text("Move: WASD");
    ImGui::Text("Up: SPACE");
    ImGui::Text("Down: Left Shift");
    ImGui::Text("Lock/Unlock Cursor: C");
    ImGui::Text("Disable Cursor Callback: Q");
    ImGui::Text("Go to earth: R");
    ImGui::EndChild();
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

} // namespace GUI
