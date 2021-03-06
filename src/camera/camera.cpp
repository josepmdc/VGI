#include "camera.h"
#include <iostream>

Camera ::Camera() {
    m_CameraPos = glm::vec3(0.0f, 10.0f, 20.0f);
    m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    m_CameraDown = glm::vec3(0.0f, -1.0f, 0.0f);
    m_View = glm::mat4(1.0f);
}

void Camera::ProcessInput(GLFWwindow* window, State& state) {
    const float cameraSpeed = 0.25f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        m_CameraPos += cameraSpeed * m_CameraFront;
        state.UnfocusFromPlanet();
        state.EnableCursorCallback();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
        state.UnfocusFromPlanet();
        state.EnableCursorCallback();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_CameraPos -= cameraSpeed * m_CameraFront;
        state.UnfocusFromPlanet();
        state.EnableCursorCallback();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
        state.UnfocusFromPlanet();
        state.EnableCursorCallback();
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { // Go up pressing space
        m_CameraPos += m_CameraUp * cameraSpeed;
        state.UnfocusFromPlanet();
        state.EnableCursorCallback();
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { // Go down pressing left shift
        m_CameraPos -= m_CameraUp * cameraSpeed;
        state.UnfocusFromPlanet();
        state.EnableCursorCallback();
    }
}

glm::vec3 Camera::GetCameraPos() {
    return m_CameraPos;
}

glm::vec3 Camera::GetCameraFront() {
    return m_CameraFront;
}

glm::vec3 Camera::GetCameraUp() {
    return m_CameraUp;
}

glm::vec3 Camera::GetCameraDown() {
    return m_CameraDown;
}

glm::mat4 Camera::GetViewMatrix() {
    return m_View;
}

void Camera::SetCameraPos(glm::vec3 camPos) {
    m_CameraPos = camPos;
}

void Camera::SetCameraFront(glm::vec3 camFront) {
    m_CameraFront = camFront;
}

void Camera::SetCameraUp(glm::vec3 camUp) {
    m_CameraUp = camUp;
}

void Camera::SetCameraDown(glm::vec3 camDown) {
    m_CameraDown = camDown;
}

void Camera::SetView(glm::mat4 v) {
    m_View = v;
}

void Camera::LookAt() {
    m_View = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
}
