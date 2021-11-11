#pragma once

#include <vector>

#include "../planet/planet.h"
#include "../state/state.h"

namespace GUI {
void SetUp(GLFWwindow* window);
void NewFrame();
void DrawControls(std::vector<Planet*> planets, std::vector<Planet*> academicPlanets, State& state);
void Destroy();
}; // namespace GUI
