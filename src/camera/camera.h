#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../state/state.h"


class Camera {
  private:
    glm::vec3 m_CameraPos;
    glm::vec3 m_CameraFront;
    glm::vec3 m_CameraUp;
    glm::vec3 m_CameraDown;

    float cameraSpeed = 0.025f; // adjust accordingly

    glm::mat4 m_View;

  public:
    Camera();

    glm::vec3 getCameraPos();
    glm::vec3 getCameraFront();
    glm::vec3 getCameraUp();
    glm::vec3 getCameraDown();
    glm::mat4 getView();

    void SetCameraPos(glm::vec3 camPos);
    void SetCameraFront(glm::vec3 camFront);
    void SetCameraUp(glm::vec3 camUp);
    void SetCameraDown(glm::vec3 camDown);
    void SetView(glm::mat4 v);

    void ProcessInput(GLFWwindow* window, State state);

    void LookAt();
};