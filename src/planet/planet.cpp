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

void Planet::Draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    Sphere::Draw();
}
