#include "gui.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

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
        for (Planet* planet : selectedPlanets) {
            planet->ClearOrbitBuffer();
        }
    }

    int selectedPlanetIndex = state.GetSelectedPlanetIndex();

    if (ImGui::BeginCombo("Camara position", planets[selectedPlanetIndex]->GetName().c_str(), 0)) {
        for (int i = 0; i < planets.size(); i++) {
            const bool isSelected = (selectedPlanetIndex == i);
            if (ImGui::Selectable(planets[i]->GetName().c_str(), isSelected)) {
                state.SetSelectedPlanet(i, planets[i]->GetName());
                state.FocusOnPlanet();
                state.DisableCursorCallback();
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    int speed = state.GetSpeedMode();
    const char* elems_names[COUNT] = { "1x", "1.5x", "2x" };
    const char* elem_name = (speed >= 0 && speed < COUNT) ? elems_names[speed] : "Unknown";

    if (ImGui::SliderInt("Speed", &speed, 0, COUNT - 1, elem_name)) {
        state.SetSpeedMode((SpeedMode)speed);
    }

    ImGui::TextColored(ImVec4(0.961, 0.808, 0.259, 1), "Current date (ISO Format): %s", state.GetDate().c_str());

    if (ImGui::BeginCombo("Year", std::to_string(state.GetYear()).c_str())) {
        for (int year = 1550; year < 2649; year++) {
            const bool isSelected = (state.GetYear() == year);
            if (ImGui::Selectable(std::to_string(year).c_str(), isSelected)) {
                state.SetYear(year);
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Month", std::to_string(state.GetMonth()).c_str())) {
        for (int month = 1; month <= 12; month++) {
            const bool isSelected = (state.GetMonth() == month);
            if (ImGui::Selectable(std::to_string(month).c_str(), isSelected)) {
                state.SetMonth(month);
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Day", std::to_string(state.GetDay()).c_str())) {
        int maxDay = state.GetMonth() == 2 ? 28 : 30;
        for (int day = 1; day <= maxDay; day++) {
            const bool isSelected = (state.GetDay() == day);
            if (ImGui::Selectable(std::to_string(day).c_str(), isSelected)) {
                state.SetDay(day);
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
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
