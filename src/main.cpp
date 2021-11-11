#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader/shader.h"
#include "planet/planet.h"
#include "skybox/skybox.h"
#include <stb_image.h>
//--------------- VGI: Tipus de Polars (per la Visualització Interactiva)
#define POLARZ 'Z'
#define POLARY 'Y'
#define POLARX 'X'

GLuint cubemaptexture = 0;



char Vis_Polar;		


void dibuixa_Skybox(Shader shader, GLuint cmTexture, char eix_Polar, glm::mat4 MatriuProjeccio, glm::mat4 MatriuVista) {
    glm::mat4 ModelMatrix(1.0);

    glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content

    // Activació shader per a cub skybox
    shader.Bind();
    std::cout << "printa------------------------------";
    // Pas Matrius Projecció i Vista a shader
    glUniformMatrix4fv(shader.GetUniformLocation( "u_Projection"), 1, GL_FALSE, &MatriuProjeccio[0][0]);
    glUniformMatrix4fv(shader.GetUniformLocation("u_View"), 1, GL_FALSE, &MatriuVista[0][0]);

    // Rotar skyBox per a orientar sobre eix superior Z o X en Vista Esfèrica (POLARX, POLARY, POLARZ)
    if (eix_Polar == POLARZ)
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    else if (eix_Polar == POLARX)
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    // Escalar Cub Skybox a 5000 per encabir objectes escena a l'interior
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5000.0f, 5000.0f, 5000.0f)); //glScaled(5000.0, 5000.0, 5000.0);
    glUniformMatrix4fv(shader.GetUniformLocation("u_Model"),1, GL_FALSE, &ModelMatrix[0][0]);

    // Activar textura cubemaps del Skybox per encabir objectes escena
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cmTexture);

    // Attribute Locations must be setup before calling glLinkProgram()
    glBindAttribLocation(shader.GetId(), 0, "in_Vertex"); // Vèrtexs

    //  Dibuix del Skybox
    drawCubeSkybox();

    glDepthFunc(GL_LESS); // set depth function back to default
}
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

void processInput(GLFWwindow* window) {
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

    float sensitivity = 0.5f; // change this value to your liking
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

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    glm::vec3 planetPositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(2.0f, 1.0f, 2.0f),
        glm::vec3(3.0f, 1.0f, 3.0f),
        glm::vec3(4.0f, 1.0f, 4.0f),
        glm::vec3(5.0f, 1.0f, 5.0f),
        glm::vec3(6.0f, 1.0f, 6.0f),
    };




     Shader shadersky("shaders/Skybox");
    Shader shader("shaders/Basic");
  

         
  
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    Planet planet(0.5f, 36, 18, "assets/textures/planets/earth.jpg");
  

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

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

        int modelLocation = shader.GetUniformLocation("u_Model");
        int numberOfCubes = 7;
       
        for (unsigned int i = 0; i < numberOfCubes; i++) {
            glm::mat4 model = glm::mat4(1.0f);

            // If they are not the central sphere rotate around the central sphere
            if (i != 0) {
                const float radius = 2.0f;
                float camX = sin(glfwGetTime() * (numberOfCubes - i) / 5) * radius;
                float camZ = cos(glfwGetTime() * (numberOfCubes - i) / 5) * radius;
                model = glm::translate(model, glm::vec3(camX, 0.0f, camZ) * planetPositions[i]);
            } else {
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            }

            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
            
            planet.Draw();
        }
       
        dibuixa_Skybox(shadersky, cubemaptexture, Vis_Polar, projection, view);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}
