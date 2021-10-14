#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

glm::vec3 cameraPos   = glm::vec3(0.0f,  0.0f,  10.0f);
// glm::vec3 cameraPos   = glm::vec3(0.0f,  10.0f,  0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f,  0.0f,  -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f,  1.0f,   0.0f);
glm::vec3 cameraDown  = glm::vec3(0.0f, -1.0f,   0.0f);

void processInput(GLFWwindow *window) {
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(2.0f, 1.0f, 2.0f),
        glm::vec3(3.0f, 1.0f, 3.0f),
        glm::vec3(4.0f, 1.0f, 4.0f),
        glm::vec3(5.0f, 1.0f, 5.0f),
        glm::vec3(6.0f, 1.0f, 6.0f),
    };
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    Shader shader("assets/shaders/Basic");
    
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));  

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.114, 0.125, 0.129, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();
        glBindVertexArray(VAO);

        glm::mat4 model = glm::mat4(1.0f);
        // Rotate 55 degrees so it's kind of flat on the ground
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // Rotate cube on every iteration
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));  

        // Translate on the z axis to move the scene forward thus our camera backwards
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        // view = glm::lookAt(cameraPos, cameraPos + cameraDown, cameraFront);

        // Prespective projection matrix
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);

        // Send Model, View, Projection matrices to the shader {{{
        int viewLocation = shader.GetUniformLocation("u_View");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLocation = shader.GetUniformLocation("u_Projection");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        int numberOfCubes = sizeof(cubePositions)/sizeof(*cubePositions);
        for (unsigned int i = 0; i < numberOfCubes; i++) {
            glm::mat4 model = glm::mat4(1.0f);

            // If they are not the central cube rotate around the central cube
            if (i != 0) {
                const float radius = 2.0f;
                float camX = sin(glfwGetTime() * (numberOfCubes - i) / 5) * radius;
                float camZ = cos(glfwGetTime() * (numberOfCubes - i) / 5) * radius;
                float camY = cos(glfwGetTime() * i * (numberOfCubes - i));
                model = glm::translate(model, glm::vec3(camX, camY, camZ) * cubePositions[i]);
            }
            else {
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            int modelLocation = shader.GetUniformLocation("u_Model");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

            int colorLocation = shader.GetUniformLocation("u_Color");
            if (i == 0)
                glUniform4f(colorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
            else
                glUniform4f(colorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
