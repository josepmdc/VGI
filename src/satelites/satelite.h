#pragma once

#include <string>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include "../mesh/sphere.h"

class Satelite : public Sphere {
  private:
    glm::vec3 m_Coordinates;
    unsigned int m_Texture;
    std::string m_Name;
    float m_OrbitRadius = 0.0f;
    std::vector<float> m_OrbitVertices;
    GLuint m_OrbitsVBO, m_OrbitsVAO;

  public:
    //Satelite(glm::vec3 coordinates, std::string texturePath);
    //Satelite(float r, int sectors, int stacks, glm::vec3 coordinates, std::string texturePath);
    Satelite(YAML::Node values, std::string name, bool isAcademic);
    std::string GetName() { return m_Name; }
    void Satelite::Draw();
};
