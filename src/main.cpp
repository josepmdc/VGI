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
#ifdef _WIN32
#include <Windows.h>
#endif
#include <date.h>
#include <mutex>

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

#pragma comment(lib, "Winmm.lib")

State state;
Camera camera;
std::mutex mtx; // TODO

void GetDate() {
    using namespace date;
    using namespace std::chrono;

    auto date = sys_days{ year{ state.GetYear() } / month{ state.GetMonth() } / day{ state.GetDay() } } + hours{ state.GetHour() } + minutes{ state.GetMinute() };

    while (true) {
        date = sys_days{ year{ state.GetYear() } / month{ state.GetMonth() } / day{ state.GetDay() } } + hours{ state.GetHour() } + minutes{ state.GetMinute() };

        int sleepDuration = 0;
        switch (state.GetSpeedMode()) {
        case SpeedMode::Slow:
            sleepDuration = 500;
            break;
        case SpeedMode::Normal:
            sleepDuration = 10;
            break;
        default:
            sleepDuration = 0;
            break;
        }

        date += 1min;

        std::stringstream date_str;
        date_str << date;

        auto dp = floor<days>(date);
        auto ymd = year_month_day{ dp };
        auto time = make_time(duration_cast<milliseconds>(date - dp));
        state.SetDate(
            static_cast<int>(ymd.year()),
            static_cast<unsigned int>(ymd.month()),
            static_cast<unsigned int>(ymd.day()),
            time.hours().count(),
            time.minutes().count(),
            date_str.str());

        std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration));
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

#ifdef _WIN32
    // play sound on loop
    PlaySound("./assets/sounds/space_ambience.wav", GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC | SND_LOOP);
#endif

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
