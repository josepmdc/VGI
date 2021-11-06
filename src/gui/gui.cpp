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

void DrawToggleButton(bool* v) {
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    ImGui::InvisibleButton("", ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *v = !*v;
    if (ImGui::IsItemHovered())
        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_ButtonActive] : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * 0.5f);
    else
        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_Button] : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * 0.50f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + (*v ? 1 : 0) * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}

void DrawControls(std::vector<Planet*> planets, std::vector<Planet*> academicPlanets, State& state) { 
    ImGui::Begin("Controls");

    bool togglePlanets = state.RealisticModePlanetsEnabled();
    bool toggleOrbits = state.RealisticModeOrbitsEnabled();
    
    ImGui::Text("Planet Size Realistic Mode");
    ImGui::SameLine(200);
    DrawToggleButton(&togglePlanets);

    std::vector<Planet*> selectedPlanets = state.RealisticModePlanetsEnabled() ? planets : academicPlanets;

    if (togglePlanets != state.RealisticModePlanetsEnabled()) {
        state.ToggleRealisticModePlanets();
        selectedPlanets = state.RealisticModePlanetsEnabled() ? planets : academicPlanets;
        for (Planet* planet : planets) {
            planet->GenerateOrbit(state.RealisticModeOrbitsEnabled() ? planet->GetOrbitRadius() : state.GetOrbitRadius());
        }
    }

    ImGui::Text("Orbit Realistic Mode");
    ImGui::SameLine(200);
    DrawToggleButton(&toggleOrbits);
    
    if (toggleOrbits != state.RealisticModeOrbitsEnabled()) {
        state.ToggleRealisticModeOrbits();
        for (Planet* planet : selectedPlanets) {
            planet->GenerateOrbit(state.RealisticModeOrbitsEnabled() ? planet->GetOrbitRadius() : state.GetOrbitRadius());
        }
    }

    int selectedPlanetIndex = state.GetSelectedPlanetIndex();

    if (ImGui::BeginCombo("Camara position", planets[selectedPlanetIndex]->GetName().c_str(), 0)) {
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

    if (!state.RealisticModeOrbitsEnabled()) {
        float radius = state.GetOrbitRadius();
        if (ImGui::SliderFloat("Radius", &radius, 0.0f, 10.0f)) {
            state.SetOrbitRadius(radius);
            for (Planet* planet : selectedPlanets) {
                planet->GenerateOrbit(radius);
            }
        }
    }

    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Controls:");
    ImGui::BeginChild("Scrolling");
    ImGui::Text("Move: WASD");
    ImGui::Text("Up: SPACE");
    ImGui::Text("Down: Left Shift");
    ImGui::Text("Show/Hide Cursor: C");
    ImGui::Text("Disable Cursor Callback: Q");
    ImGui::Text("Go to selected planet: R");
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
