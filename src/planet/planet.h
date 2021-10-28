#pragma once

#include <string>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include "../mesh/sphere.h"

class Planet : public Sphere {
    private:
        glm::vec3 m_Coordinates;
        unsigned int m_Texture;

    public:
        Planet(glm::vec3 coordinates, std::string texturePath);
        Planet(float r, int sectors, int stacks, glm::vec3 coordinates, std::string texturePath);
        Planet::Planet(YAML::Node values);
        glm::vec3 GetCoordinates() { return m_Coordinates; }
        unsigned int GetTexture() { return m_Texture; }
        void Draw();
};
