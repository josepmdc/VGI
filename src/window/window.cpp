#include "window.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <iostream>

namespace window {
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

State* state;
Camera* camera;

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.2f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    // cameraFront = glm::normalize(front);
    camera->SetCameraFront(glm::normalize(front));
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    state->Modify(key, action);
}

Window InitWindow(State* s, Camera* cam) {
    GLFWwindow* glfwWindow;

    state = s;
    camera = cam;

    Window window;
    window.glfwWindow = nullptr;

    /* Initialize the library */
    if (!glfwInit())
        return window;

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window.ScreenWidth = mode->width;
    window.ScreenHeight = mode->height;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindow = glfwCreateWindow(window.ScreenWidth, window.ScreenHeight, "OpenGL", NULL, NULL);
    if (!glfwWindow) {
        glfwTerminate();
        return window;
    }

    glfwMakeContextCurrent(glfwWindow);
    glfwSetKeyCallback(glfwWindow, KeyCallback);

    window.glfwWindow = glfwWindow;

    return window;
}
}; // namespace window
