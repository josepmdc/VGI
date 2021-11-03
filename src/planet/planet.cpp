#include "planet.h"
#include "../util/util.h"

#include <iostream>

Planet::Planet(glm::vec3 coordinates, std::string texturePath) : Sphere(0.5f, 36, 18) {
    m_Coordinates = coordinates;
    m_Texture = util::LoadTexture(texturePath);
}

Planet::Planet(float r, int sectors, int stacks, glm::vec3 coordinates, std::string texturePath) : Sphere(r, sectors, stacks) {
    m_Coordinates = coordinates;
    m_Texture = util::LoadTexture(texturePath);
}

static float ComputeSphereRadius(YAML::Node values) {
    float earth_radius = 6378;
    float radius = values["diameter"].as<float>() / 2;
    return radius / 10000000;
}

Planet::Planet(YAML::Node values, std::string name) : Sphere(ComputeSphereRadius(values), 36, 18) {    
    m_Texture = util::LoadTexture(values["texture"].as<std::string>());
    m_Name = name;

    std::vector<float> coordinates = values["coordinates"].as<std::vector<float>>();
    glm::vec3 glm_coordinates = glm::vec3(coordinates[0], coordinates[1], coordinates[2]);
    m_Coordinates = glm_coordinates;
    m_K = values["k"].as<int>();
    float UA = 149597870.7;
    m_OrbitRadius = (UA * (0.4 + 0.3 * m_K)) / 10000000;
}

void Planet::Draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    Sphere::Draw();
}
