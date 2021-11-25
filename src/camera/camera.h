#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

class Camera {
  private:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraDown;

    float cameraSpeed = 0.025f; // adjust accordingly

  public:
    ~Camera();
    Camera();

    glm::vec3 getCameraPos();
    glm::vec3 getCameraFront();
    glm::vec3 getCameraUp();
    glm::vec3 getCameraDown();

    void setCameraPos(glm::vec3 camPos);
    void setCameraFront(glm::vec3 camFront);
    void setCameraUp(glm::vec3 camUp);
    void setCameraDown(glm::vec3 camDown);

    glm::mat4 
    void processInput(GLFWwindow* window);
};