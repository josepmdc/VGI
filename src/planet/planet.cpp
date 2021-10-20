#include "planet.h"
#include "../util/util.h"

#include <iostream>

Planet::Planet(float r, int sectors, int stacks, std::string texturePath) : Sphere(r, sectors, stacks) {
    m_Texture = util::LoadTexture(texturePath);
}

void Planet::Draw() {
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, m_Texture);
    Sphere::Draw();
}
