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

    std::string m_SelectedPlanet = "earth";
    int m_SelectedPlanetIndex = 3;
    bool m_FocusedOnPlanet = false;

    int m_Year = 1970;
    int m_Month = 1;
    int m_Day = 1;
    int m_Hour = 1;
    std::string m_Date;

    SpeedMode m_speedMode = SpeedMode::Hours;

  public:
    State();
    void Modify(int key, int action);

    bool CursorDisabled() { return m_DisableCursor; }
    bool CursorCallbackDisabled() { return m_DisableCursorCallback; }
    void DisableCursorCallback() { m_DisableCursorCallback = true; }
    void EnableCursorCallback() { m_DisableCursorCallback = false; }

    bool RealisticModePlanetsEnabled() { return m_RealisticModePlanets; }
    bool RealisticModeOrbitsEnabled() { return m_RealisticModeOrbits; }
    void ToggleRealisticModePlanets() { m_RealisticModePlanets = !m_RealisticModePlanets; }
    void ToggleRealisticModeOrbits() { m_RealisticModeOrbits = !m_RealisticModeOrbits; }

    float GetOrbitRadius() { return m_OrbitRadius; }
    void SetOrbitRadius(float radius) { m_OrbitRadius = radius; }

    std::string GetSelectedPlanet() { return m_SelectedPlanet; }
    void SetSelectedPlanet(int index, std::string selectedPlanet) {
        m_SelectedPlanetIndex = index;
        m_SelectedPlanet = selectedPlanet;
    }

    int GetSelectedPlanetIndex() { return m_SelectedPlanetIndex; }

    std::string GetDate() { return m_Date; }
    void SetDate(int year, int month, int day, int hour);
    void SetYear(int year) { m_Year = year; }
    void SetMonth(int month) { m_Month = month; }
    void SetDay(int day) { m_Day = day; }
    void SetHour(int hour) { m_Hour = hour; }
    int GetYear() { return m_Year; }
    int GetMonth() { return m_Month; }
    int GetDay() { return m_Day; }
    int GetHour() { return m_Hour; }

    void SetSpeedMode(SpeedMode mode) { m_speedMode = mode; }
    SpeedMode GetSpeedMode() { return m_speedMode; }

    bool IsFocusedOnPlanet() { return m_FocusedOnPlanet; }
    void FocusOnPlanet() { m_FocusedOnPlanet = true; }
    void UnfocusFromPlanet() { m_FocusedOnPlanet = false; }
};
