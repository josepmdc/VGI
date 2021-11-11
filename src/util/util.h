#pragma once

#include <string>
#include <vector>
#include "../planet/planet.h"

namespace util {
unsigned int LoadTexture(std::string path);
std::vector<Planet*> LoadPlanets(bool isAcademic);
}
