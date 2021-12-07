#include "state.h"

State::State() {}

void State::Modify(int key, int action) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_C:
            m_DisableCursor = !m_DisableCursor;
            break;
        case GLFW_KEY_Q:
            if (!m_FocusedOnPlanet) {
                m_DisableCursorCallback = !m_DisableCursorCallback;
            }
            break;
        }
    }
}

void State::SetDate(int year, int month, int day, int hour) {
    m_Year = year;
    m_Month = month;
    m_Day = day;
    m_Hour = hour;

    m_Date = std::to_string(year) + "-" +
             std::to_string(month) + "-" +
             std::to_string(day) + "T" +
             std::to_string(hour);
}
