#include "state.h"

State::State() {}

void State::Modify(int key, int action) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_C:
            m_DisableCursor = !m_DisableCursor;
            break;
        case GLFW_KEY_R:
            m_DisableCursorCallback = !m_DisableCursorCallback;
            break;
        }
    }
}
