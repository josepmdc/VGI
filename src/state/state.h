#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

enum SpeedMode { Slow,
                 Normal,
                 Fast,
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
    unsigned int m_Month = 1;
    unsigned int m_Day = 1;
    unsigned int m_Hour = 1;
    unsigned int m_Minute = 1;
    std::string m_Date;

    SpeedMode m_speedMode = SpeedMode::Normal;

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

    void SetDate(int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, std::string date_str);
    void SetYear(int year) { m_Year = year; }
    void SetMonth(unsigned int month) { m_Month = month; }
    void SetDay(unsigned int day) { m_Day = day; }
    void SetHour(unsigned int hour) { m_Hour = hour; }
    void SetMinute(unsigned int minute) { m_Minute = minute; }

    std::string GetDate() { return m_Date; }
    int GetYear() { return m_Year; }
    unsigned int GetMonth() { return m_Month; }
    unsigned int GetDay() { return m_Day; }
    unsigned int GetHour() { return m_Hour; }
    unsigned int GetMinute() { return m_Minute; }

    void SetSpeedMode(SpeedMode mode) { m_speedMode = mode; }
    SpeedMode GetSpeedMode() { return m_speedMode; }

    bool IsFocusedOnPlanet() { return m_FocusedOnPlanet; }
    void FocusOnPlanet() { m_FocusedOnPlanet = true; }
    void UnfocusFromPlanet() { m_FocusedOnPlanet = false; }
};
