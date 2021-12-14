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

const float SCALE = 0.00000025;

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
    return radius / 15000;
}

static float ComputeAcademicSphereRadius(YAML::Node values) {
    float earth_radius = 6378;
    float radius = values["academic"].as<float>() * earth_radius;
    return radius / 15000;
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
    } else {
        std::rotate(m_OrbitVertices.begin(), m_OrbitVertices.begin() + 3, m_OrbitVertices.end());
        m_OrbitVertices[1998] = coordinates[0];
        m_OrbitVertices[1999] = coordinates[1];
        m_OrbitVertices[2000] = coordinates[2];
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
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)m_OrbitVertices.size() / 3);
}

void Planet::GenerateFullOrbit() {
    using namespace date;

    int step = std::round(m_OrbitalPeriod / 365.0);
    if (step == 0) step = 1;

    m_OrbitVertices = std::vector<float>(373 * 3);

    year_month_day date = sys_days{ 1970_y / January / 01 };

    for (int i = 0; i < 373 * 3; i += 3) {
        date = sys_days{ date } + days{ step };

        std::stringstream date_str;
        date_str << date;

        double ephemerisTime = spice::GetEphemerisTime(date_str.str());
        glm::vec3 coordinates = spice::GetCoordinate(ephemerisTime, m_Name + " barycenter");
        coordinates *= SCALE;

        m_OrbitVertices[i] = coordinates[0];
        m_OrbitVertices[i + 1] = coordinates[1];
        m_OrbitVertices[i + 2] = coordinates[2];
    }
}

void RenderPlanets(std::vector<Planet*> planets, State& state, Camera& camera, Shader& shader) {
    double ephemerisTime = spice::GetEphemerisTime(state.GetDate());
    for (Planet* planet : planets) {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        if (planet->GetName() != "sun") {
            position = spice::GetCoordinate(ephemerisTime, planet->GetName() + " barycenter");
            position *= SCALE; // scale down the planet's position
        }

        if (planet->GetName() == state.GetSelectedPlanet() && state.IsFocusedOnPlanet()) {
            glm::vec3 pos = glm::vec3(position[0] * 1.5, position[1] + planet->GetRadius() + 5, position[2] * 1.5);
            camera.SetCameraPos(pos);
            camera.SetCameraFront(-pos); // look at the origin
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        shader.SetMat4("u_Model", model);

        planet->Draw();

        shader.SetMat4("u_Model", glm::mat4(1.0f));
        planet->DrawOrbit();

        // Render Rings
        if (planet->GetName() == "saturn") {
            glLineWidth(2.0f);
            GLfloat rr = 0.01f;
            for (int i = 0; i < 25; i++) {
                glm::mat4 ringModel(1);
                ringModel = glm::translate(ringModel, position);
                ringModel = glm::rotate(ringModel, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                ringModel = glm::scale(ringModel, glm::vec3(rr, rr, rr));
                shader.SetMat4("u_Model", ringModel);
                glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)planet->GetOrbitVertices().size() / 3);
                if (i == 15)
                    rr += 0.0003f;
                else
                    rr += 0.0002f;
            }
        }

        //-------------------------------------------------------------------------------------------------------------------------
        if (planet->GetName() == "earth") {
            for (Satelite* satellite : planet->GetSatelites()) {
                double satelite_lt;

                position = spice::GetCoordinate(ephemerisTime, satellite->GetName());
                position *= SCALE;
                position *= 1.05; // TODO: find out the value
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, position);
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                shader.SetMat4("u_Model", model);

                satellite->Draw();
            }
        }
        //-------------------------------------------------------------------------------------------------------------------------
    }
}
