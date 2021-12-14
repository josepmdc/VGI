#pragma once

#include <string>
#include <glm/glm.hpp>

namespace spice {
void Init();
glm::vec3 GetCoordinate(double ephemerisTime, std::string planet);
double GetEphemerisTime(std::string date);
} // namespace spice
