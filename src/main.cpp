#include <chrono>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <thread>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "glm/fwd.hpp"
#include "shader/shader.h"
#include "planet/planet.h"
#include "skybox/skybox.h"
#include "util/util.h"
#include "state/state.h"
#include "camera/camera.h"
#include "gui/gui.h"
#include "spice/spice.h"
#include "window/window.h"

State state;
Camera camera;

void GetDate() {
    struct std::tm tm;
    while (true) {
        tm.tm_year = state.GetYear();
        tm.tm_mon = state.GetMonth();
        tm.tm_mday = state.GetDay();
        tm.tm_hour = state.GetHour();

        switch (state.GetSpeedMode()) {
        case SpeedMode::Minutes:
            util::addMin(tm);
            break;
        case SpeedMode::Hours:
            util::addHour(tm);
            break;
        case SpeedMode::Days:
            util::addDay(tm);
            break;
        default:
            util::addHour(tm);
            break;
        }
        state.SetDate(tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main(void) {
    window::Window window = window::InitWindow(&state, &camera);
    if (window.glfwWindow == nullptr) {
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    spice::Init();
    GUI::SetUp(window.glfwWindow);

    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/Basic");

    Skybox skybox("shaders/Skybox", { "assets/textures/skybox/right.png",
                                      "assets/textures/skybox/left.png",
                                      "assets/textures/skybox/top.png",
                                      "assets/textures/skybox/bottom.png",
                                      "assets/textures/skybox/front.png",
                                      "assets/textures/skybox/back.png" });

    std::vector<Planet*> planets = util::LoadPlanets(false);
    std::vector<Planet*> academicPlanets = util::LoadPlanets(true);

    std::thread dateThread(GetDate);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window.glfwWindow)) {
        glClearColor(0.114, 0.125, 0.129, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSetCursorPosCallback(window.glfwWindow, state.CursorCallbackDisabled() ? NULL : window::MouseCallback);
        glfwSetInputMode(window.glfwWindow, GLFW_CURSOR, state.CursorDisabled() ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

        GUI::NewFrame();

        shader.Bind();

        camera.ProcessInput(window.glfwWindow, state);
        camera.LookAt();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window.ScreenWidth / (float)window.ScreenHeight, 0.1f, 5000.0f);
        shader.SetMat4("u_Projection", projection);
        shader.SetMat4("u_View", camera.GetViewMatrix());

        std::vector<Planet*> selectedPlanets = state.RealisticModePlanetsEnabled() ? planets : academicPlanets;
        RenderPlanets(selectedPlanets, state, camera, shader);

        skybox.Draw(projection, camera.GetViewMatrix());

        GUI::DrawControls(planets, academicPlanets, state);

        glfwSwapBuffers(window.glfwWindow);
        glfwPollEvents();
    }

    for (Planet* planet : planets) {
        delete planet;
    }

    GUI::Destroy();
    glfwTerminate();

    return 0;
}
