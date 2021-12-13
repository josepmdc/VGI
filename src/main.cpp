#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Windows.h>


#include "shader/shader.h"
#include "planet/planet.h"
#include "skybox/skybox.h"
#include "util/util.h"
#include "state/state.h"
#include "camera/camera.h"
#include "gui/gui.h"

#pragma comment(lib, "Winmm.lib")

State state;
Camera camera;

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

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
    //cameraFront = glm::normalize(front);
    camera.SetCameraFront(glm::normalize(front));
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
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

    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/Basic");

    std::vector<std::string> skyboxFaces = { "assets/textures/skybox/right.png",
                                             "assets/textures/skybox/left.png",
                                             "assets/textures/skybox/top.png",
                                             "assets/textures/skybox/bottom.png",
                                             "assets/textures/skybox/front.png",
                                             "assets/textures/skybox/back.png" };

    Skybox skybox("shaders/Skybox", skyboxFaces);

    std::vector<Planet*> planets = util::LoadPlanets(false);
    std::vector<Planet*> academicPlanets = util::LoadPlanets(true);

    if (!state.RealisticModeOrbitsEnabled()) {
        for (Planet* planet : planets) {
            planet->GenerateOrbit(state.GetOrbitRadius());
        }
        for (Planet* planet : academicPlanets) {
            planet->GenerateOrbit(state.GetOrbitRadius());
        }
    }

    // play sound on loop
    PlaySound("./assets/sounds/space_ambience.wav", GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC | SND_LOOP);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        glfwSetCursorPosCallback(window,
                                 state.CursorCallbackDisabled() ? NULL : mouse_callback);

        //processInput(window);
        camera.ProcessInput(window, state);

        glfwSetInputMode(window, GLFW_CURSOR,
                         state.CursorDisabled() ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

        GUI::NewFrame();

        glClearColor(0.114, 0.125, 0.129, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();

        camera.LookAt();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 5000.0f);

        shader.SetMat4("u_View", camera.getView());
        shader.SetMat4("u_Projection", projection);

        float i = planets.size();

        int modelLocation = shader.GetUniformLocation("u_Model");
        std::vector<Planet*> selectedPlanets = state.RealisticModePlanetsEnabled() ? planets : academicPlanets;
        for (Planet* planet : selectedPlanets) {
            float radius = state.RealisticModeOrbitsEnabled() ? planet->GetOrbitRadius() : state.GetOrbitRadius();
            float camX = sin(glfwGetTime() / (5 - i)) * radius;
            float camZ = cos(glfwGetTime() / (5 - i)) * radius;

            if (planet->GetName() == state.GetSelectedPlanet()) {
                state.SetCurrentPosition(glm::vec3(camX, planet->GetRadius(), camZ) * planet->GetCoordinates());
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

        skybox.Draw(projection, camera.getView());

        GUI::DrawControls(planets, academicPlanets, state);

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
