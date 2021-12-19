#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "satelite.h"
#include "../util/util.h"

#include <iostream>

static float ComputeSphereRadius(YAML::Node values) {
    float radius = values["diameter"].as<float>() / 2;
    return radius / 100000;
}

static float ComputeAcademicSphereRadius(YAML::Node values) {
    float earth_radius = 6378;
    float radius = values["academic"].as<float>() * earth_radius;
    return radius / 15000;
}

Satelite::Satelite(YAML::Node values, std::string name, bool isAcademic) : Sphere(isAcademic ? ComputeAcademicSphereRadius(values) : ComputeSphereRadius(values), 36, 18) {
    m_Texture = util::LoadTexture(values["texture"].as<std::string>());
    m_Name = name;
    float UA = 149597870.7;
    m_OrbitRadius = 1;
}

void Satelite::Draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    Sphere::Draw();
}
