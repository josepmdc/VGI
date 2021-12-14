#pragma once

#include <string>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include "../mesh/sphere.h"
#include "../satelites/satelite.h"
#include "../state/state.h"
#include "../camera/camera.h"
#include "../shader/shader.h"

class Planet : public Sphere {
  private:
    glm::vec3 m_Coordinates;
    unsigned int m_Texture;
    int m_K;
    std::string m_Name;
    float m_OrbitRadius = 0.0f;
    std::vector<float> m_OrbitVertices;
    int m_OrbitIndex = -1;
    GLuint m_OrbitsVBO, m_OrbitsVAO;
    std::vector<Satelite*> m_satelites;
    int m_OrbitalPeriod = 0;

  public:
    Planet(glm::vec3 coordinates, std::string texturePath);
    Planet(float r, int sectors, int stacks, glm::vec3 coordinates, std::string texturePath);
    Planet(YAML::Node values, std::string name, bool isAcademic);
    glm::vec3 GetCoordinates() { return m_Coordinates; }
    unsigned int GetTexture() { return m_Texture; }
    int GetK() { return m_K; }
    float GetOrbitRadius() { return m_OrbitRadius; }
    std::string GetName() { return m_Name; }
    std::vector<Satelite*> GetSatelites() { return m_satelites; }
    void Draw();
    void DrawOrbit();
    void InitOrbit();
    void AddNextOrbitVertex(glm::vec3 coordinates);
    void ClearOrbitBuffer();
    void GenerateFullOrbit();
    std::vector<float> GetOrbitVertices() { return m_OrbitVertices; }
};

void RenderPlanets(std::vector<Planet*> planets, State& state, Camera& camera, Shader& shader);
