#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

class State {
  private:
    bool m_DisableCursor = false;
    bool m_DisableCursorCallback = false;
    bool m_RealisticMode = false;
    float m_OrbitRadius = 1.0f;
    glm::vec3 m_CurrentPosition;
    std::string m_SelectedPlanet = "earth";
    int m_SelectedPlanetIndex = 3;

  public:
    State();
    void Modify(int key, int action);
    bool CursorDisabled() { return m_DisableCursor; }
    bool CursorCallbackDisabled() { return m_DisableCursorCallback; }
    void SetCurrentPosition(glm::vec3 currentPosition) { m_CurrentPosition = currentPosition; }
    bool RealisticModeEnabled() { return m_RealisticMode; }
    void ToggleRealisticMode() { m_RealisticMode = !m_RealisticMode; }
    float GetOrbitRadius() { return m_OrbitRadius; }
    void SetOrbitRadius(float radius) { m_OrbitRadius = radius; }
    glm::vec3 GetCurrentPosition() { return m_CurrentPosition; }
    std::string GetSelectedPlanet() { return m_SelectedPlanet; }
    void SetSelectedPlanet(int index, std::string selectedPlanet) {
        m_SelectedPlanetIndex = index;
        m_SelectedPlanet = selectedPlanet;
    }
    int GetSelectedPlanetIndex() { return m_SelectedPlanetIndex; }
};
