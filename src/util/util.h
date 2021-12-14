#pragma once

#include <string>
#include <vector>
#include <ctime>
#include "../planet/planet.h"

namespace util {
unsigned int LoadTexture(std::string path);
std::vector<Planet*> LoadPlanets(bool isAcademic);
void addMonth(struct std::tm& tm);
void addDay(struct std::tm& tm);
void addHour(struct std::tm& tm);
void addMin(struct std::tm& tm);
}
