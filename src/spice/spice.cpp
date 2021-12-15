#include <glm/gtc/type_ptr.hpp>
#include <SpiceUsr.h>
#include <iostream>

#include "glm/ext/vector_float3.hpp"
#include "spice.h"

namespace spice {

// The value comes from
// http://naif.jpl.nasa.gov/pub/naif/toolkit_docs/C/cspice/getmsg_c.html
// as the maximum message length
constexpr const unsigned SpiceErrorBufferSize = 1841;

void Init() {
    erract_c("SET", 0, const_cast<char*>("REPORT"));
    errprt_c("SET", 0, const_cast<char*>("NONE"));

    // Load spice kernel for planetary data
    furnsh_c("assets/kernels/de440.bsp");
    furnsh_c("assets/kernels/jup365.bsp");
    furnsh_c("assets/kernels/sat427l.bsp");
    furnsh_c("assets/kernels/ura111.bsp");
    furnsh_c("assets/kernels/naif0012.tls");
}

void LogError(std::string message) {
    char buffer[SpiceErrorBufferSize];
    getmsg_c("LONG", SpiceErrorBufferSize, buffer);
    std::cout << "[SPICE] " << message << " =>> " << buffer << std::endl;
    reset_c();
}

glm::vec3 GetCoordinate(double ephemerisTime, std::string planet) {
    return GetCoordinate(ephemerisTime, planet, "Sun");
}

glm::vec3 GetCoordinate(double ephemerisTime, std::string planet, std::string origin) {
    double lt;
    glm::dvec3 position = glm::dvec3(0.0);

    spkpos_c(planet.c_str(), ephemerisTime, "ECLIPJ2000",
             "None", origin.c_str(), glm::value_ptr(position), &lt);

    if (failed_c()) {
        LogError("GetCoordinate");
    }

    return glm::vec3(position[1], position[2], position[0]);
}

double GetEphemerisTime(std::string date) {
    double ephemerisTime = 0.0;
    str2et_c(date.c_str(), &ephemerisTime);
    if (failed_c()) {
        LogError("GetEphemerisTime");
    }
    return ephemerisTime;
}

} // namespace spice
