#pragma once

#include <string>
#include <glm/glm.hpp>

#include "../mesh/sphere.h"

class Planet : public Sphere {
    private:
        glm::vec3 m_Coordinates;
        unsigned int m_Texture;

    public:
        Planet(float r, int sectors, int stacks, std::string texturePath);
        glm::vec3 GetCoordinates() { return m_Coordinates; }
        unsigned int GetTexture() { return m_Texture; }
        void Draw();
};
