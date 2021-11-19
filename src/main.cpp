#include <chrono>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SpiceUsr.h>

#include "shader/shader.h"
#include "planet/planet.h"
#include "skybox/skybox.h"
#include "util/util.h"
#include "state/state.h"
#include "gui/gui.h"

State state;

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

void GetDate(std::string& date) {
    int hour = 23;
    int day = 0;
    int month = 0;
    int year = 1999;
    while (true) {
        hour = hour == 23 ? hour = 0 : hour + 1;
        day = hour == 0 ? (day == 28 ? 1 : day + 1) : day;
        month = day == 1 && hour == 0 ? (month == 12 ? 1 : month + 1) : month;
        year = month == 1 && day == 1 & hour == 0 ? year + 1 : year;
        date = std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day) + "T" + std::to_string(hour);
        std::cout << date << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
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

    // Load spice kernel for planetary data
    furnsh_c("assets/kernels/de440.bsp");
    furnsh_c("assets/kernels/naif0012.tls");

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

    glm::dvec3 position = glm::dvec3(0.0);
    double ephemerisTime = 0.0;

    std::string date;
    std::thread dateThread(GetDate, std::ref(date));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        glfwSetCursorPosCallback(window,
                                 state.CursorCallbackDisabled() ? NULL : mouse_callback);

        processInput(window);

        glfwSetInputMode(window, GLFW_CURSOR,
                         state.CursorDisabled() ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

        GUI::NewFrame();

        glClearColor(0.114, 0.125, 0.129, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 5000.0f);

        shader.SetMat4("u_View", view);
        shader.SetMat4("u_Projection", projection);

        str2et_c(date.c_str(), &ephemerisTime);

        int i = planets.size();

        int modelLocation = shader.GetUniformLocation("u_Model");
        std::vector<Planet*> selectedPlanets = state.RealisticModePlanetsEnabled() ? planets : academicPlanets;
        for (Planet* planet : selectedPlanets) {
            float radius = state.RealisticModeOrbitsEnabled() ? planet->GetOrbitRadius() : state.GetOrbitRadius();
            float camX = sin(glfwGetTime() / (5 - i)) * radius;
            float camZ = cos(glfwGetTime() / (5 - i)) * radius;

            if (planet->GetName() == "sun") {
                position = glm::vec3(0.0f, 0.0f, 0.0f);
            } else {
                double lt;
                spkpos_c((planet->GetName() + " barycenter").c_str(), ephemerisTime, "ECLIPJ2000", "None", "Sun", glm::value_ptr(position), &lt);
            }

            position *= 0.00000003; // scale down the planet position so they are not too far

            if (planet->GetName() == state.GetSelectedPlanet()) {
                state.SetCurrentPosition(glm::vec3(position[0], position[2], position[1]));
            }

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(position[0], position[2], position[1]));
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

            planet->Draw();

            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
            planet->DrawOrbit();

            i++;
        }

        skybox.Draw(projection, view);

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
