#pragma once

#include <string>
#include <glm/glm.hpp>

namespace spice {
void Init();
glm::vec3 GetCoordinate(double ephemerisTime, std::string planet);
glm::vec3 GetCoordinate(double ephemerisTime, std::string planet, std::string origin);
double GetEphemerisTime(std::string date);
} // namespace spice
