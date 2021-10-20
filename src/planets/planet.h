#pragma once

#include <string>
#include <glm/glm.hpp>

#include "../mesh/sphere.h"

class Planet : public Sphere {
    private:
        std::string m_TexturePath;
        glm::vec3 m_Coordinates;

    public:
        using Sphere::Sphere; 
        glm::vec3 GetCoordinates() { return m_Coordinates; }
};
