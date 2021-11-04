#include "planet.h"
#include "../util/util.h"

#include <iostream>
#include <cmath>

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
    m_Coordinates = glm::vec3(coordinates[0], coordinates[1], coordinates[2]);
    m_K = values["k"].as<int>();
    float UA = 149597870.7;
    m_OrbitRadius = (UA * (0.4 + 0.3 * m_K)) / 10000000;

    GenerateOrbit(m_OrbitRadius);
}

void Planet::GenerateOrbit(float orbitRadius) {
    m_OrbitVertices.clear();
    GLfloat xx, zz;
    float angl;
    for (int i = 0; i < 2000; i++) {
        angl = (float)(M_PI / 2 - i * (M_PI / 1000));
        xx = sin(angl) * orbitRadius;
        zz = cos(angl) * orbitRadius;
        glm::vec3 coordinates = glm::vec3(xx, 0.0f, zz) * m_Coordinates;
        m_OrbitVertices.push_back(coordinates[0]);
        m_OrbitVertices.push_back(coordinates[1]);
        m_OrbitVertices.push_back(coordinates[2]);
    }

    glGenVertexArrays(1, &m_OrbitsVAO);
    glGenBuffers(1, &m_OrbitsVBO);
    glBindVertexArray(m_OrbitsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_OrbitsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_OrbitVertices.size(), m_OrbitVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Planet::Draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    Sphere::Draw();
}

void Planet::DrawOrbit() {
    glBindVertexArray(m_OrbitsVAO);
    glLineWidth(1.0f);
    glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)m_OrbitVertices.size() / 3);
}
