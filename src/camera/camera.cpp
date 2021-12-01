#pragma once

#include "camera.h"


Camera ::Camera() {
    cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraDown = glm::vec3(0.0f, -1.0f, 0.0f);

    view = glm::mat4(1.0f);
}



Camera::~Camera()
    {
    }

void Camera::processInput(GLFWwindow* window, State state) {
    //const float cameraSpeed = 0.025f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //Go up pressing space
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) //Go down pressing left shift
        cameraPos -= cameraUp * cameraSpeed;
    //prototype for debugging purposes
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        cameraPos = state.GetCurrentPosition(); //get position of earth
}

glm::vec3 Camera::getCameraPos() {
    return cameraPos;
}

glm::vec3 Camera::getCameraFront() {
    return cameraFront;
}


glm::vec3 Camera::getCameraUp() {
    return cameraUp;
}

glm::vec3 Camera::getCameraDown() {
    return cameraDown;
}

glm::mat4 Camera::getView() {
    return view;
}


void Camera::setCameraPos(glm::vec3 camPos)
{
    cameraPos = camPos;
}

void Camera::setCameraFront(glm::vec3 camFront)
{
    cameraFront = camFront;
}

void Camera::setCameraUp(glm::vec3 camUp)
{
    cameraUp = camUp;
}

void Camera::setCameraDown(glm::vec3 camDown)
{
    cameraDown = camDown;
}

void Camera::setView(glm::mat4 v) {
    view = v;
}

void Camera::lookAt() {
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
