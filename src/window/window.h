#include "../camera/camera.h"
#include "../state/state.h"
#include <GLFW/glfw3.h>

namespace window {
struct Window {
    GLFWwindow* glfwWindow;
    unsigned int ScreenWidth;
    unsigned int ScreenHeight;
};

void MouseCallback(GLFWwindow* window, double xpos, double ypos);
Window InitWindow(State* state, Camera* camera);
}; // namespace Window
