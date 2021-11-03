#pragma once

#include <vector>

#include "../planet/planet.h"
#include "../state/state.h"

namespace GUI {
void SetUp(GLFWwindow* window);
void NewFrame();
void DrawControls(std::vector<Planet*> planets, State& state);
void Destroy();
}; // namespace GUI
