#include <glm/gtc/type_ptr.hpp>
#include <SpiceUsr.h>
#include <iostream>

#include "glm/ext/vector_float3.hpp"
#include "spice.h"

namespace spice {

void Init() {
    erract_c("SET", 0, const_cast<char*>("REPORT"));
    errprt_c("SET", 0, const_cast<char*>("NONE"));

    // Load spice kernel for planetary data
    furnsh_c("assets/kernels/de440.bsp");
    furnsh_c("assets/kernels/naif0012.tls");
}

glm::vec3 GetCoordinate(double ephemerisTime, std::string planet) {
    double lt;
    glm::dvec3 position = glm::dvec3(0.0);

    spkpos_c((planet + " barycenter").c_str(), ephemerisTime, "ECLIPJ2000",
             "None", "Sun", glm::value_ptr(position), &lt);

    return glm::vec3(position[0], position[1], position[2]);
}

double GetEphemerisTime(std::string date) {
    double ephemerisTime = 0.0;
    str2et_c(date.c_str(), &ephemerisTime);

    if (failed_c()) {
        std::cout << "Error getting Ephemeris Time: " << date << std::endl;
    }
    return ephemerisTime;
}
} // namespace spice
