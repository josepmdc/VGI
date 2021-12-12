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

void State::SetDate(int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, std::string date_str) {
    m_Year = year;
    m_Month = month;
    m_Day = day;
    m_Hour = hour;
    m_Minute = minute;
    m_Date = date_str;
}
