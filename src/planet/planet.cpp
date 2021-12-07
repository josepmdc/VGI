#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>
#include <ctime>
#include <date.h>

#include "planet.h"
#include "../util/util.h"
#include "../spice/spice.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Planet::Planet(glm::vec3 coordinates, std::string texturePath) : Sphere(0.5f, 36, 18) {
    m_Coordinates = coordinates;
    m_Texture = util::LoadTexture(texturePath);
}

Planet::Planet(float r, int sectors, int stacks, glm::vec3 coordinates, std::string texturePath) : Sphere(r, sectors, stacks) {
    m_Coordinates = coordinates;
    m_Texture = util::LoadTexture(texturePath);
}

static float ComputeSphereRadius(YAML::Node values) {
    float radius = values["diameter"].as<float>() / 2;
    return radius / 10000000;
}

static float ComputeAcademicSphereRadius(YAML::Node values) {
    float earth_radius = 6378;
    float radius = values["academic"].as<float>() * earth_radius;
    return radius / 100000;
}

Planet::Planet(YAML::Node values, std::string name, bool isAcademic) : Sphere(isAcademic ? ComputeAcademicSphereRadius(values) : ComputeSphereRadius(values), 36, 18) {
    m_Texture = util::LoadTexture(values["texture"].as<std::string>());
    m_Name = name;

    std::vector<float> coordinates = values["coordinates"].as<std::vector<float>>();
    m_Coordinates = glm::vec3(coordinates[0], coordinates[1], coordinates[2]);
    m_K = values["k"].as<int>();
    float UA = 149597870.7;
    m_OrbitRadius = (UA * (0.4 + 0.3 * m_K)) / 10000000;
    m_OrbitalPeriod = values["period"].as<int>();

    if (m_Name != "sun") {
        GenerateFullOrbit();
        InitOrbit();
    }

    if (!values["satelites"].IsNull()) {
        for (auto satelite = values["satelites"].begin(); satelite != values["satelites"].end(); satelite++) {
            m_satelites.push_back(new Satelite(satelite->second, satelite->first.as<std::string>(), true));
        }
    }
}

void Planet::InitOrbit() {
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

void Planet::AddNextOrbitVertex(glm::vec3 coordinates) {
    if (m_OrbitVertices.size() < 2001) {
        m_OrbitVertices.push_back(coordinates[0]);
        m_OrbitVertices.push_back(coordinates[1]);
        m_OrbitVertices.push_back(coordinates[2]);
        // m_OrbitIndex += 3;
    } else {
        std::rotate(m_OrbitVertices.begin(), m_OrbitVertices.begin() + 3, m_OrbitVertices.end());
        m_OrbitVertices[1998] = coordinates[0];
        m_OrbitVertices[1999] = coordinates[1];
        m_OrbitVertices[2000] = coordinates[2];
        // m_OrbitVertices[++m_OrbitIndex % 2001] = coordinates[0];
        // m_OrbitVertices[++m_OrbitIndex % 2001] = coordinates[1];
        // m_OrbitVertices[++m_OrbitIndex % 2001] = coordinates[2];
        // m_OrbitIndex = m_OrbitIndex % 2001;
    }

    glBindVertexArray(m_OrbitsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_OrbitsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_OrbitVertices.size(), m_OrbitVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Planet::ClearOrbitBuffer() {
    m_OrbitVertices.clear();
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

void Planet::GenerateFullOrbit() {
    using namespace date;

    int step = std::round(m_OrbitalPeriod / 365.0);
    if (step == 0)
        step = 1;

    m_OrbitVertices = std::vector<float>(365 * 3);

    year_month_day date = sys_days{ 1970_y / January / 01 };

    for (int i = 0; i < 365 * 3; i += 3) {
        date = sys_days{ date } + days{ step };

        std::stringstream date_str;
        date_str << date;

        double ephemerisTime = spice::GetEphemerisTime(date_str.str());
        glm::vec3 coordinates = spice::GetCoordinate(ephemerisTime, m_Name + " barycenter");
        coordinates *= 0.00000003;

        m_OrbitVertices[i] = coordinates[1];
        m_OrbitVertices[i + 1] = coordinates[2];
        m_OrbitVertices[i + 2] = coordinates[0];
    }
}

void RenderPlanets(std::vector<Planet*> planets, State& state, Camera& camera, Shader& shader) {
    double ephemerisTime = spice::GetEphemerisTime(state.GetDate());
    for (Planet* planet : planets) {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        if (planet->GetName() != "sun") {
            position = spice::GetCoordinate(ephemerisTime, planet->GetName() + " barycenter");
            position *= 0.00000003; // scale down the planet's position
        }

        if (planet->GetName() == state.GetSelectedPlanet() && state.IsFocusedOnPlanet()) {
            glm::vec3 pos = glm::vec3(position[1] * 1.5, position[2] + planet->GetRadius() + 5, position[0] * 1.5);
            camera.SetCameraPos(pos);
            camera.SetCameraFront(-pos); // look at the origin
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position[1], position[2], position[0]));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        shader.SetMat4("u_Model", model);

        planet->Draw();

        shader.SetMat4("u_Model", glm::mat4(1.0f));
        planet->DrawOrbit();

        //-------------------------------------------------------------------------------------------------------------------------
        if (planet->GetName() == "earth") {
            Satelite* moon = planet->GetSatelites()[0];

            position = spice::GetCoordinate(ephemerisTime, moon->GetName());
            position *= 0.000000025;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(position[1], position[2], position[0]));
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            shader.SetMat4("u_Model", model);

            moon->Draw();
        }
        //-------------------------------------------------------------------------------------------------------------------------
    }
}
