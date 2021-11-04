#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "shader/shader.h"
#include "planet/planet.h"
#include "util/util.h"
#include "state/state.h"
#include "gui/gui.h"

State state;

unsigned int LoadTexture(std::string path);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraDown = glm::vec3(0.0f, -1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
glm::vec3 earthPos = glm::vec3(.0f, .0f, .0f);

void processInput(GLFWwindow* window) {
    const float cameraSpeed = 0.025f; // adjust accordingly
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
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
    cameraFront = glm::normalize(front);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    state.Modify(key, action);
}

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    unsigned int SCR_WIDTH = mode->width;
    unsigned int SCR_HEIGHT = mode->height;

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", glfwGetPrimaryMonitor(), NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glfwSetKeyCallback(window, KeyCallback);

    GUI::SetUp(window);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/Basic");

    std::vector<Planet*> planets = util::LoadPlanets();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        glfwSetCursorPosCallback(window,
                                 state.CursorCallbackEnabled() ? mouse_callback : NULL);

        processInput(window);

        glfwSetInputMode(window, GLFW_CURSOR,
                         state.CursorEnabled() ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

        GUI::NewFrame();

        glClearColor(0.114, 0.125, 0.129, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        int viewLocation = shader.GetUniformLocation("u_View");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        int projectionLocation = shader.GetUniformLocation("u_Projection");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        float i = planets.size();

        int modelLocation = shader.GetUniformLocation("u_Model");

        for (Planet* planet : planets) {
            float radius = state.RealisticModeEnabled() ? planet->GetOrbitRadius() : state.GetOrbitRadius();
            float camX = sin(glfwGetTime() / (5 - i)) * radius;
            float camZ = cos(glfwGetTime() / (5 - i)) * radius;

            if (planet->GetName() == state.GetSelectedPlanet()) {
                state.SetCurrentPosition(glm::vec3(camX, 0.0f, camZ) * planet->GetCoordinates());
            }

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(camX, 0.0f, camZ) * planet->GetCoordinates());
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

            planet->Draw();

            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
            planet->DrawOrbit();

            i++;
        }

        GUI::DrawControls(planets, state);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (Planet* planet : planets) {
        delete planet;
    }

    GUI::Destroy();
    glfwTerminate();

    return 0;
}
