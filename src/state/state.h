#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

enum SpeedMode { Minutes,
                  Hours,
                  Days,
                  COUNT };

class State {
  private:
    bool m_DisableCursor = false;
    bool m_DisableCursorCallback = false;
    bool m_RealisticModePlanets = false;
    bool m_RealisticModeOrbits = false;
    float m_OrbitRadius = 1.0f;
    glm::vec3 m_CurrentPosition;
    std::string m_SelectedPlanet = "earth";
    int m_SelectedPlanetIndex = 3;
    std::string m_Date;
    SpeedMode m_speedMode = SpeedMode::Hours;

  public:
    State();
    void Modify(int key, int action);
    bool CursorDisabled() { return m_DisableCursor; }
    bool CursorCallbackDisabled() { return m_DisableCursorCallback; }
    void SetCurrentPosition(glm::vec3 currentPosition) { m_CurrentPosition = currentPosition; }
    bool RealisticModePlanetsEnabled() { return m_RealisticModePlanets; }
    bool RealisticModeOrbitsEnabled() { return m_RealisticModeOrbits; }
    void ToggleRealisticModePlanets() { m_RealisticModePlanets = !m_RealisticModePlanets; }
    void ToggleRealisticModeOrbits() { m_RealisticModeOrbits = !m_RealisticModeOrbits; }
    float GetOrbitRadius() { return m_OrbitRadius; }
    void SetOrbitRadius(float radius) { m_OrbitRadius = radius; }
    glm::vec3 GetCurrentPosition() { return m_CurrentPosition; }
    std::string GetSelectedPlanet() { return m_SelectedPlanet; }
    void SetSelectedPlanet(int index, std::string selectedPlanet) {
        m_SelectedPlanetIndex = index;
        m_SelectedPlanet = selectedPlanet;
    }
    int GetSelectedPlanetIndex() { return m_SelectedPlanetIndex; }
    void SetDate(std::string date) { m_Date = date; }
    std::string GetDate() { return m_Date; }
    void SetSpeedMode(SpeedMode mode) { m_speedMode = mode; }
    SpeedMode GetSpeedMode() { return m_speedMode; }
};
